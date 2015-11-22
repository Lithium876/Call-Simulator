/*
Class Day & time: Wednesdays 3 - 5pm
ID    : 1401375 & 1403821
Authors: Lomar Lilly & Gavin Buckley 
Coruse: Programming 2
*/

//--- Header Files ----
#include <stdio.h>          
#include <stdlib.h>			
#include <conio.h>
#include <string.h>
#include <Windows.h>
#include <time.h>
//---------------------

#define MIN 5000000		//Set minium value in range
#define MAX 5009999		//Set maxium value in range

int N;					//Gobal variable to hold current position in the SuspectInfo Structure array
int check,simmax;				//Global variable to check if suspects were added to the suspect file
FILE *CallStats,*Suspectphone,*CallLog; //Declare file pointers

//--- Declare Structures---
struct SuspectInfo
{
	int casenum;
	char Fname[25];
	char Lname[25];
	char telephone[8];
}Suspect[MAX];		           //Global Structure Array
struct BinarySuspect
{
	int casenum;
	char Fname[25];
	char Lname[25];
	char telephone[8];
	int Callin;
	int Callout;
	int minutesin;
	int minutesout;
}suspect={0,"","","",0,0,0,0}; //Global Structure Array with initailized values
//------------------------------------------------

//--- Function Proto-types-----
void login();				//Login Function username: Admin password: P@$$w0rd (case sensitive)
int displaymenu();		    //Display the menu
void menu();				//Calls the respective function based on the option selected in the menu displayed
void initialize();			//Create Files as well as creating blank records in the binary created
void add();					//Allows the user to add suspect information into a structure array and write them to a file
void simulate();			//Takes two random numbers from the structure array to create a call log and write them to a file
void display();				//Displays the call log from the file
void displaystats();		//Display call statistics from file
//---------------------------------------------------------------------------------------------------

//------------------------- MAIN FUNCTION ------------------------
int main()
{
    int choice;
    login();                //Calling the login function
    choice=displaymenu();   //Calling the display menu function and storing the return valued the the variable "choice"
    menu(choice);			//Calling the "choice" option function from the menu function
	getch();				//Holds the information on the screen until a key is pressed
    return 0;
}
//--------------------------------------------------------------------------------------------------

//---------------------- DISPLAY STATISTICS FUNCTION --------------------------
void displaystats()
{
	char telephone[8];											//Dummpy variable to hold number to serach for
	int i=0;
	CallStats=fopen("Call Statistics File.bin","r+b");			//Opens CallStats in read binary mode
	if(CallStats!=NULL)											//Checks if Callstats is Null
	{
		printf("Enter suspect telephone number (xxx-xxxx)\n");
		scanf("%s",telephone);
        while(!feof(CallStats))									//Loops until the end of the file CallStats is reached
         {
            fread(&suspect,sizeof(suspect),1,CallStats);		//Reading from binary file
            if(strcmp(suspect.telephone,telephone)==0)			//Searches Binary file for the number stored in the bummy variable
            {
			   i++;												//Increments if a match was found
			   /*PRINT OUT*/
			   system("cls");
			   puts("\n \t \t  |=======================================|");
			   puts("\t \t  |\t        CALL STATISTICS     \t  |");
			   puts("\t \t--|=======================================|--\n");
			   printf("\tCase Number\n");
			   printf("\t     %d   \t  %s   %s   %s\n\n", suspect.casenum, suspect.Fname, suspect.Lname,suspect.telephone);
			   printf("\t\t\tTotal Incomming Calls :- %d\n\t\t\tTotal Outgoing Calls  :- %d\n\t\t    Total Incomming Call Minutes :- %d\n\t\t    Total Outgoing Call Minutes  :- %d\n\n",suspect.Callin,suspect.Callout,suspect.minutesin,suspect.minutesout);
			   break;											//End Loops
			}
        }
		if(i<=0) printf("\n\a%s was not found, please check your format (xxx-xxxx)\n\n",telephone);
	}
	else
		 printf("Error");
	fclose(CallStats);											 //Close CallStats file
	system("pause");											 //Holds display on screen until the presses a key 
	system("cls");												 //Clears Screen
	fflush(stdin);											     //Clears buffer
	menu(displaymenu());                                         //Calls the menu function using the return value from the displaymenu function as arugment
}
//-----------------------------------------------------------------------------------------

