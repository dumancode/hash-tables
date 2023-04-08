//FURKAN DUMAN
//2453173

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Assumptions
 -> All printf() functions O(1)
 -> I predict all string functions have a O(1)*/


struct Mail
{
    int Email_ID;
    char Sender[51];
    char Receipt[51];
    int Day_of_the_month;
    int countWords;


};

struct Mail*  readMails(char *, int,int, int*);
void InsertingHashTableWithLinearProbing(struct Mail *, struct Mail, int,int );
void InsertingHashTableWithDoubleProbing(struct Mail*, struct Mail, int, int, int );
double newLoadFactorFunction(int);
void print_table(struct Mail *,int);
int hashFunction(int,int);
int hash2_Function(int);
void search(struct Mail *, int,int, char*, int );
void menu();



int main()
{

    int choose_for_collision,number;

    char filePath[50];
    printf("Enter data path: ");
    scanf("%s",filePath);

    fflush(stdin);

    printf("How many files: ");
    scanf("%d",&number);

    printf("*********************\n");
    printf("Pick whether double hashing or linear probing\n");
    printf("1-) Linear Probing\n");
     printf("2-) Double Hashing\n");
    printf("*********************\n");
    scanf("%d",&choose_for_collision);

    printf("*********************\n");

    int table_Size_forMain = 0; // I create this int for learn hash table size using readMails function by pass by refarence.
    struct Mail*hashTable = readMails(filePath, number,choose_for_collision, &table_Size_forMain);



    int menu_selection  = 0;
    // For do while we don't know O().It depends.
    do
    {
        menu();


        scanf("%d",&menu_selection);


        if(menu_selection == 1)  // for search
        {
            int email_id;
            char * sendername;
            printf("Enter unique identifier: ");
            scanf("%d",&email_id);
            printf("Enter sender:");
            scanf("%s",sendername);

            search(hashTable,table_Size_forMain,email_id,sendername,choose_for_collision);
        }

        else if(menu_selection == 2)  // for print table -> 0(1)
        {
            print_table(hashTable,table_Size_forMain);
        }


        else if( menu_selection == 3) // For Exit  -> O(1)
        {
            printf("Bye :) ");

        }



    }
    while(menu_selection != 3);


    return 0;
}


struct  Mail * readMails(char *filePath,int number,int typeOfReSolve, int * table_Size_forMain)
{
    int table_Size = 11; // We assumed first table_Size = 11.
    struct Mail* hashTable = (struct Mail*)malloc(table_Size * sizeof(struct Mail));
    struct Mail *hashTable2 ;
    int x;

    for(x=0; x<table_Size; x++) //Here, I  give initial values of hashTable.
    {
        hashTable[x].Email_ID =0;
        hashTable[x].countWords=0;
        hashTable[x].Day_of_the_month=0;
        strcpy(hashTable[x].Receipt,"");
        strcpy(hashTable[x].Sender,"");
    }



    char *token;
    char name[50];
    FILE *inFile1;
    char fileName[50];
    char fileNumber[3] = "";
    int i;

