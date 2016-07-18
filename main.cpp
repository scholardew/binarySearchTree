//*********************************************************************************************************************************
// PROGRAMMER:  Dewey Greear
// FILENAME: 	Greear-Assn5-Prog.cpp
// DESCRIPTION: This program reads in a file containing integers and then inserts these numbers into a binary search tree.
//              The program then displays some choices for the user to choose from that include the following: display the list,
//              add a number to the list, delete a number from the list, and finding a particular number in the list and displaying
//              this number's subtree.
// CLASS        CS 372 / spring 8week1
//*********************************************************************************************************************************

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

//binary search tree nodes
struct treeNode
{
	int data;
	treeNode* leftPtr;
	treeNode* rightPtr;
};

//root node
struct treeRoot
{
	int countNodes;
	treeNode* treeTop;
};

treeRoot* createTree (); //creates a root node
bool isEmpty (treeRoot* rootPtr); //checks if tree is empty
void inOrderDisplay (treeNode* walker, int &countNumsPerLine); //outputs numbers
void deleteFromTree(treeNode* &nodeToDelete); //deletes node from tree
treeNode* findNode(treeRoot* rootPtr, int findItem, bool &found); //searches for a specific node
void insertNode (treeRoot* &rootPtr, int newData); //inserts new node in tree
void freeNodes (treeRoot* &rootPtr, treeNode* current); //deletes nodes from tree
void destroyTree (treeRoot* &rootPtr); //deletes root node
void deleteNode (treeRoot* &rootPtr, int userNumber); //calls destroy tree to delete nodes from tree
treeNode* createNode(treeNode* createNewNode); //creates new nodes
int nodeCount (treeRoot* rootPtr); //returns the count of nodes in the tree
int readInData(treeRoot* &rootPtr); //reads data from a file

const int MAX_NUMS = 10; //used for outputting 10 numbers per line

//*****************************************************************************************************************************************
//FUNCTION		: main
//DESCRIPTION	: This function acts as the main control for the program.
//CALLS TO		: createTree, readInData, isEmpty, inOrderDisplay, insertNode, deleteNode, findNode, freeNodes, destroyTree
//RETURNS		: Returns 0 upon successful completion
//OUTPUTS		: Outputs a user menu operated inside of a switch function in which most of the functions are called
//*****************************************************************************************************************************************
int main()
{	
	int userNumber; //number entered by user
	treeNode* findNumber; //pointer used for finding a user selected number
	int counter = 0; //number of nodes in the tree
	treeRoot* rootPtr; // root pointer node
	treeNode* walker = NULL;
	ifstream numberFile;
	int choice;
	bool found = false;
	int count = 0; // counter used for determining the amount of numbers are written per line
	
	rootPtr = createTree();

	counter = readInData (rootPtr);
	
	cout << "\nThere are " << rootPtr->countNodes << " numbers in the tree.\n\n";
	
	
	while (true)
    {
		cout << "What would you like to do?\n\n1. Display all the integers in the binary tree\n2. Add a value to the tree\n3."
			 << " Delete a value from the tree\n4. Find a value in the binary tree and display its subtree\n5. Exit the program\n"
			 << "\nEnter your selection: ";
		cin >> choice;
		cout << endl;
		switch (choice)
		{
	    	case 1:
	    		if(isEmpty(rootPtr)){
	    			cout << "This tree is empty\n\n";
	    		}
	    		else{
	    			count = 0; // resets numbers printed per line back to 0
					inOrderDisplay (rootPtr->treeTop, count);
					cout << endl << endl;
				}//end else
				break;
	    	case 2:
				cout << "Please enter a value to add to the tree: ";
				cin >> userNumber;
				insertNode(rootPtr, userNumber);		
				break;
	   	    case 3:
	   	    	if(isEmpty(rootPtr)){
	   	    		cout << "Cannot delete anything because the tree is empty\n\n";
	   	    	}//end if
	   	    	else{
					cout << "Please enter a value to delete: ";
					cin >> userNumber;
					deleteNode(rootPtr, userNumber);
					
				}//end else
				break;
		    case 4:	    
		    	if(isEmpty(rootPtr)){
		    		cout << "Cannot find anything because tree is empty\n\n";
		    	}//end if
		    	else{
					cout << "Please enter a value to find: ";
					cin >> userNumber;
					cout << endl;
					findNumber = findNode (rootPtr, userNumber, found);
					if (findNumber->data != userNumber)
						cout << userNumber << " was not found in the list\n\n";
					else{
						count = 0;// resets numbers printed per line back to 0
						cout << "The values stored in the subtree whose root is " << userNumber << " are:\n\n";
						inOrderDisplay(findNumber, count);
						cout << endl << endl;
					}//end else
				}//end else
	    		break;
	    	case 5:
				return 0;
	    	default:
				cout << "Input was not one of the choices...\n\n";
		}//end switch
    }//end while
	freeNodes(rootPtr, walker);
	destroyTree(rootPtr);
	return 0;
}//end main

