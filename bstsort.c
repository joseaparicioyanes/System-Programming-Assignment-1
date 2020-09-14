/*
*******************************************************************************************************************************************
 *Purpose/Description: 		The main objective of this program is  to sort the lines of an input file (or from standard input) and print the sorted 
			lines to an output file (or standard output). Depending on the commands entered by the user, the program must distinguish between 
			uppercase and lowercase letters, and must set the output to fail or the standard input/output. To perform all these tasks, there 
			are several functions that perform each task independently. The first is readFromFile which takes two character arrays as parameters 
			where the first represents the name of the input file and the other represents the name of the output file (if any), and two integers 
			one is cflag (to identify if the comparison should be case sensitive or not), and another for oflag (it is used to identify if there 
			is an output file or not). This function reads line by line from the file and starts calling the other functions to perform all the 
			tasks required by the user (the other functions are described below). The readFromConsole function conforms to the aforementioned 
			function description, with the only deference that it does not receive the inputFileName parameter. The functions stringCompareCS 
			(case sensitive comparison), and stringCompare receive as parameters two-character arrays which are compared according to the ASCII 
			table. These functions return 1 if the first argument is greater than the second, 0 if the two arguments are the same, or -1 if the 
			second argument is greater than the first. The newNode function is called from the insert function, it receives as an argument an 
			array of characters and allocates it in memory using malloc and thus creating a new node. The insert function receives as a parameter 
			a pointer to a struct (node), an array of characters that represents the line of text (key), and an integer that represents cflag to 
			know if the comparisons should be made case sensitive or not. The inorderTraversal function receives as parameters a pointer to a 
			struct (the root of the tree), a 2D array of characters to save all the lines of text in case they go to an output file, an array of 
			integers to save the number of occurrences of each line of text, and an integer representing oflag. The postOrderTraversal function 
			receives a pointer to a struct (the root of the tree) as an argument and deallocates all the memory occupied by the tree using the 
			free() function. Finally, the writeToFile function receives as an argument an array of characters that represents the name of the 
			output file, a 2D array that represents the information that must be written to the file, and an array of integers that represents 
			the number of occurrences of each line of text.   

 * Author’s Name: Jose Aparicio
 * Author’s Panther ID: 6186111 
 * Certification: I hereby certify that this work is my own and none of it is the work of any other person.
*********************************************************************************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define stringSize 101				//defining the maximum string size incuding the null charcter
int numberOfLines = 0;				//declaring a variable to count the number of lines to create an array of that size
int arrayCounter = 0;                           //declaring a variable to iterate through the array


struct node{                                    //creating a struct to shape the nodes
	char *key;                              //the key is a pointer to a char to be allocated using malloc
	struct node *left;			//declaring a pinter to the left child
	struct node *right;                     //declaring a pointer to the right child
	int occurrences;                        //integer to count the occurrences of each line of text
};

void readFromFile(char inputFileName[],char outputFileName[],int cflag,int oflag);              //declaring the function readFromFile 
void readFromConsole(char outputFileName[],int cflag,int oflag);				//declaring the function readFromConsole
int stringCompareCS(char string1[], char string2[]);						//declaring the function stringCompareCS
int stringCompare(char str1[], char str2[]);							//declaring the function stringCompare
struct node *newNode(char newKey[]);								//declaring the function newNode
struct node* insert(struct node* node,char key[], int cflag);					//declaring the function insert
void inorderTraversal(struct node *root, char output[][stringSize], int occ[],int oflag);	//declaring the function inorderTraversal
void postOrderTraversal(struct node* root);							//declaring the function postOrderTraversal
void writeToFile(char outputFileName[], char output[][stringSize],int occ[]);			//declaring the function writeToFile

int  main(int argc, char ** argv)                                     		//main function
{
	extern char *optarg;                                          		//external variable (getopt)
	extern int optind;				              		//external variable (getopt)
	int c;                                                        		//variables to be used in the while loop
	char inputFileName[stringSize];			              		//char array used to store the input file name
	char outputFileName[stringSize];                              		//char array used to store the output file name
	int cflag=0;						      		//integer variable to be use as a boolean to know if the user wants a case sensitive solution or not
	int oflag=0;						      		//integer variable to be use as a boolean to know if the output is to a file or to the console
	int inputFromConsole=0;
		                                                      
	while ((c = getopt(argc, argv, "c :ols")) != -1){                        //while loop to iterate through all the arguments
		switch (c) {
			case 'c':
				cflag=1;					//retting the cflag to 1 in case c is present in the arguments
				break;
			case 'o':
				oflag=1;					//retting the oflag to 1 in case o is present in the arguments
				strcpy(outputFileName,argv[optind]);            //getting the name of the output file
				if((optind+1)==argc)
                                        inputFromConsole=1;                     // assuming input from console if there is no mor arguments
				break;
			case '?':
				break;
		}
	}

	if(inputFromConsole==1 || argc==1 || (cflag==1 && argc==2)){            //if statement to know if the input is from a file or from the console
		readFromConsole(outputFileName,cflag,oflag);                    //calling the readFromConsole function
	}else{
		strcpy(inputFileName, argv[argc-1]);                            //getting the name of the input file
		readFromFile(inputFileName, outputFileName,cflag,oflag);        //calling the function readFromFile
	}
	return 0;
}


void readFromConsole(char outputFileName[],int cflag,int oflag){                        //function to get the input from the console
        char line[stringSize];								//array of char to store the line of text
        struct node *root=NULL;								//creating a pointer to a struct wish is the root of the tree
        int lineNumber=0;								//int to be use as boolean to know if it the first line or not								

	while(fgets(line,stringSize,stdin)!=NULL){                                      //while loop to loop until the end of the user input
		if(lineNumber==0){                                                      //check if it is the first line
			lineNumber=1;                                                   //setting lineNumber to 1
			if(line[0]==10)                                                 //if statement to break the while loop if the user enter a blank line
                       		break;
                        root=insert(root,line,cflag);                                   //calling the insert function to create a new node
                        numberOfLines++;                                                //increasing the number of lines
                }else{
			if(line[0]==10)                                                 //if statement to break the while loop if the user enter a blank line
                                break;
                        insert(root,line,cflag);                                        //calling the insert function to create a new node if it does not exist
                        numberOfLines++;                                                //increasing the number of lines
                       }	
        }

        const int size = numberOfLines;                                                 //creating a constant variable to be used as the size of an array
        char output[size][stringSize];                                                  //creating a 2D char array to store all the text lines in the tree to print them to a file
        int occ[size];									//creating an int array to store the occurrences of each line in the tree to print them to a file

        inorderTraversal(root,output,occ,oflag);                                        //calling the inorderTraversal function

        if(oflag==1)                                                                    //if the oflag is 1, calling the write to file function
                writeToFile(outputFileName,output,occ);
	postOrderTraversal(root);                                                       //calling the postOrderTraversal function to deallocate all the space in memory
}

void readFromFile(char inputFileName[],char outputFileName[],int cflag,int oflag){				//function to read from the file
        int count=1;												
        FILE* inputFile;
        char line[stringSize];											//array of char to store the line of text
	struct node *root=NULL;											//creating a pointer to a struct wish is the root of the tree
	int lineNumber=0;											//int to be use as boolean to know if it the first line or not

        if((inputFile = fopen(inputFileName,"r")) == NULL){                                                     //opening the file or displaying a message if it cannot be open
                printf("Error opening %s\n", inputFileName);
                exit(100);
        }


        while(fgets(line,stringSize,inputFile) ){                                                       //while loop to iterate through the file getting line by line
			if(lineNumber==0){								//check if it is the first line
				lineNumber=1;								//setting lineNumber to 1
				if(line[0]=='\n')							//if statement to break the while loop if there is a blank line
					break;
				root=insert(root,line,cflag);						//calling the insert function to create a new node
				numberOfLines++;							//increasing numberOfLines
			}else{
				if(line[0]=='\n') 							//if statement to break the while loop if there is a blank line
                                        break;
				insert(root,line,cflag);						//calling the insert function to create a new node if it does not exist
				numberOfLines++;							//increasing numberOfLines
			}
        }


        if(fclose(inputFile) == EOF){                                                                   //closing the file or displaying a message if it cannot be close
                printf("Error closing %s\n", inputFileName);                            
                exit(102);
        }
	
	const int size = numberOfLines;									//creating a constant variable to be used as the size of an array
	char output[size][stringSize];									//creating a 2D char array to store all the text lines in the tree to print them to a file
	int occ[size];											//creating an int array to store the occurrences of each line in the tree to print them to a file
	
	inorderTraversal(root,output,occ,oflag);							//calling the inorderTraversal function
 	
	if(oflag==1)											//if the oflag is 1, calling the write to file function
        	writeToFile(outputFileName,output,occ);
	postOrderTraversal(root);									//calling the postOrderTraversal function to deallocate all the space in memory
}



int stringCompareCS(char string1[], char string2[]){							//functin to compare two strings, case sensitive
//if s1>s2 return 1, else if s1<s2 return -1, else if s1==s2 return 0
	int i;                                                                                          //variable to be used in the for loop
	int iterations = strlen(string1)<strlen(string2)?strlen(string1):strlen(string2);               //calculating the number of iteratins in the for loop that is equal to the size of the smallest string

	for(i=0;i<iterations;i++){                                                                      //for loop to iterate through the strings
		if(string1[i]>string2[i])                                                		//if the character in position i of string1 is greater than  the character in position i of string2, then return 1
			return 1;
		else if(string1[i]<string2[i])								//else if the character in position i of string1 is less than  the character in position i of string2, then return -1
			return-1;
	}
	return strlen(string1)==strlen(string2)?0:strlen(string1)>strlen(string2)?1:-1;                 //else retun 0, or return 1 if the sise of string 1 is bigger than the size of string2 or othrwise return -1 
}

int stringCompare(char str1[], char str2[]){								//functin to compare two strings, case sensitive
//if s1>s2 return 1, else if s1<s2 return -1, else if s1==s2 return 0
        int i;												//variable to be used in the for loop
	char string1[stringSize];									//creating a char array to store str1
	char string2[stringSize];									//creating a char array to store str2
	strcpy(string1,str1);										//setting string1 equal to str1
	strcpy(string2,str2);										//setting string2 equal to str2
        int iterations = strlen(string1)<strlen(string2)?strlen(string1):strlen(string2);		//calculating the number of iteratins in the for loop that is equal to the size of the smallest string
	
        for(i=0;i<iterations;i++){									//for loop to iterate through the strings
		if(string1[i]>=97)									//if statemento to convert all the lowwer case letters to upper case, just to compare them not permanently
			string1[i]-=32;
		if(string2[i]>=97)									//if statemento to convert all the lowwer case letters to upper case, just to compare them not permanently
			string2[i]-=32;

                if(string1[i]>string2[i])								//if the character in position i of string1 is greater than  the character in position i of string2, then return 1
                        return 1;
                else if(string1[i]<string2[i])								//else if the character in position i of string1 is less than  the character in position i of string2, then return -1
                        return-1;
        }
        return strlen(string1)==strlen(string2)?0:strlen(string1)>strlen(string2)?1:-1;			//else retun 0, or return 1 if the sise of string 1 is bigger than the size of string2 or othrwise return -1

}

struct node* insert(struct node* node,char key[], int cflag){						//function to insert a new node with a given key in the binary search tree 
	if (node == NULL)										//if statement to return a new node if the tree is empty 
		return newNode(key); 
  
	if(cflag==0){                                                                                   //if the case sensitive flag is false then compare using the function stringCompare
		if (stringCompare(key, node->key)==-1)                                                  //if-else if statements to recur down the tree
			node->left  = insert(node->left, key,cflag); 
    		else if (stringCompare(key, node->key)==1) 
        		node->right = insert(node->right, key,cflag);
		else if (stringCompare(key, node->key)==0){
			node->occurrences++;                                                            //increasing the occurrences if the new key is equal to an existent key
			numberOfLines--;                                                                //increasing the number of lines
		}
  	}else if(cflag==1){										//if the case sensitive flag is true then compare using the function stringCompareCS
                if (stringCompareCS(key, node->key)==-1)						//if-else if statements to recur down the tree
                        node->left  = insert(node->left, key,cflag);
                else if (stringCompareCS(key, node->key)==1)
                        node->right = insert(node->right, key,cflag);
		else if (stringCompareCS(key, node->key)==0){
                        node->occurrences++;								//increasing the occurrences if the new key is equal to an existent key
			numberOfLines--;								//increasing the number of lines
		}
	}

    	return node; 
}

struct node *newNode(char newKey[]) 									//function to create a new node
{ 
	struct node *temp =  (struct node *)malloc(sizeof(struct node));                                //alocatinc the new node in memory using the mallogc function
	temp->key = malloc(sizeof(newKey));								//allocating space in memory for the key
    	strcpy(temp->key,newKey);                                                                       //setting the key to the new key 
    	temp->left = temp->right = NULL;                                                                //setting the pinters to the left and right child as null
	temp->occurrences=1;                                                                            //setting the number of occurrences to 1
    	return temp; 
} 

void writeToFile(char outputFileName[], char output[][stringSize],int occ[]){				//function to write to the output file
        FILE* outputFile;
	int i;												//variable to be used in the for loop

	if((outputFile = fopen(outputFileName,"w")) == NULL){            				//opening the file or displaying a message if it cannot be open
                printf("Error opening %s\n", outputFileName);
                exit(100);
        }

	for(i=0;i<numberOfLines;i++)									//for loop to print to the file the lines of text and the occurrences
        	fprintf(outputFile,"%d, %s",occ[i],output[i]);

        if(fclose(outputFile) == EOF){                                                       		//closing the file or displaying a message if it cannot be close
                printf("Error closing %s\n", outputFileName);
                exit(102);
        }
}


void inorderTraversal(struct node *root, char output[][stringSize], int occ[], int oflag) 		//function to preform an in-order traversal
{
    	if (root != NULL){                                                                              
       		inorderTraversal(root->left,output,occ,oflag); 
		if(oflag==0)										//if oflag is equal to 0, print the result to the console
       			printf("%d, %s \n",root->occurrences, root->key);
		else{
			occ[arrayCounter]=root->occurrences;                                            //else copy the occurrences to the occ array, and the key to the output array to print it to a file
			strcpy(output[arrayCounter++],root->key);
		} 
       		inorderTraversal(root->right,output,occ,oflag); 
    	} 
}


void postOrderTraversal(struct node* root) {								//function to preform an post-order traversal
	if(root != NULL) {
		postOrderTraversal(root->left);
		postOrderTraversal(root->right);
		free(root->key);									//deallocating the memory used by each key
		free(root);                                                                             //deallocating the memory used by each node
		root->left=NULL;									//setting the pointers to null, so that memory location can not be accessed any more using these pointers
		root->right=NULL;									//setting the pointers to null, so that memory location can not be accessed any more using these pointers
   }
}