    strcat(filePath,"\\");
    for(i=0; i < number; i++)  // O(N)
    {
        struct Mail tempHashTable;

        strcpy(fileName,filePath);
        itoa(i+1, fileNumber, 10);
        strcat(fileName,fileNumber);
        strcat(fileName,".txt");


        inFile1 = fopen(fileName,"r");


        if(i== 0)  // If the data path is written incorrectly, the file will be empty so
            //I can find out that the data path name is misspelled.
        {
            if(inFile1 == NULL)
            {
                printf("There is no datapath !!!\n");
                exit(1);

            }
        }


        if(inFile1 == NULL)  // For more than  arguments

        {
            printf("Much argument error!\n");
            exit(1);
        }

        fscanf(inFile1,"%d\n",&(tempHashTable.Email_ID));  // Read integer value and store this value in my structure.


        fgets(name, 50, inFile1);
        token = strtok(name,":");
        token = strtok(NULL," ");   // I'm reading string values after ':'

        strcpy(tempHashTable.Sender,token);    // Then store in my structure.
        int size = strlen(tempHashTable.Sender); //Total size of string
        tempHashTable.Sender[size-1] = '\0';  // I am reading strings value with the one space, so here I am deleting the space.



        fgets(name, 50, inFile1);
        token = strtok(name,":");
        token = strtok(NULL," ");        // I'm reading string values after ':'

        strcpy(tempHashTable.Receipt,token);  // Then store in my structure.
        size = strlen(tempHashTable.Receipt); //Total size of string
        tempHashTable.Receipt[size-1] = '\0'; // I am reading strings value with the one space, so here I am deleting the space.

        fscanf(inFile1,"%*s %d\n",&tempHashTable.Day_of_the_month );  //skip string and read integer value, then store in my structure.


        int countforwords = 1; //I assumed content has least one word.
        char ch;


        while((ch = fgetc(inFile1)) != EOF)  // O(N)
        {

            if((ch == ' ')) // I read the spaces
            {
                countforwords++;
            }
        }


        tempHashTable.countWords = countforwords;


        int  x = 0 ; int flag = 0;
        for(x = 0; x<table_Size;x++)
        {
            if(hashTable[x].Email_ID == tempHashTable.Email_ID )
            {
                printf("There is a e mail with this ID : %d, it cannot be repeated same ID !!!  \n",tempHashTable.Email_ID);
                flag = 1;

            }

        }
        if(flag) continue;



        int key = ( (tempHashTable.Email_ID)  + (tempHashTable.Sender[0]) -65 ); // key value for hash table.

        //Calling Hash Function
        int whereIsIndex = hashFunction(table_Size,key);
        //Creating loadFactor
        double loadFactor = (double)(i+1)/(double)table_Size;



        // We have two Collision Resolution . First one is  Linear Probing  other one Double Hashing
        //I apply one of these two methods according to the value I get from the user in the Main function.


        if(loadFactor <0.5)
        {

            if(typeOfReSolve == 1)  InsertingHashTableWithLinearProbing(hashTable,tempHashTable,whereIsIndex,table_Size);  // Linear Probing

            if(typeOfReSolve == 2) InsertingHashTableWithDoubleProbing(hashTable,tempHashTable,key,table_Size,whereIsIndex); // Double Hashing


        }

        else
        {
            int y;
            int old_Size = table_Size;


            table_Size = newLoadFactorFunction(table_Size); //Here Since there  is a Collision, it recalculates the size of the table.



            /*HashTable2 has been created temporarily. The purpose is to copy the data from the resulting table..*/
            hashTable2 =  (struct Mail*)malloc(old_Size * sizeof(struct Mail));

            for(x=0; x<old_Size; x++) //Its initial values
            {
                hashTable2[x].Email_ID =0;
                hashTable2[x].countWords=0;
                hashTable2[x].Day_of_the_month=0;
                strcpy(hashTable2[x].Receipt,"");
                strcpy(hashTable2[x].Sender,"");
            }


            for(y = 0; y<old_Size; y++) // Copy values
            {
                hashTable2[y].countWords = hashTable[y].countWords  ;
                hashTable2[y].Email_ID = hashTable[y].Email_ID  ;
                hashTable2[y].Day_of_the_month = hashTable[y].Day_of_the_month  ;
                strcpy(hashTable2[y].Receipt,hashTable[y].Receipt)  ;
                strcpy(hashTable2[y].Sender,hashTable[y].Sender)  ;


            }

            free(hashTable) ; // and delete hashTable

            hashTable =  (struct Mail*)malloc(table_Size * sizeof(struct Mail)); // Then Create again with the new Table Size.


            for(x=0; x<table_Size; x++)  // Initial values
            {
                hashTable[x].Email_ID =0;
                hashTable[x].countWords=0;
                hashTable[x].Day_of_the_month=0;
                strcpy(hashTable[x].Receipt,"");
                strcpy(hashTable[x].Sender,"");
            }





             //Here, I transfer the data in hashTable2 to hashTable according to the resolve technique.
            for(y = 0; y < old_Size ; y++)
            {

                int key = ( (hashTable2[y].Email_ID)  + (hashTable2[y].Sender[0]) -65 );
                int whereIsIndex = hashFunction(table_Size,key);
                if(typeOfReSolve == 1)
                    InsertingHashTableWithLinearProbing(hashTable,hashTable2[y],whereIsIndex,table_Size);
                if(typeOfReSolve == 2)
                    InsertingHashTableWithDoubleProbing(hashTable,hashTable2[y],key,table_Size,whereIsIndex);

            }


               // Here Some mails could not be loaded because the load factor is greater than 0.5. Here I am loading data that cannot be loaded.
            int key = ( (tempHashTable.Email_ID)  + (tempHashTable.Sender[0]) -65 );
            int whereIsIndex = hashFunction(table_Size,key);
            if(typeOfReSolve == 1)
                InsertingHashTableWithLinearProbing(hashTable,tempHashTable,whereIsIndex,table_Size);
            if(typeOfReSolve == 2)
                InsertingHashTableWithDoubleProbing(hashTable,tempHashTable,key,table_Size,whereIsIndex);

        }



        print_table(hashTable,table_Size);
        printf("\n");
        printf("*******************************************************\n");
        printf("\n");


        strcpy(fileName,"");
        fclose(inFile1);

    }


    *table_Size_forMain = table_Size; // table_Size_forMain ->  it will be using in main functions.
    return hashTable;

}


int hashFunction(int size, int key)
{
    return (key % size);
}


//In this function  I got rid of Collision with LinearProbing.
void InsertingHashTableWithLinearProbing(struct Mail *hashTable, struct Mail oldHashTable, int key, int hashTable_Size)