//*****************************************************************************************************
//FUNCTION		: readInData
//DESCRIPTION	: This file reads in a text file containing numbers
//PARAMETERS	: rootPtr - this is the root of the binary search tree
//CALLS TO		: insertNode
//OUTPUTS		: Prompts user for a filename, and if needed prints out errors
//******************************************************************************************************
int readInData(treeRoot* &rootPtr)
{
	ifstream numberFile; 
	string textFile;//user input for text file
	int newData;
	int nodeCounter = 0;
	do{
		cout << "Enter a text file to be entered into the program: ";
		cin >> textFile;
		numberFile.open(textFile.c_str());
		if(!numberFile){
			cout << "This file cannot be found, please try again." << endl;
		}//end if
	}while(!numberFile);
	
	numberFile >> newData;

	if (!numberFile){
		cout << "\nNo data is in the file" << endl;
		//rootPtr->countNodes = 0;
	}
	else{	
		insertNode (rootPtr, newData);
	
		while(numberFile){
			numberFile >> newData;
			if (numberFile)
			insertNode (rootPtr, newData);
		}//end while	
		numberFile.close();
	}//end else
	nodeCounter = nodeCount(rootPtr);
	return nodeCounter;
}// end readInData

//**************************************************************************************************************************
//FUNCTION		: nodeCount
//DESCRIPTION	: This function returns the number of nodes in the tree
//PARAMETERS	: rootPtr - this is the root of the binary search tree
//*************************************************************************************************************************
int nodeCount (treeRoot* rootPtr)
{
	return rootPtr->countNodes;
}//end nodeCount

//*************************************************************************************************************************
//FUNCTION		: createNode
//DESCRIPTION	: This function dynamically allocates memory for each new node and returns this node to the calling function
//PARAMETERS	: createNewNode - the newly created node
//RETURNS		: Returns newly created binary tree node
//OUTPUTS		: Outputs error if problems occur with memory allocation
//*************************************************************************************************************************
treeNode* createNode (treeNode* createNewNode){
	
	createNewNode = new (nothrow) treeNode;
	
	if (createNewNode == NULL){
		cout << "Heap memory error, unable to allocate memory." << endl;
		
	}//end if
	else{
		createNewNode->leftPtr = NULL;
		createNewNode->rightPtr = NULL;
	}//end else

	return createNewNode;
}//end createNode

void destroyTree (treeRoot* &rootPtr)
{
//	rootPtr->treeTop = freeNodes (rootPtr->treeTop);
	rootPtr->countNodes = 0;
	delete rootPtr->treeTop;
	rootPtr->treeTop = NULL;
	return;
}//end destroyTree

