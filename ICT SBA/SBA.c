//Food ordering system
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<conio.h>
#include<ctype.h>
//Input password
void input_password (char password[]){
	char c;
	int i=0;
	while((c=getch())>=32&&c<=126||c==8&&i<10){
        password[i++]=c;
        printf("*");
		}
	password[i]='\0'; // set null character
}
//Press number keys
int press(int i){
	char c;
	int d;
	/*Users need to press the number key (from 1 to i). The algorithm lets users press the key, change into integer and return the integer.
	  Also, there are validation check by using recursive function*/
	if((c=getch())>=49&&c<=48+i){ // set the range of number key
        d=c-48; // change the character into a integer
        printf("\n");
		}
	else 
		d=press(i);//recursive function
	return d; // return the integer
}
//change password (for owner)
void change_password(char password[],int n){
	char new_password[10],double_check[10];;
	int flat;
	FILE *fp;
	/* If n is 1, password for owner and waiters is changed.
	   Otherwise, password for VIPs is changed.*/
	//File is opened for overwritten
	if(n==1)
		fp=fopen("password_owner.txt","w");
	else if (n==2)
		fp=fopen("password_waiters.txt","w");
	else
		fp=fopen("VIP_password.txt","w");
	printf("\n Input new password(<=10 characters):");
	input_password(new_password);
	// Double input the new_password
	printf("\n Input again to avoid trancription error:");
	input_password(double_check);
	// Check whether 2 passwords are the same. If no, input again
	while(strcmp(double_check,new_password)!=0){
		printf("\n It does not match! \n Press new password again(<=10 characters):");
		input_password(new_password);
		printf("\n Input again to avoid trancription error:");
		input_password(double_check);
	}
	printf("\n Confirm?");
	printf("\n Yes: Press 1. \n No: Press 2.");
	flat=press(2);
    if(flat==2)
    	change_password(password,n);// recursive function
    fprintf(fp,"%s",new_password); // overwrite the new password to the file
    fclose(fp);
}
//read the dishes and prices from a file (for owner)
void read(char dishes[][54],int prices[],int *no_of_dishes){
	FILE *fp;
	fp=fopen("dishes.txt","r"); // file is opened for read
	*no_of_dishes=0; // called by reference
	int i;
	char c;
	// count number of dishes
	do{
		c=fgetc(fp);
		if(c=='\n')
			*no_of_dishes=*no_of_dishes+1;
	}while(c!=EOF);
	fclose(fp);
	fp=fopen("dishes.txt","r");// file is opened for read
	/* The dishes[][54] originally involves the name of the dish (50 characters) and the price (3 characters)
	   The algorithm helps us to separate them */
	for(i=0;i<*no_of_dishes;i++){
		fgets(dishes[i],54,fp);
		prices[i]=atoi(&dishes[i][50]);
		dishes[i][50]='\0';
		}
	fclose(fp);
}
//output the dishes and prices (for owner)
void display(char dishes[][54],int prices[],int no_of_dishes){
	int i;
	printf("\n");
	for(i=0;i<no_of_dishes;i++){
		printf("%2d. %s $%2d\n",i+1,dishes[i],prices[i]);
	}
}
//add a dishes (for owner)
void add_dishes(){
	char new_dish[50];
	int new_price;
	printf("\n The dish you want to add:");
	fflush(stdin);
	gets(new_dish);
	printf(" The price that the dish is:");
	scanf("%d",&new_price);
	FILE *fp;
	fp=fopen("dishes.txt","a"); // file is opened for appended
	fprintf(fp,"%-50s%d\n",new_dish,new_price);
	printf(" Adding a dish successfully!\n");
	fclose(fp);
}
//delete a dish (for owner)
void delete_a_dish(char dishes[][54],int prices[],int no_of_dishes){
	int i=0,deleted_dish;
	read(dishes,prices,&no_of_dishes);
	display(dishes,prices,no_of_dishes);// display the dishes for the owner to check which dish to be deleted
	printf("\n The dish you want to remove (no.): ");
	scanf("%d",&deleted_dish);
	FILE *fp;
	fp=fopen("dishes.txt","w");// file is opened for overwritten
	/* If the dishes[i][54] is not needed to be deleted, it will be overwritten to the file.
	   The dish which is needed to be deleted will not be overwritten to the file*/
	while(i<no_of_dishes){
		if(i!=deleted_dish-1) //deleted_dish-1 is because i starts from 0
			fprintf(fp,"%-50s%d\n",dishes[i],prices[i]);
		i++;
	}
	printf(" Successful!\n");
	fclose(fp);
}
//amend prices (for owner)
void amend_prices(char dishes[][54],int prices[],int no_of_dishes){
	int change_dish,new_price,i=0;
	read(dishes,prices,&no_of_dishes);
	display(dishes,prices,no_of_dishes);
	printf("\n The price that you want to change (no.): ");
	scanf("%d",&change_dish);
	printf(" The new price: ");
	scanf("%d",&new_price);
	FILE *fp;
	fp=fopen("dishes.txt","w"); // file is opened for overwritten
	/* If the price of dishes[i][54] is not needed to be amended, it will be overwritten to the file.
	   The dish which its price is needed to be deleted will be overwritten its new price to the file*/
	while(i<no_of_dishes){
		if(i==change_dish-1)
			fprintf(fp,"%-50s%d\n",dishes[i],new_price);
		else
			fprintf(fp,"%-50s%d\n",dishes[i],prices[i]);
		i++;
	}
	fclose(fp);
}
//change or show the dishes and prices (for owner)
void change_or_display_record(char dishes[][54],int prices[],int no_of_dishes){
	int input_choice=0;
	// Give 4 choices for the owner to choose
	printf("\n Display all dishes and prices:\tPress 1.\n");
	printf(" Add a dish:\t\t\tPress 2.\n");
	printf(" Delete a dish:\t\t\tPress 3.\n");
	printf(" Amend the price of a dishes:\tPress 4.\n");
	input_choice=press(4);
    if(input_choice==1){
    	read(dishes,prices,&no_of_dishes);
    	display(dishes,prices,no_of_dishes);
    }
    if(input_choice==2)
    	add_dishes();
    if(input_choice==3)
    	delete_a_dish(dishes,prices,no_of_dishes);
    if(input_choice==4)
    	amend_prices(dishes,prices,no_of_dishes);
}
//check recent orders (for owner)
void recent_orders(){
	FILE *fp;
	int input_choice;
	printf("\n Shown in text: press 1.\n Shown in Spreadsheet: press 2.");
	input_choice=press(2);
	if(input_choice==1){
		fp=fopen("orders.txt","r");// file is opened for read
		char c;
		while((c=fgetc(fp))!=EOF){
			printf("%c",c); // output all text in orders.txt
			if(c=='\n')
				printf(" ");
		}
		fclose(fp);
	}
	else
		system("statistics.csv");
}
//read comments (for owner)
void read_comments(){
	FILE *fp;
	char ch;
	fp=fopen("comment.txt","r");// file is opened for read
	if(fp==NULL)
		printf("\n No comment!\n");
	else
	do{
		ch=fgetc(fp);
		if(ch!=EOF)
			printf("%c",ch);//Output all comments
		if(ch=='\n')
			printf(" ");
	}while(ch!=EOF);
	fclose(fp);
}
//log in (for owner,VIPs,waiters)
int log_in(int n){
	FILE *fp;
	int  wrong=0;
    char password[10], password_input[10];
    /* If n is 1, password for owner  is read.
       If n is 2, password for waiters is read.
	   Otherwise, password for VIPs is read.*/
	//File is opened for read
    if(n==1)
    	fp=fopen("password_owner.txt","r");
    else if(n==2)
    	fp=fopen("password_waiters.txt","r");
    else
    	fp=fopen("VIP_password.txt","r");
    fscanf(fp,"%s",password);// read the password and pass it to the string
    printf(" Input the password(<=10 characters):");
    input_password(password_input);
	while(strcmp(password_input,password)!=0&&wrong<4){// check if the inputted password and the correct password are the same
		printf("\n Wrong Password!\n Input again:");
		input_password(password_input);
		wrong++;
		}
	if(wrong>=3) // When the user input wrongly for 5 times, he/she is not allowed to log in
		printf("\n Do you forget the password? You have inputted wrongly 5 times!\n");
	if(strcmp(password_input,password)==0)
		return 1; // 1 for success log-in
	else
		return 0; // 1 for unsuccess log-in
	fclose(fp);
}
//sorting (alphabetical order) (for customers)
void sort_alpha(int no_of_dishes,int sorted_prices[],char sorted_dishes[][54]){
	/* Sorting (insertion sort) the dishes in alphabetical order
	   The insertion sort is based on the idea of repeatedly inserting a new data into a sorted list and then shifting
	   The main sorted list is sorted_dishes[][54]. When shifting, the corresponding sorted_prices[] will also be shifting*/
	char next_element[51];
	int i, j, n, shifting;
	for (i=1;i<no_of_dishes;i++){
		strcpy(next_element,sorted_dishes[i]);
		n=sorted_prices[i];
		j=i-1;
		shifting=1;
		while(j>=0 && shifting)
			if(strcmp(sorted_dishes[j],next_element)>0){
				strcpy(sorted_dishes[j+1],sorted_dishes[j]);
				sorted_prices[j+1]=sorted_prices[j];
				j=j-1;
			}
			else
				shifting=0;
		strcpy(sorted_dishes[j+1],next_element);
		sorted_prices[j+1]=n;
	}
	display(sorted_dishes,sorted_prices,no_of_dishes);//display the sorted lists by using function 'display'
}
//sorting (ascending) (for customers)
void sort_ascend(int no_of_dishes,int sorted_prices[],char sorted_dishes[][54]){
	/* Sorting (selection sort) the prices in ascending order
	   The insertion sort is to swap the minimum value with the first value, then repeat the steps and swap with second, third ... values, etc.
	   The main sorted list is sorted_prices[]. When swapping, the corresponding sorted_dishes[][54] will also be swapping*/
	//temp1 and temp[] are used to save the temporary data
	int temp1,i,round, min_price, min_position; 
	char temp[51];
	for (round =0; round <= no_of_dishes-2; round++){
		min_price = sorted_prices[round];
		min_position = round;
		for (i = round+1; i<=no_of_dishes-1; i++)
			if (min_price > sorted_prices[i]){
				min_price = sorted_prices[i];
				min_position = i;
			}
	//swapping (prices)
	temp1=sorted_prices[round];
	sorted_prices[round]=sorted_prices[min_position];
	sorted_prices[min_position]=temp1;
	//swapping (dishes)
	strcpy(temp,sorted_dishes[round]);
	strcpy(sorted_dishes[round],sorted_dishes[min_position]);
	strcpy(sorted_dishes[min_position],temp);
	}
	display(sorted_dishes,sorted_prices,no_of_dishes);//display the sorted lists by using function 'display'
}
//Searching (for customers)
int search_dish(int no_of_dishes,int prices[],char dishes[][54],int search1[]){
	/*Searching for the dish like search engine*/
	char search[80];
	int find[no_of_dishes], plus[40];
	int i,j, k, no_of_plus=1;
	printf(" Input keywords (separate with '+'and end with ';'): ");
	fflush(stdin);
	gets(search);
	for(i=0;i<no_of_dishes;i++)// initialize
		find[i]=0;
	for(i=0;i<40;i++)// initialize
		plus[i]=0;
	for(i=0;i<strlen(search);i++)
		if(search[i]=='+'||search[i]==';'){
			plus[no_of_plus]=i+1;
			no_of_plus++;
		}
	no_of_plus--;
	for(i=1;i<=no_of_plus;i++)
		for(j=0;j<no_of_dishes;j++){
			k=0;
			while(k<50){
				if(strncmp(&search[plus[i-1]],&dishes[j][k],plus[i]-plus[i-1]-1)==0)
					find[j]=find[j]+1;
			k++;
			}
		}
	k=0;
	for(i=0;i<no_of_dishes;i++){
		if(find[i]>0)
			search1[i]=1;
		else
			k++;
		}
	if(k==no_of_dishes)
		return 2;
	else
		return 1;
}
//VIP (for customers)
double VIP(char VIP_no[]){
	char VIP_code[7],find[50][7];
	int i=0, j=0, correct=0;
	double discount;
	printf("\n Input your VIP no. (e.g. 001):");
	FILE *fp;
	fp=fopen("VIP.txt","r");// file is opened for read
	/* To check if the inputted VIP_no is a string with numbers, not other characters.
	   Otherwise, input the VIP_no again*/
	while(!correct){
		correct=1;
		scanf("%s",VIP_no);
		while(correct&&i<=2){
			if(isdigit(VIP_no[i])!=1||strlen(VIP_no)!=3)
				correct=0;
			if(!correct)
				printf(" Wrong! Input again: ");
			i++;
		}
	}
	i=0;
	// Combine the "VIP" and VIP_no into VIP_code
	strcpy(VIP_code,"VIP");
	strcat(VIP_code,VIP_no);
	VIP_code[6]='\0';
	printf(" VIP: %s\n",VIP_code);
	// Check if VIP_code is in the file, if not, output "No that VIP!"
	while(fgets(find[i],8,fp)!=NULL){
		find[i][6]='\0';
		i++;
	}
	while(strcmp(find[j],VIP_code)!=0){
		j++;
		if(j>i){
			printf(" No that VIP!");
			return 1;
		}
	}
	/* The discount shown in the file is "_%" form.
	   The algorithm helps us receive the discount (in string form) and convert to floating point number form*/
	find[j+1][3]='\0';
	discount=atof(find[j+1]);
	discount=discount/100.0;
	fclose(fp);
	return 1-discount;
}
//Add VIP (for customers)
int add_VIP(int discount){
	FILE *fp;
	fp=fopen("VIP.txt","a+");// file is opened for appended and read
	char c;
	int count=0; // variable count is used for counting the number of VIPs there are
	while((c=fgetc(fp))!=EOF)
		if(c=='%')
			count++;
	count++;
	if(count<10)
		fprintf(fp,"\nVIP00%d\n%d%%\n",count,discount);
	else
		fprintf(fp,"\nVIP0%d\n%d%%\n",count,discount);
	fclose(fp);
	return count;
}
//Add dishes (for customers)
void add_ordered_dishes(int ordered_dishes[],int *count,int no_of_dishes){
	/* To add the dishes which is for validation*/
	int input,input_no,i,input_choice=0;
	printf(" Which dish (number) (e.g. 25) you want to add?\n");
	printf(" Input: ");
	scanf("%d",&input);
	while(input<0||input>no_of_dishes){// validation check
			printf(" Wrong Input!\n");
			printf(" Input a dish again: ");
			scanf("%d",&input);
	}
	if(input!=0){
		printf(" Number of the dish (%d) you want: ",input);
		scanf("%d",&input_no);
		while(input_no<=0){// validation check
			printf(" Wrong Input!\n");
			printf(" Input again: ");
			scanf("%d",&input_no);
			}
		ordered_dishes[input-1]=ordered_dishes[input-1]+input_no;
		}
	*count=*count+1;// called by reference
	//organize all ordered dishes and output them for double check
	printf("\n  %-18s%-10s\n","Dishes You Chose","quantities");
	for (i=0;i<no_of_dishes;i++)
		if(ordered_dishes[i]!=0)
			printf("  Dish %-13d%d\n",i+1,ordered_dishes[i]);
	printf(" Completed?");
	printf(" \n Yes: Press 1\n");
	printf(" No: Press 2\n");
	input_choice=press(2);
	if(input_choice==2)
		add_ordered_dishes(ordered_dishes,&*count,no_of_dishes);//recursive function
}
//Delete dishes (for customers)
void delete_ordered_dishes(int ordered_dishes[],int *count,int no_of_dishes){
	/* To delete the dishes which is for validation*/
	int input,i,input_choice=0;
	printf(" Which dish (number) (e.g. 25) you want to delete?\n");
	printf(" Input: ");
	scanf("%d",&input);
	while((input<0||input>no_of_dishes)||ordered_dishes[input-1]==0){// validation check
		printf(" Wrong Input!\n");
		printf(" Input a dish again: ");
		scanf("%d",&input);
	}
	ordered_dishes[input-1]=0;
	*count=*count-1;// called by reference
	//organize all ordered dishes and output them for double check
	printf("\n  %-18s%-10s\n","Dishes You Chose","quantities");
	for (i=0;i<no_of_dishes;i++)
		if(ordered_dishes[i]!=0)
			printf("  Dish %-13d%d\n",i+1,ordered_dishes[i]);
	printf(" Completed?");
	printf(" \n Yes: Press 1\n");
	printf(" No: Press 2\n");
	input_choice=press(2);
	if(input_choice==2)
		delete_ordered_dishes(ordered_dishes,&*count,no_of_dishes);// recursive function
}
//Amend number of dishes (for customers)
void amend_ordered_dishes(int ordered_dishes[],int count,int no_of_dishes){
	/* To amend the price of the dishes which is for validation*/
	int input,input_no,i,input_choice=0;
	printf(" Which dish (number) (e.g. 25) you want to amend its quantity?\n");
	printf(" Input: ");
	scanf("%d",&input);
	while((input<0||input>no_of_dishes)||ordered_dishes[input-1]==0){// validation check
		printf(" Wrong Input!\n");
		printf(" Input a dish again: ");
		scanf("%d",&input);
	}
	printf("Input the quantity of the Dish %d: ",input);
	scanf("%d",&input_no);
	while(input_no<=0){// validation check
			printf(" Wrong Input!\n");
			printf(" Input again: ");
			scanf("%d",&input_no);
			}
	ordered_dishes[input-1]=input_no;
	//organize all ordered dishes and output them for double check
	printf("\n  %-18s%-10s\n","Dishes You Chose","quantities");
	for (i=0;i<no_of_dishes;i++)
		if(ordered_dishes[i]!=0)
			printf("  Dish %-13d%d\n",i+1,ordered_dishes[i]);
	printf(" Completed?");
	printf(" \n Yes: Press 1\n");
	printf(" No: Press 2\n");
	input_choice=press(2);
	if(input_choice==2)
		amend_ordered_dishes(ordered_dishes,count,no_of_dishes);// recursive function
}
//ordering
void ordering(int no_of_dishes,int prices[],char dishes[][54],int search1[],int input_no,int ordered_dishes[],int sorted_prices[],char sorted_dishes[][54],int *count){
	printf("\n Searching: Do you want to search for the keywords about the dishes you want to have?");
	printf("\n Yes: Press 1.\n No: Press 2");
	int input_choice,input_choice1,input,search=2,i;
	input_choice=press(2);
	if(input_choice==1){
		// searching (function)
		search=search_dish(no_of_dishes,prices,dishes,search1);
		if(search==2)
			printf("\n Sorry to tell you that no food matches up your keywords!");
	}
	if(search==1){
		for(i=0;i<no_of_dishes;i++){
			if(search1[i]==1)
				printf("%2d. %s $%2d\n",i+1,dishes[i],prices[i]);
		}
		printf("\n Which above dish(es) do you want to like?\n ");
		printf("Input the dishes (e.g. 25) you like. Input 0 if you finish inputting.\n");
		printf(" After inputting a dish, press Enter key.\n");
		do{
			printf(" Input a dish: ");
			scanf("%d",&input);
			while(input<0||input>no_of_dishes||(input!=0&&search1[input-1]!=1)){// validation check
				printf(" Wrong Input!\n");
				printf(" Input a dish again: ");
				scanf("%d",&input);
			}
			if(input!=0){
				printf(" Number of the dish (%d) you want: ",input);
				scanf("%d",&input_no);
				while(input_no<=0){// validation check
					printf(" Wrong Input!\n");
					printf(" Input again: ");
					scanf("%d",&input_no);
				}
				ordered_dishes[input-1]=ordered_dishes[input-1]+input_no;
			}
			*count=*count+1;
		}while(input!=0);// Input 0 to exit the loop
		search=2;
	}
	if(input_choice==2||search==2){
		printf("\n You can now choose the dishes by sorting!");
		printf("\n Sorting in Alphabetical Order:\t\tPress 1.");
		printf("\n Sorting in Ascending Order (prices):\tPress 2.");
		input_choice=press(2);
		// sorting (function)
		if(input_choice==1)
			sort_alpha(no_of_dishes,sorted_prices,sorted_dishes);
		else 
			sort_ascend(no_of_dishes,sorted_prices,sorted_dishes);
		printf("\n Which dishes do you want to like?\n ");
		printf("Input the dishes (e.g. 25) you like. Input 0 if you finish inputting.\n");
		printf(" After inputting a dish, press Enter key.\n");
		do{
			printf(" Input a dish: ");
			scanf("%d",&input);
			while(input<0||input>no_of_dishes){// validation check
				printf(" Wrong Input!\n");
				printf(" Input a dish again: ");
				scanf("%d",&input);
			}
			if(input!=0){
				printf(" Number of the dish (%d) you want: ",input);
				scanf("%d",&input_no);
				while(input_no<=0){// validation check
					printf(" Wrong Input!\n");
					printf(" Input again: ");
					scanf("%d",&input_no);
				}
				ordered_dishes[input-1]=ordered_dishes[input-1]+input_no;
			}
			*count=*count+1;
		}while(input!=0);// Input 0 to exit the loop
	}
	//organize all ordered dishes and output them for double check
	printf("\n  %-18s%-10s\n","Dishes You Chose","quantities");
	for (i=0;i<no_of_dishes;i++)
		if(ordered_dishes[i]!=0)
			printf("  Dish %-13d%d\n",i+1,ordered_dishes[i]);
	printf(" Confirm?");
	printf(" \n Yes: Press 1\n");
	printf(" No: Press 2\n");
	input_choice=press(2);
	while(input_choice==2){// When input_choice is 2, the record of ordered dishes is needed to be changed
		input_choice=1;
		printf(" Do you want to add, delete or amend the quantities of the dishes?\n");
		// Give 4 choices for the owner to choose
		printf(" Add: Press 1\n");
		printf(" Delete: Press 2\n");
		printf(" Amend: Press 3\n");
		printf(" Nothing is needed to be changed: Press 4\n");
		input_choice1=press(4);
		if (input_choice1==1)
			add_ordered_dishes(ordered_dishes,&*count,no_of_dishes);
		else if (input_choice1==2)
			delete_ordered_dishes(ordered_dishes,&*count,no_of_dishes);
		else if (input_choice1==3)
			amend_ordered_dishes(ordered_dishes,*count,no_of_dishes);
		printf("\n Confirm?");
		printf("\n Yes: Press 1\n");
		printf(" No: Press 2\n");
		input_choice=press(2);
	}
	printf(" Please wait for the dishes. If all dishes are served, please press Enter.\n");
	// press Enter to continue
	char c;
	while((c=getch())!=13)
        printf("\n");
}
//calculate total price
double calculate_total(int no_of_dishes,int ordered_dishes[],int sorted_prices[],double discount,int VIP_price){
	int i;
	double total=0.0;
	for (i=0;i<no_of_dishes;i++)
			if(ordered_dishes[i]!=0)
				total=total+ordered_dishes[i]*sorted_prices[i];
	total=discount*total*1.1+VIP_price; // 10% service charge
	printf(" Thank You! \n The total price (including 10%% service charge) is $%.1f\n",total);// output the total price to customers
	printf(" Please wait for our waiters for checking the bill.\n");
	return total;
}
// generate the current time
void generate_time(char t[]){
	// To pass the current time to string t
	time_t current;
	time(&current);
	strcpy(t,ctime(&current));
}
//write the orders
void write_into_file(char filename[],int table_no,int no_of_dishes,int ordered_dishes[],char sorted_dishes[][54],int sorted_prices[],int VIP_price,double discount,double total_price, char t[]){
	int i;
	FILE *fp;
	fp=fopen(filename,"w+");// file is opened for read and overwritten
	/* To write the ordered dishes, numbers of ordered dishes, prices of dishes, total price, time for orderring,
	   VIP application fee (if any), VIP discount (if any) and table number to the file "filename"*/
	fprintf(fp,"   Table %d\n",table_no);
	fprintf(fp," %s\n",t);
	for (i=0;i<no_of_dishes;i++)
		if(ordered_dishes[i]!=0)
			fprintf(fp," %-50s%5d%6d.0\n",sorted_dishes[i],ordered_dishes[i],ordered_dishes[i]*sorted_prices[i]);
	fprintf(fp,"---------------------------------------------------------------------\n");
	if(VIP_price!=0)
		fprintf(fp," VIP apply%54.d%\n",VIP_price);
	if(discount!=1)
		fprintf(fp," VIP discount%50.f%%\n",(1.0-discount)*100);
	fprintf(fp," Total (including 10%% service charge)%27.1f\n",total_price);
	fclose(fp);
}
//signals to waiters (for customers and waiters)
void signal_to_waiters(int table_no, double total_price, char t[]){
	/* To give signal to waiters when the customers want to be checked the bill*/
	FILE *fp;
	fp=fopen("signal.txt","a");// file is opened for appended
	// the waiters have to know the table number, the total cost and the order time 
	fprintf(fp,"Table %d: $%.1f\nTime: %s",table_no,total_price,t);// t for time
	fprintf(fp,"--------------------------\n");// written after each order
	fclose(fp);
}
//User(for owner)
void owner(char password[],char dishes[][54],int prices[],int no_of_dishes){
	system("cls");
	system("color 0E");
	printf("\n Restaurant Owner");
	int input_choice=0;
	// Give 5 choices for the owner to choose
	printf("\n Change password (owner):\t\tPress 1.");
	printf("\n Change password (waiters):\t\tPress 2.");
	printf("\n Check recent orders:\t\t\tPress 3.");
	printf("\n Change or display records:\t\tPress 4.");
	printf("\n Change VIP password:\t\t\tPress 5.");
	printf("\n Read the comments:\t\t\tPress 6.");
	input_choice=press(6);
    if(input_choice==1)
    	change_password(password,1);
    if(input_choice==2)
    	change_password(password,2);	
    if(input_choice==3)
    	recent_orders();
	if(input_choice==4)
		change_or_display_record(dishes,prices,no_of_dishes);
	if(input_choice==5)
		change_password(password,3);
	if(input_choice==6)
		read_comments();
	printf("\n Do you want to exit the system?\n Yes:\tPress 1.\n No:\tPress 2.\n");
	input_choice=press(2);
    if(input_choice==2){
    	printf("\n");
    	owner(password,dishes,prices,no_of_dishes);// recursive function
	}
}
//User(for waiters)
void waiters(char password[],char dishes[][54],int prices[],int no_of_dishes){
	int input_choice=0,check=1,i,j,found;
	while(check==1){
		system("cls");
		system("color 0A");
		printf("\n Waiters:");
		// The only work for waiters is to check the bills until all are checked
		printf("\n Please go to the corresponding table to check the bills.\n");
		FILE *fp;
		char str[70][50];
		char c;
		/* To read the orders from file and output the first order.
		   Then, overwrite the orders which have not been checked the bills to the file.
		   After checking 1 bill, continue checking the others until no bills are needed to be checked.
		   The orders and bills are sorted in time (the earliest bill is the first to be checked)
		   If there is no any bill, output "No order now!" to remind the waiters.*/
		i=j=found=0;
		fp=fopen("signal.txt","r");// file is opened for overwritten
		while(fgets(str[i],40,fp)!=NULL)
			i++;
		printf("\n");
		while(j<i&&!found){
			if(strcmp(str[j],"--------------------------\n")==0){
				printf("\n %s %s\n",str[j-2],str[j-1]);
				found=1;
			}
			j++;
		}
		if(j<=1){
			printf("\n No order now!\n");
			printf("\n Please press 0 if you want to exit the program\n");
			printf("\n Please press Enter if you recheck whether there is am order now \n");
			while((c=getch())!=13&&c!=48)
        		printf("\n");
			if(c==48)
				check=0;
			else
				check=1;
		}
		else{
			printf("\n Please press Enter if you complete checking the bill\n");
			while((c=getch())!=13)// Press Enter to continue
        		printf("\n");
        	fclose(fp);
        	fp=fopen("signal.txt","w");
			for(j=3;j<i;j+=3)
				fprintf(fp,"%s%s%s",str[j],str[j+1],str[j+2]);// overwritten the bills which are not checked
			fclose(fp);
		}
	}
	printf("\n Do you want to exit the system?\n Yes:\tPress 1.\n No:\tPress 2.\n");
	input_choice=press(2);
    if(input_choice==2){
    	printf("\n");
    	waiters(password,dishes,prices,no_of_dishes);// recursive function
    }
}
//User(for customers)
void customers(char dishes[][54],int prices[],int no_of_dishes){
	system("cls");
	system("color 0C");
	printf("\n Customers");
	FILE *fp,*fpr;
	int input_choice=0,correct,i,VIP_no=0;//VIP_no is 0 for non-VIP
	int VIP_apply,VIP_price=0;//VIP_apply: Each $100 for 5% (at most 25%)
	double discount=1.0;// no discount: 1.0
	char password[10], VIP_number[4];
	fpr=fopen("VIP_password.txt","r");// file is opened for read
	fscanf(fpr,"%s",password);
	printf("\n Welcome!!\n");
	printf(" Are you the VIP?\n Yes: Press 1.\n No: Press 2.");
	input_choice=press(2);
	if(input_choice==1){
		correct=log_in(3);
		if (correct==1)
			discount=VIP(VIP_number);// check the discount
		printf(" Please be reminded that the password is %s\n\n",password);// remind the VIP about the password
	}
	if(input_choice==2){
		// If the customer is not the VIP, he/she can apply one
		printf(" Do you want to be the VIP?\n Yes:\tPress 1.\n No:\tPress 2.\n");
		input_choice=press(2);
		if(input_choice==1){
			printf(" Each $100 for 5%% discount (at most 25%).\n");
			printf(" Input the discount you want:");
			scanf("%d",&VIP_apply);
			// variable VIP_apply is the positive integer not larger than 25 and it is the multiple of 5
			while(VIP_apply%5!=0||VIP_apply<=0||VIP_apply>25){// validation check
				printf(" Wrong Input! Input again: ");
				scanf("%d",&VIP_apply);
			}
			VIP_price=VIP_apply*20;// calculate the price of the aplication for VIP
			discount=1.0-VIP_apply*0.01;// calculate the discount of the VIP
			VIP_no=add_VIP(VIP_apply);// Generate a new VIP and save the data into the file
			if(VIP_no<10)// output the generated VIP number
				printf(" Your VIP no is 00%d\n",VIP_no);
			else
				printf(" Your VIP no is 0%d\n",VIP_no);
			printf(" Your password for VIP is %s\n\n",password);// give the password to the new VIP
		}
	}
	read(dishes,prices,&no_of_dishes);
	char filename[18],temp[2];
	int table_no;
	printf("\n Input Table no (1-20): ");
	scanf("%d",&table_no); // Input the table number eith range 1 to 20
	while(table_no<1||table_no>20){// validation check
		printf(" Wrong Input. Input again (1-20): ");
		scanf("%d",&table_no);
	}
	/* To generate a filename which is used to save the orders of the customers*/
	strcpy(filename,"order(table");
	itoa(table_no,temp,10);
	if(table_no<10){
		filename[11]='0';
		strcpy(&filename[12],temp);
	}
	else
		strcpy(&filename[11],temp);
	strcpy(&filename[13],").txt");
	char sorted_dishes[no_of_dishes][54];
	int sorted_prices[no_of_dishes];
	int search=2, search1[no_of_dishes];
	int ordered_dishes[no_of_dishes],input,count=0,input_no;
	// copy prices[] and dishes[][54] into sorted_prices[] and sorted_dishes[][54] respectively
	//initialize the number of ordered dishes
	for(i=0;i<no_of_dishes;i++){
		sorted_prices[i]=prices[i];
		strcpy(sorted_dishes[i],dishes[i]);
		search1[i]=0;
		ordered_dishes[i]=0;
	}
	printf("\n Thank for using this system.");
	ordering(no_of_dishes,prices,dishes,search1,input_no,ordered_dishes,sorted_prices,sorted_dishes,&count);
    printf(" Do you need to add some dishes now?\n Yes: press 1.\n No: press 2.\n");
    input_choice=press(2);
    while(input_choice==1){
    	int new_ordered_dishes[no_of_dishes];
    	for(i=0;i<no_of_dishes;i++)
			new_ordered_dishes[i]=0;
    	ordering(no_of_dishes,prices,dishes,search1,input_no,new_ordered_dishes,sorted_prices,sorted_dishes,&count);
    	for(i=0;i<no_of_dishes;i++)
			ordered_dishes[i]=ordered_dishes[i]+new_ordered_dishes[i];
		printf(" Do you need to add some dishes now?\n Yes: press 1.\n No: press 2.\n");
    	input_choice=press(2);
	}
	double total_price=0.0;
	// Calculate the total price
	total_price=calculate_total(no_of_dishes,ordered_dishes,sorted_prices,discount,VIP_price);
	// Generate time
	char t[100],c;
	generate_time(t);
	// write the necessary items into the file 
	write_into_file(filename,table_no,no_of_dishes,ordered_dishes,sorted_dishes,sorted_prices,VIP_price,discount,total_price,t);
	// give signal to waiters
	signal_to_waiters(table_no,total_price,t);
	fp=fopen(filename,"r");// file is opened for read
	//copy the contents of file "filename" and append it into the files "orders.txt" for checking by the owner
	fpr=fopen("orders.txt","a");// file is opened for appended
	while((c=fgetc(fp))!=EOF)
		fputc(c,fpr);
	fprintf(fpr,"\n\n\n");
	fclose(fpr);
	fclose(fp);
	//copy the detail of the order and append it into the files "statistics.csv" for checking by the owner
	fpr=fopen("statistics.csv","a");
	if(discount==1.0)
		fprintf(fpr,"%d,%.1f,Nil,Nil,%s",table_no,total_price,t);
	else
		fprintf(fpr,"%d,%.1f,%d,%.0f%%,%s",table_no,total_price,VIP_number,(1-discount)*100.0,t);
	fclose(fpr);
	// Customers leave their comments
	printf(" Do you have any comments?\n Yes:\tPress 1.\n No:\tPress 2.\n");
	input_choice=press(2);
	if(input_choice==1){// Have comments to leave
		fp=fopen("comment.txt","a+");//file is opened for appended and read
		printf(" Please leave some comments (Input ; when you complete): \n");
		fputs("\nComment:\n",fp);
		while((c=getchar())!=';')// Stop inputting the comments when user input ';'
			fputc(c,fp);
		fclose(fp);
	}
	int r;
	r=remove(filename);//remove the file
	printf("\n Do you want to exit the system?\n Yes:\tPress 1.\n No:\tPress 2.\n");
	input_choice=press(2);
    if(input_choice==2){
    	printf("\n");
    	customers(dishes,prices,no_of_dishes);// recursive function
    }
}
//start the program
void start(char dishes[][54],int prices[],int no_of_dishes){
	system("color 0B");
	FILE *fp;
    system("cls");
    int input_choice=0, wrong=0;
    char password[10], password_input[10];
    printf("\n Welcome! You are using the food ordering system in our restaurant.");
    printf("\n Who are you?");
    printf("\n Restaurant owner:\tPress 1.");
    printf("\n Waiters:\t\tPress 2.");
    printf("\n Customers:\t\tPress 3.\n");
    input_choice=press(3);
    if (input_choice==1){
    	int correct=0;
    	correct=log_in(1);// log-in for owner
    	if(correct==1)
			owner(password,dishes,prices,no_of_dishes);// owner (function)
    }
    else if (input_choice==2){
		int correct=0;
			correct=log_in(2);// log-in for waiters
    	if(correct==1)
			waiters(password,dishes,prices,no_of_dishes);// waiters (function)
	}
	else
		customers(dishes,prices,no_of_dishes);// customers (function)
    printf("\n Thank you for using our food ordering system!!\n");
}
//introduction
void introduction(){
	FILE *fp;
	int input;
	fp=fopen("introduction.txt","r");// file is opened for read
	system("cls");
	system("COLOR 0D");
	char c;
	printf(" ");
	do{
		c=fgetc(fp);
		if(c!=EOF)
			printf("%c",c);// output the introduction
		if(c=='\n')
			printf(" ");
	}while(c!=EOF);
	printf("\n");
	printf(" Do you want to read the map?\n Yes: Press 1.\n No: Press 2.");
	input=press(2);
	if(input==1)
		system("Map.url");
}

//main function
int main(){
	char dishes[70][54];
	int prices[70];
	int no_of_dishes=0,input_choice=0;
	char c;
	// windows color (background and font)
	/*0 = Black, 1 = Blue, 2 = Green, 3 = Aqua, 4 = Red, 5 = Purple, 6 = Yellow, 7 = White, 8 = Grey, 9 = Light Blue, 
	  A = Light Green, B = Light Aqua, C = Light Red, D = Light Purple, E = Light Yellow, F = Bright White*/
	system("COLOR 0F");
	printf("\n JackHung's Japanese Restaurant JJR - Food Ordering System\n\n");
    printf("\n Introduction of our restaurant\tPress 1.");
    printf("\n Using the system\t\tPress 2.\n");
    input_choice=press(2);
    if (input_choice==1){
    	introduction();// introduction (function)
    	printf(" Press Enter to continue.\n");
    	while((c=getch())!=13)
        	printf("\n");
    	system("COLOR 0F");
    	start(dishes,prices,no_of_dishes);//start the program
    }
	else
		start(dishes,prices,no_of_dishes);//start the program
    system("pause");
    return 0;
}