{
    while(1)
    {

        if((hashTable[key].Email_ID) == 0) // If  that row in the HashTable is blank

        {
            hashTable[key].Email_ID = oldHashTable.Email_ID;
            hashTable[key].countWords = oldHashTable.countWords;
            hashTable[key].Day_of_the_month = oldHashTable.Day_of_the_month;       //->  Values are loaded
            strcpy(hashTable[key].Receipt,oldHashTable.Receipt);
            strcpy(hashTable[key].Sender,oldHashTable.Sender);
            break;
        }
        else
        {
            key = hashFunction(hashTable_Size,(key+1));  // If not, new key value is calculated.
        }

    }


}

//In this function  I got rid of Collision with Double Hashing.
void InsertingHashTableWithDoubleProbing(struct Mail *hashTable, struct Mail oldHashTable, int key, int hashTable_Size, int oldkey)

{
    int count = -1;
    int newkey= oldkey;

    while(1)
    {

        if((hashTable[newkey].Email_ID) == 0) // If  that row in the HashTable is blank

        {

            hashTable[newkey].Email_ID = oldHashTable.Email_ID;
            hashTable[newkey].countWords = oldHashTable.countWords;
            hashTable[newkey].Day_of_the_month = oldHashTable.Day_of_the_month;

            strcpy(hashTable[newkey].Receipt,oldHashTable.Receipt);
            strcpy(hashTable[newkey].Sender,oldHashTable.Sender);

            break;
        }
        else
        {
            count++; // for i values ( h(x) = ((h(x) +(i*h2(x))) % tableSize) );
            newkey = hashFunction(hashTable_Size,hashFunction(hashTable_Size,key)+count*hash2_Function(key)); // If not, new key value is calculated.
        }

    }


}

int hash2_Function(int key)
{

    return (5- (key % 5) );
}

/*In this function,I'm calculating the new table size. As we know, the new table size should be the nearest upper number to the number 2 times the old size.
 So first I multiply the table size by 2. If the resulting number is divisible by one of the prime numbers (1-10), it means that number is not a prime number,
so I increase the number again. If it is not divisible then that number is prime and this is my new table_Size.
We should know that although my function does not work properly for prime numbers less than 11 and 11 BUT,
WE ASSUMED THAT THE SMALLEST TABLE SIZE = 11.
the smallest number that can come to this function due to our assumption is 11, and the smallest prime number that this function can return from 11*2+1 is 23. .*/

double newLoadFactorFunction(int tableSize)
{
    tableSize = 2*tableSize;

    while(1)
    {

        if(tableSize%2 == 0 || tableSize%3 == 0  ||tableSize%5 == 0 || tableSize%7 == 0 )
            tableSize++;
        else
            return tableSize;
    }

}

void print_table(struct Mail *hashtable, int table_Size)
{


    int i;
    printf("Index     ID     Sender    Recipient    Date    Words\n");
    for(i = 0; i<table_Size; i++)
    {
        if(hashtable[i].Email_ID == 0)
        {
            printf("%d\n",i);
        }
        else
            printf("%-10d%-7d%-11s%-13s%-10d%-10d\n",i,hashtable[i].Email_ID,hashtable[i].Sender,hashtable[i].Receipt,hashtable[i].Day_of_the_month,hashtable[i].countWords);




    }
}

    void search(struct Mail *hashtable, int hashtable_Size,int email_id, char* sender_name, int typeOfResolve )
    {

        int value = email_id + sender_name[0] -65 ;

        int key = hashFunction(hashtable_Size,value) ;
        int old_key = key;
        int i;
        int flag = 1;


        if(typeOfResolve == 1)
        {


            for( i = 0; i<hashtable_Size; i++)

            {
                if(hashtable[key].Email_ID == 0)
                {
                     flag = 1;
                     break;
                }

                if((hashtable[key].Email_ID) == email_id)

                {
                    flag = 0;
                    printf("\nRecipient: %s\n",hashtable[key].Receipt);
                    printf("Date: %d\n", hashtable[key].Day_of_the_month);
                    printf("Number of words: %d\n",hashtable[key].countWords);

                    break;
                }
                else
                {
                    key = hashFunction(hashtable_Size,(key+1));  //Update key value

                }
            }

            if(flag)
                printf("\nStudent is not found!\n");

        }

        int doubleflag = 0;
        if(typeOfResolve == 2)

        {
            int count = -1;
            int i;
            for( i = 0;  i<hashtable_Size ; i++)

            {

                   if(hashtable[key].Email_ID == 0)
                {
                     doubleflag = 1;
                     break;
                }

                if((hashtable[key].Email_ID) == email_id)

                {
                    doubleflag = 0;
                    printf("Recipient: %s\n",hashtable[key].Receipt);
                    printf("Date: %d\n", hashtable[key].Day_of_the_month);
                    printf("Number of words: %d\n",hashtable[key].countWords);

                    break;
                }
                else
                {
                    count++;
                    key = hashFunction(hashtable_Size,hashFunction(hashtable_Size,old_key)+count*hash2_Function(old_key));
                }




            }

            if(doubleflag)
                printf("\nStudent is not found!\n");


        }
    }

void menu()
{
    printf("\nPlease choose one of the following options:\n") ;
    printf("(1) Search an email:\n");
    printf("(2) Print Table:\n");
    printf("(3) Exit\n");



}