//*********************************************************************************************************************************************
//FUNCTION		: deleteNode
//DESCRIPTION	: This function finds the node to be deleted and deletes the node
//PARAMETERS	: rootPtr - this is the root of the binary search tree
//                node_value - this is the value in the tree that will be deleted
//CALLS TO		: deleteFromTree
//*********************************************************************************************************************************************
void deleteNode(treeRoot* &rootPtr, int userNumber){
		
	bool found = false;						
	
	treeNode* current;						
	treeNode* previousCurrent;				
	
	current = rootPtr->treeTop;
	previousCurrent = rootPtr->treeTop;

	while (current != NULL && !found)
	{
		if (current->data == userNumber)
			found = true;
			
		else
		{
			previousCurrent = current;

			if (current->data > userNumber)
				current = current->leftPtr;

			else
				current = current->rightPtr;
		} // end else
	} // end while
	
	// Item not found
	if (current == NULL){
		cout << "\nThe number is not in the list\n";
		cout << endl;
	}// end if
	else if (found)
	{
		if (current == rootPtr->treeTop)
			deleteFromTree(rootPtr->treeTop);
		
		else if (previousCurrent->data > userNumber)
			deleteFromTree(previousCurrent->leftPtr);
		
		else
			deleteFromTree(previousCurrent->rightPtr);
			
		cout << "\nRemoved value " << userNumber << " from tree\n\n";
		
		rootPtr->countNodes = rootPtr->countNodes -1;
	} // end else if
	return;
}//end deleteNode

//*************************************************************************************************************************
//FUNCTION		: deleteFromTree
//DESCRIPTION   : This function gets called from deleteNode and deletes the tree node picked by the user
//PARAMETERS	: deleteNode - binary search tree node to be deleted
//*************************************************************************************************************************
void deleteFromTree(treeNode* &deleteNode)
{
	treeNode* current;						
	treeNode* previousCurrent;				
	treeNode* temp;							
			
	if (deleteNode == NULL){
		cout << "Cannot delete node because value is NULL";
		cout << endl;
	}//end if
	
	// node has no leaves 
	else if (deleteNode->leftPtr == NULL && deleteNode->rightPtr == NULL)
	{
		temp = deleteNode;
		deleteNode = NULL;

		delete temp;
	} // end else if
	
	else if (deleteNode->leftPtr == NULL)
	{
		temp = deleteNode;
		deleteNode = temp->rightPtr;
		
		delete temp;
	} // end else if


	else if (deleteNode->rightPtr == NULL)
	{
		temp = deleteNode;
		deleteNode = temp->leftPtr;
		
		delete temp;
	} // end else if

	else
	{
		current = deleteNode->leftPtr;
		previousCurrent = NULL;
		
		while (current->rightPtr != NULL)
		{
			previousCurrent = current;
			current = current->rightPtr;
		} // end while
		
		deleteNode->data = current->data;
		
		if (previousCurrent == NULL)
			deleteNode->leftPtr = current->leftPtr;
		else
			previousCurrent->rightPtr = current->leftPtr;
			
		delete current;
	} // end else
	return;
}

//****************************************************************************************************************************************************************

//*******************************************************************************************************************
//FUNCTION		: freeNodes
//DESCRIPTION	: This function recursively deallocates memory to all of the treeNodes structures
//PARAMETERS	: rootPtr - root pointer for binary search tree
//				  current - treeNode used to traverse the binary search tree
//*******************************************************************************************************************
void freeNodes (treeRoot* &rootPtr, treeNode* current)
{
	current = rootPtr->treeTop;
	if (current != NULL){
		freeNodes (rootPtr, current->leftPtr);
		freeNodes (rootPtr, current->rightPtr);
		delete current;
		current = NULL;
	}//end if
	return;
}//end freeNodes