//------------------------ DISPLAY FUNTION ------------------------------
void display()
{
	const size_t line_size = 300;                          //Maximum number of characters to be read (including the final null-character)
	char* line = (char*) malloc(line_size);                //Dynamically allocate space to the variable "line"
	char op;
	int i=0;
	CallLog=fopen("Call log file.txt","r");				   //Opening CallLog file in read mode 
	if(CallLog == NULL)
    {
      printf("Error while opening the file.\n");
      exit(0);											  //Terminates program
    }
	puts("\n \t \t  |=======================================|");
	puts("\t \t  |\t       CALL LOG DISPLAY     \t  |");
	puts("\t \t--|=======================================|--");
	while (fgets(line, line_size, CallLog) != NULL)       //Reads a line from Callog and stores it into line
	{
		printf(line);									  //Displays line from file
		i++;											  //i=i+1
		if(i==20)										  //Checks if 20 lines were read from the CallLog file
		{
			i=0;										  //Initialzing the number of lines printed from file
			printf("\nPress ANY key to display more");
			getch();									  //Holds display on screen until the presses a key
			system("cls");                                //Clears Screen
			puts("\n \t \t  |=======================================|");
			puts("\t \t  |\t       CALL LOG DISPLAY     \t  |");
			puts("\t \t--|=======================================|--");
		}
	}
	free(line);                                           //Deallocates the memory previously allocated by malloc
	printf("\n\t\t\t\tEnd of File\n\n");
	system("pause");                                      //Holds display on screen until the presses a key 
	system("cls");                                        //Clears Screen
	fclose(CallLog);                                      //Close CallLog file
	menu(displaymenu());                                  //Calls the menu function using the return value from the displaymenu function as arugment
}
//-------------------------------------------------------------------------------------------