//*******************************************************************************************************************
//FUNCTION		: insertNode
//DESCRIPTION	: This function inserts numbers into the binary search tree
//PARAMETERS	: rootPtr - root pointer for binary search tree
//                newData - new number to be inserted into binary search tree
//CALLS TO		: createNode
//********************************************************************************************************************
void insertNode (treeRoot* &rootPtr, int newData)
{
	treeNode* current = NULL;
	treeNode* previousCurrent = NULL;
	treeNode* newNode = NULL;
	treeNode* temp = NULL;           //used to see if value is found
	bool found = false;
	newNode = createNode(newNode);
	newNode->data = newData;
	newNode->leftPtr = NULL;
	newNode->rightPtr = NULL;
	
	if (rootPtr->treeTop == NULL){
		rootPtr->treeTop = newNode;
		rootPtr->countNodes++;			//increment count node
	}//end if

	else{
		current = rootPtr->treeTop;	
		rootPtr->countNodes++;      //increment count node
		while (current != NULL){
			previousCurrent = current;
			if (current->data == newData){
				cout << endl;
				cout << current->data << ": ";
				cout << "This number is a duplicate and will be ignored." << endl;
				rootPtr->countNodes = rootPtr->countNodes - 1;
				return;
			}//end if
		    else if (current->data > newData)
				current = current->leftPtr;
			else
				current = current->rightPtr;
		}//end while
			
		if (previousCurrent->data > newData)
			previousCurrent->leftPtr = newNode;
				
		else
			previousCurrent->rightPtr = newNode;
	}//end else
	return;
}//end insertNode

//**********************************************************************************************************************
//FUNCTION		: findNode
//DESCRIPTION	: Looks for a node in the tree that is to be deleted
//PARAMETERS    : rootPtr - root pointer to the binary search tree
//                findItem - number that is to be searched for
//                found - returns true if found, otherwise it returns false
//                parentNode - pointer to the value before current
//RETURNS		: Returns the current value of item that is found
//              : Returns parentNode by reference
//***********************************************************************************************************************
treeNode* findNode(treeRoot* rootPtr, int findItem, bool &found)
{
	treeNode* current;
	treeNode* previousCurrent;
	
	if (rootPtr->treeTop == NULL)
		cout << "\nTree is empty\n";
	else{ 
		current = rootPtr->treeTop;
		while (current != NULL && !found){
			if (current->data == findItem)
				found = true;				
			else if (current->data > findItem){
				previousCurrent = current;			
				current = current->leftPtr;
			}//end else if
			else{				
				previousCurrent = current;
				current = current->rightPtr;
			}//end else
		}//end while
		if(!found)
			current = previousCurrent;
	}//end else
	return current;
	
}//end findNode

//**************************************************************************************************************
//FUNCTION		: createTree
//DESCRIPTION	: This function creates a new binary search tree and sets the count of nodes to 0, while initializing
//                the root to NULL
//RETURNS		: Returns the newly created binary search tree
//*****************************************************************************************************************
treeRoot* createTree ()
{
	treeRoot *rootPtr;
	rootPtr = new (nothrow) treeRoot;
	if (rootPtr == NULL)
		cout << "Heap memory error, program terminating" << endl;
	rootPtr->treeTop = NULL;
	rootPtr->countNodes = 0;
	return rootPtr;
}//createTree 

//***********************************************************************************************************************
//FUNCTION		: isEmpty
//DESCRIPTION 	: This function checks to see if the binary search tree is empty
//PARAMETERS	: rootPtr - this is the root of the binary search tree
//RETURNS		: Returns true if binary search tree if empty
//***********************************************************************************************************************
bool isEmpty (treeRoot* rootPtr)
{
	if (rootPtr->treeTop == NULL)
		return true;
	else
		return false;
}//end isEmpty

//************************************************************************************************************************
//FUNCTION		: inOrderDisplay
//DESCRIPTION	: This function recursively displays the numbers in the search tree using in order traversal
//PARAMETERS	: walker - treeNode used to traverse the list
//CALLS TO		: inOrderDisplay
//*************************************************************************************************************************
void inOrderDisplay (treeNode* walker, int &countNumsPerLine) //***note to self - always use call by reference when passing a number!!!!!!!
{	
	if (walker != NULL){
		inOrderDisplay (walker->leftPtr, countNumsPerLine);
		cout << setw(5) << walker->data;
		++countNumsPerLine;
		if (countNumsPerLine == MAX_NUMS){
			cout << endl;
			countNumsPerLine = 0;			
		}//end if
		inOrderDisplay (walker->rightPtr, countNumsPerLine);
	}//end if
	return;
}//end inOrderDisplay