//---------------------------- SIMULATE FUCTION -------------------------------
void simulate()
{
	char *day[]={"MON","TUE","WED","THUR","FRI","SAT","SUN"};
	char *month[]={"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
	char *call[]={"Incomming Call","Outgoing Call"};
	int simnum,i,caseno;
	long int currpos;											    //Variable to hold the value of current position in CallStats file
	CallLog=fopen("Call log file.txt","a+");						//Opens CallLog file
	printf("\t\tHow many call simulations would\n\t\tyou like to make? (Max: %d) ",simmax*2);
	scanf("%d",&simnum);
	/*Ensures that the user can only generate the MAX amount of simulation at a time*/
	if(simnum>(simmax*2))     //Simmax is calulated by the number of records in suspect file times 2
	{
		printf("\t\tYou can't simnulate %d call(s) as the max \n\t\tcall you can simualte is %d\n",simnum,simmax*2);
		system("pause");
		system("cls");
		puts("\n \t \t  |=======================================|");
		puts("\t \t  |\t        CALL SIMULATOR     \t  |");
		puts("\t \t--|=======================================|--");
		simulate();
	}
	//----------------------------------------------------------------------------------
	srand(time(NULL));
	for(i=0;i<simnum;i++)
	{
		int in=rand()%(N-MIN)+MIN, out=rand()%(N-MIN)+MIN;             //Generates random array postions
		int year=2000+rand()%16, date=rand()%(31-1)+1;                 //Generates random years (2000 - 2015) & days (1 - 31)
		int callminutes=(rand()%60)+1;								   //Generates call minutes assuming that calls don't exceed an hour 
		int hours=rand()%25, minutes=rand()%60;						   //Genterates time in 24 hour format
		int callstat=rand()%2;										   //Generates Incomming or Outgoing Call
		/*Ensures that two same numbers are not taken from array*/
		if(in==out)													   
		{
			do{
				srand(time(NULL));
				in=rand()%(N-MIN)+MIN;
				out=rand()%(N-MIN)+MIN;
			}while(in==out);
		}
		//------------------------------------------------------------
		printf("\n\t%s from: %s to %s\n",call[callstat],Suspect[in].telephone,Suspect[out].telephone);
		//Writes to CallLog text file
		fprintf(CallLog,"\n\t%s from: %s to %s\n",call[callstat],Suspect[in].telephone,Suspect[out].telephone);
		/*----------------------------------- Time Print Out Format -----------------------------------  */
		if(hours<10)
		{
			if(minutes<10)
			{
				printf("\tCall Date & Time: %s, %s, %d, %d @ 0%d:0%d",day[rand()%7],month[rand()%12],date,year,hours,minutes);
				printf("\n\tCall Duriation:   %d Minutes\n",callminutes);
				//Writes to CallLog text file
				fprintf(CallLog,"\tCall Date & Time: %s, %s, %d, %d @ %d:0%d",day[rand()%7],month[rand()%12],date,year,hours,minutes);
				fprintf(CallLog,"\n\tCall Duriation:   %d Minutes\n",callminutes);
			}
			else
			{
				printf("\tCall Date & Time: %s, %s, %d, %d @ 0%d:%d",day[rand()%7],month[rand()%12],date,year,hours,minutes);
				printf("\n\tCall Duriation:   %d Minutes\n",callminutes);
				//Writes to CallLog text file
				fprintf(CallLog,"\tCall Date & Time: %s, %s, %d, %d @ 0%d:%d",day[rand()%7],month[rand()%12],date,year,hours,minutes);
				fprintf(CallLog,"\n\tCall Duriation:   %d Minutes\n",callminutes);
			}
		}
		else if(minutes<10)
		{
			printf("\tCall Date & Time: %s, %s, %d, %d @ %d:0%d",day[rand()%7],month[rand()%12],date,year,hours,minutes);
			printf("\n\tCall Duriation:   %d Minutes\n",callminutes);
			//Writes to CallLog text file
			fprintf(CallLog,"\tCall Date & Time: %s, %s, %d, %d @ %d:0%d",day[rand()%7],month[rand()%12],date,year,hours,minutes);
			fprintf(CallLog,"\n\tCall Duriation:   %d Minutes\n",callminutes);
		}
		else
		{
			printf("\tCall Date & Time: %s, %s, %d, %d @ %d:%d",day[rand()%7],month[rand()%12],date,year,hours,minutes);
			printf("\n\tCall Duriation:   %d Minutes\n",callminutes);
			fprintf(CallLog,"\tCall Date & Time: %s, %s, %d, %d @ %d:%d",day[rand()%7],month[rand()%12],date,year,hours,minutes);
			fprintf(CallLog,"\n\tCall Duriation:   %d Minutes\n",callminutes);
		}
		//------------------------------------------------------------------------------------------------------------------------------------
		CallStats=fopen("Call Statistics File.bin","r+b");          //Open Callstats in binary read and write mode
		while(!feof(CallStats))                                     //Loops code while not at the end of the CallStats file
		{
			fread(&suspect,sizeof(suspect),1,CallStats);            //Reads data from the CallStats
			if(strcmp(Suspect[in].telephone,suspect.telephone)==0)  //Searches the CallStats file for simulated number
			{

				currpos=ftell(CallStats);                            //Returns the current file position in the CallStats file
				suspect.Callout=suspect.Callout+1;                   //Checks the amount of outgoing calls
				suspect.minutesout=suspect.minutesout+callminutes;   //Updates the minutes in field in suspect structure by adding callminutes
				fseek(CallStats,currpos-sizeof (suspect),SEEK_SET);  //Positions cursor for writing
				fwrite(&suspect, sizeof(suspect), 1, CallStats);     //Writes data from suspect to CallStats
				fflush(CallStats);
				rewind(CallStats);
				break;                                               //Ends loop
			}
		}
		fclose(CallStats);                                           //Close CallStats file
		CallStats=fopen("Call Statistics File.bin","r+b");           //Open Callstats in binary read and write mode
		while(!feof(CallStats))                                      //Loops code while not at the end of the CallStats file
		{
			fread(&suspect,sizeof(suspect),1,CallStats);             //Reads data from the CallStats
			if(strcmp(Suspect[out].telephone,suspect.telephone)==0)  //Searches the CallStats file for simulated number
				{
					currpos=ftell(CallStats);                        //Returns the current file position in the CallStats file
					suspect.Callin=suspect.Callin+1;                 //Checks the amount of incomming calls
					suspect.minutesin=suspect.minutesin+callminutes; //Updates the minutes in field in suspect structure by adding callminutes
					fseek(CallStats,currpos-sizeof (suspect),SEEK_SET);//Positions cursor for writing
					fwrite(&suspect, sizeof(suspect), 1, CallStats); //Writes data from suspect to CallStats
					fflush(CallStats);
					rewind(CallStats);
					break;                              //Ends loop
				}
		}
		fclose(CallStats);                              //Close CallStats file
	}
	fclose(CallLog);                                    //Close CallLog file
	system("pause");                                    //Holds display on screen until the presses a key
	system("cls");										//Clears Screen
	menu(displaymenu());                                //Calls the menu function using the return value from the displaymenu function as arugment
}
//-------------------------------------------------------------------------------

//-------------------- ADD FUNCTION ---------------------------
void add()
{
	char op, num;					
	static int n=0;													//Keeps track of the total number of records in suspect text file
	static int i=MIN-1;												//Keeps track of the array's current postion making sure data isn't over written
	int x;
	i++;
	Suspectphone=fopen("Suspect phone number file.txt","a+");		//Open Suspectphone file in append mode
	CallStats=fopen("Call Statistics File.bin","ab");				//Opens Call Statistic file in binary append mode
	for (i;i<=MAX;i++)												//Makes Sure the user does not exceed the MAX range (5009999)
	{
		char num3[4],num4[5]="";									//Dummy variables to validate telephone number input
		while(1)													//Loops code forever
		{
			printf("\n\t\t\t    Enter Case Number: ");
			if(scanf("%d", &Suspect[i].casenum)!=1)					//Checks if the value entered is an integer value
			{
				fflush(stdin);										//Clears buffer
				printf("\a\n\t\t\t\t   ERROR!!\n\t\t That input was not a number value, try again.\n ");
			}
			else if(Suspect[i].casenum<=0)							//Tests to see if the case number is less than or equal to 0
			{
				fflush(stdin);										//Clears buffer
				printf("\a\n\t\t\t\t   ERROR!!\n\t\t   That input was less than 1, try again.\n");
			}
			else
			{
				for(x=MIN;x<i;x++)									//Makes sure x is less than the current array posstion
				{
					if(Suspect[x].casenum==Suspect[i].casenum)		//Checks structure array if case number entered already exists
					{
						printf("\a\n\t\t\t\t    ERROR!!\t\t\n\t\t\t Case Number: %d already exists \n\t   Re-Enter previous record with a different case number\n",Suspect[i].casenum);
						fclose(Suspectphone);						//Closes the suspectphone file
						fclose(CallStats);							//Closes the suspectphone file
						i--;										//move the current array position back one place to rewrite record
						add();										//Recursion at its best ^_^ ... calls the add function
					}
				}
				system("cls");										//Clears Screen
								        /*PRINTOUTS*/
				puts("\n \t \t  |=======================================|");
				puts("\t \t  |\t         ADD SUSPECT     \t  |");
				puts("\t \t--|=======================================|--");
				printf("\n\t\t\t    Enter Case Number: %d",Suspect[i].casenum);
				break;												//Tells the complier to exit the loop
			}
		}
		fflush(stdin);												//Clears buffer
		printf("\n\t\tEnter Suspects First and Last name: ");
		scanf("%25s %25s",Suspect[i].Fname,Suspect[i].Lname);		//Accepting strings no longer than 25 characters long
		fflush(stdin);												//Clears buffer
		while(1)													//Loops code forever
		{
            printf("\t\tEnter Telephone Number (xxx-xxxx): ");
            scanf("%[0123456789]-%[0123456789]",num3,num4);		    //Accepting strings of only integer value with an hyphen
			if(strlen(num3)!=3 || strlen(num4)!=4 || (char)num3[0]=='1'|| (char)num3[0]=='0') //Validates the variables entered 
            {
				fflush(stdin);										//Clears buffer
                printf("\a\n\t\t\t   Invalid number format!\n\t\t    Format: xxx-xxxx [7 DIGITS REQUIRED]\n\n");
            }
			else 
			{
				printf("");
				strcpy(Suspect[i].telephone,num3);					//Copies the string value from num3 to Suspect[i].telephone
				strcat(Suspect[i].telephone,"-");					//Adds a hyphen to Suspect[i].telephone
				strcat(Suspect[i].telephone,num4);					//Adds the string value from num4 to Suspect[i].telephone
				for(x=MIN;x<i;x++)									//Makes sure x is less than the current array posstion
				{
					if(strcmp(Suspect[x].telephone,Suspect[i].telephone)==0)  //Checks structure array if telephone number entered already exists
					{
						printf("\a\n\t\t\t\t    Error!\n\t\t    Telephone Number: %s already exists\n\t   Re-Enter previous record with a different telephone number\n",Suspect[i].telephone);
						fclose(Suspectphone);						//Closes the suspectphone file
						fclose(CallStats);							//Closes the suspectphone file
						i--;										//move the current array position back one place to rewrite record
						add();										//Recursion at its best ^_^ ... calls the add function
					}
				}
				fflush(stdin);										//Clears buffer
				system("cls");										//Clears Screen
				                        /*PRINTOUTS*/
				puts("\n \t \t  |=======================================|");
				puts("\t \t  |\t         ADD SUSPECT     \t  |");
				puts("\t \t--|=======================================|--");
				printf("\n\t\t\t    Enter Case Number: %d",Suspect[i].casenum);
				printf("\n\t\tEnter Suspects First and Last name: %s %s\n",Suspect[i].Fname,Suspect[i].Lname);
				printf("\t\tEnter Telephone Number (xxx-xxxx): %s\n",Suspect[i].telephone);
                break;												 //Ends loop
			}
		}
		printf("\n\t\t\t  Case Number: %d\n\t\t\t\tFirst Name: %s \n\t\t\t\tLast Name: %s\n\t\t\t\tTelephone Number: %s\n", Suspect[i].casenum, Suspect[i].Fname, Suspect[i].Lname,Suspect[i].telephone);
		/*Writes Records to text file*/
		fprintf(Suspectphone,"   %d \t\t %s \t\t %s \t\t %s\n", Suspect[i].casenum, Suspect[i].Fname, Suspect[i].Lname,Suspect[i].telephone);
		memcpy(&suspect, &Suspect[i], sizeof suspect);                 //Copies the elements stored in structure "Suspect[i]'s" memory to another structure "suspect"
		fseek(CallStats,sizeof(suspect)*(suspect.casenum-1),SEEK_SET); //Postions the cursor for writing to the binary file
		fwrite(&suspect,sizeof(suspect),1,CallStats);				   //Writes records to the binary file
		printf("\n\t     Press ANY key to Add more records or \'n\' to continue\n\n");
		n++;														   //Increment the amount records in the file by 1
		op=getch();													   //Waits for user input then stores it in the variable "op"
		op=tolower(op);												   //Makes users input in "op" lowercased
		if (op=='n')
		{
			fclose(Suspectphone);									   //Close the Suspectphone file
			fclose(CallStats);										   //Close the CallStats file
			//printf("\nCurrent Position: %d\n",i);					   //Displays the current array index
			printf("\t\t\t   Total Records added: %d\n\n",n);		   //Displays the total number of records added to the text file
			system("pause");										   //Holds display on screen until the presses a key 
			break;													   //Ends loop
		}
	}
	N=i;																//Updates the global varible "N" to the current array postion
	check=n;															//Updates the global varible "Check" to the total number of records added to the text file
	simmax=n;															//Updates the global varible "simmax" to the total number of records added to the text file
	system("cls");														//Clear Screen
	menu(displaymenu());												//Calls the menu function using the return value from the displaymenu function as arugment
}
//---------------------------------------------------------------------------------------

//---------------------- INITIALIZE FUNCTION ------------------------
void initialize()															
{
	int i,x,n=MIN;	
	for(x=MIN;x<=N;x++)									//Makes sure x is less than or equal to the current array posstion
	{
		Suspect[x].casenum=0;							//Resets structure array case number to 0 if program is not closed but re-initalized
		strcpy(Suspect[x].telephone,"");				//Resets structure array telephone number to 0 if program is not closed but re-initalized
	}
	Suspectphone=fopen("Suspect phone number file.txt","w+");								//Opens the Suspectphone file in read write mode
	fprintf(Suspectphone,"Case Number \t First Name \t Last Name \t Telephone Number\n");   //Writes strings in " " to file
	fclose(Suspectphone);																    //Closes the Suspectphone file
	
	CallStats=fopen("Call Statistics File.bin","wb");										//Open CallStats file in write binary mode
	if (CallStats!=NULL)                                                                    //Check if the CallStats file is not NULL
	{
		for(i=MIN;i<MAX;i++)
		{
			fwrite(&suspect,sizeof(suspect),1,CallStats);									//Writes 9999 blank records to the CallStats file
		}
		printf("\t\t\tFiles Initialized Successfully");
		printf("\n\t\t   File created with %d blank records.\n\n",i);
		fclose(CallStats);																    //Closes the CallStats file
	}
	else                                                                                    //If the CallStats file is NULL execute the follow block
	{
		printf("Couldn't open file");
		fclose(CallStats);                                                                  //Closes the CallStats file
	}
    CallLog=fopen("Call log file.txt","w+");												//Opens the CallLog file in write and read mode
	fclose(CallLog);																		//Closes the CallLog file
}
//---------------------------------------------------------------------------------------

//---------------------- MENU FUNCTION ---------------------------
void menu(int x)									//Function accepts an integer value and returns nothing
{
    switch(x)										
    {
        case 1:										//Executes the following block of code if the integer value x is 1
        {
			initialize();							//initialize function called to create and initialize files
			system("pause");						//Holds output on the screen until the user presses a key
			system("cls");							//Clears the screen
            menu(displaymenu());					//Calls the menu function using the return value from the displaymenu function as arugment
            break;									//Ends this instance of the program
        }
        case 2:										//Executes the following block of code if the integer value x is 2
        {
			system("cls");							//Clears the screen
			puts("\n \t \t  |=======================================|");
			puts("\t \t  |\t         ADD SUSPECT     \t  |");
			puts("\t \t--|=======================================|--");
			add();									//add function called to add records to the suspect phone file
            break;									//Ends this instance of the program
        }
        case 3:										//Executes the following block of code if the integer value x is 3
        {
			if(check<2)								//Executes the following block if less than two records are in the phone suspect file
			{
				printf("\t\aYou can't simualte any call with less than two suspect number\n\n");
				system("pause");					//Holds output on the screen until the user presses a key
				system("cls");						//Clears the screen
				menu(displaymenu());				//Calls the menu function using the return value from the displaymenu function as arugment
			}
			else                                    //Executes the following block if two or more records are in the phone suspect file
			{
				system("cls");					    //Clears the screen
				puts("\n \t \t  |=======================================|");
				puts("\t \t  |\t        CALL SIMULATOR     \t  |");
				puts("\t \t--|=======================================|--");
				simulate();						    //Simulate function called to simulate phone calls and populate the call log file
				break;							    //Ends this instance of the program
			}
        }
        case 4:                                     //Executes the following block of code if the integer value x is 4
        {
			if(check<2)								//Executes the following block if less than two records are in the phone suspect file
			{
				printf("\t\t\t\t\aFile is Empty\n\n");
				system("pause");					//Holds output on the screen until the user presses a key
				system("cls");						//Clears screen
				menu(displaymenu());			    //Calls the menu function using the return value from the displaymenu function as arugment
			}
			else                                    //Executes the following block if two or more records are in the phone suspect file
			{
				system("cls");			            //Clears screen	
				display();                          //Calls display function to display records from the call logs file
				break;                              //Ends this instance of the program
			}
        }
        case 5:                                     //Executes the following block of code if the integer value x is 5
        {
			if(check<1)								//Executes the following block if less than one record are in the phone suspect file
			{
				printf("\a\t\t\tNo Number in file to search for\n\n");
				system("pause");					//Holds output on the screen until the user presses a key
				system("cls");						//Clears screen
				menu(displaymenu());				//Calls the menu function using the return value from the displaymenu function as arugment
			}
			else                                    //Executes the following block if one or more records are in the phone suspect file
			{
				displaystats();						//Calls the displaystats funciton to display call stats of a specific number in the phone file
				break;								//Ends this instance of the program
			}
        }
        case 6:                                     //Executes the following block of code if the integer value x is 6
        {
			printf("\t\t\t\tLogin Out...\n");       //Message to user
			Sleep(2000);							//Pause program for 2 seconds
            exit(0);								//Close program
            break;									//Ends this instance of the program
        }
        default:                                    //Executes the following block of code if the integer value x is not any of the instances above
            {
                printf("\a \t \t|    Invaild Option Selection, Try Again    |\n");
                puts("\t \t|-------------------------------------------|");
                Sleep(1000);                         //Pause program for 1 second
                system("cls");						 //Clears Screen
                menu(displaymenu());				 //Calls the menu function using the return value from the displaymenu function as arugment
                break;								 //Ends this instance of the program
            }
    }
}
//-------------------------------------------------------------------------------------

//---------------- DISPLAY MENU FUNCTION -----------------
int displaymenu()
{
    int option;
    puts("\n \t \t  |=======================================|");
    puts("\t \t  | \t           MAIN MENU \t \t  |");
    puts("\t \t--|=======================================|--");
    printf("\t \t|  Select the number that corresponds with  |\n \t \t|\t \t your option.\t \t    |\n");
    puts("\t \t|-------------------------------------------|");
    printf("\t \t|\t  1. Initialize System \t \t    |\n");
    printf("\t \t|\t  2. Add Suspects Phone Number      |\n");
    printf("\t \t|\t  3. Simulate Call \t \t    |\n");
    printf("\t \t|\t  4. Display Call Log \t \t    |\n");
    printf("\t \t|\t  5. Display Call Statistics \t    |\n");
    printf("\t \t|\t  6. Exit\t \t \t    |\n");
    puts("\t \t|-------------------------------------------|");
    printf("\t \t| Option: ");
    scanf("%d",&option);
    puts("\t \t|-------------------------------------------|");
    return option;
}
//--------------------------------------------------------------------------------------

//------------------- LOGIN FUNCTION -----------------------------
void login()
{
    int i;									     //Variable Declaration 
    char uid[25],pwd[25],s_uid[25]={"Admin"};				  
    char s_pwd[25]={"p@$$w0rd"},ch=' ';						
    system("color 1E");                         //Set porgram color to blue and yellow 
	puts("\n\n\n\n\n\t\t\t\t     WELCOME");
	puts("\n\t\t\t   JAMEKCA MAYJOR CRIMES UNIT");
    puts("\n\t \t \t |-----------------------------|");
    puts("\t \t \t |             LOGIN           |");
    puts("\t \t \t |-----------------------------|");
    printf(" \t \t \t |Enter the User ID : ");
    scanf("%s",uid);
    printf(" \t \t \t |Enter the Password : ");
    i=0;
    while(1)									//loops code block forever
    {
        ch=getch();
        if(ch==13)							    //13 is the ASCII value for the "Enter" key
            break;								//Exit loop
        else if(ch==8)							//8 is the ASCII value for the "backspace" key
        {       if(i!=0)                        //This is for avoiding the input from being deleted 
            {
                printf("\b");				    //'\b' represents blackspace escape character
                printf("%c",32);                //32 is the ASCII value for space
                printf("\b");					//'\b' represents blackspace escape character
                i--;
                pwd[i]='\0';
            }
            else
                continue;
        }
        else
        {
            putchar('*');                       //Replaces each character type by a *
            pwd[i]=ch;
            i++;
        }
    }
    puts("\n \t \t \t |-----------------------------|");
    pwd[i]='\0';
    if((strcmp(uid,s_uid))==0 && (strcmp(pwd,s_pwd))==0)
    {
        printf("\t \t \t |\t USER AUTHENTICATED    |");
        puts("\n \t \t \t |-----------------------------|");
        Sleep(1000);
        system("cls");
    }
    else
    {
        printf("\t \a \t \t |UNABLE TO AUTHENTICATE USER, |\n \t \t \t | \t    Try again\t       |");
        puts("\n \t \t \t |-----------------------------|");
        Sleep(2000);
        system("cls");
		fflush(stdin);
        login();
    }
}
//-----------------------------------------------------------------------------------------