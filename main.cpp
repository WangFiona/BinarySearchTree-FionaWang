#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

/*
 * Author: Fiona Wang
 * Date: April 18, 2022
 * This program takes an input from the console or a file and builds a binary search tree
 */

//Class for the nodes used in the binary tree
class BNode {
  int data;
  BNode* left;
  BNode* right;

public:
  //Constructors
  BNode(int newData){
    data = newData;
    left = NULL;
    right = NULL;
  }

  //Set and get functions
  void setLeft(BNode* newLeft){
    left = newLeft;
  }

  void setRight(BNode* newRight){
    right = newRight;
  }

  BNode* getLeft(){
    return left;
  }

  BNode* getRight(){
    return right;
  }

  void setData(int newData){
    data = newData;
  }

  int getData(){
    return data;
  }
};

//Declaring the functions
void addConsole(BNode* &tree);
void addFile(BNode* &tree);
void add(BNode* tree, int data);
void printTree(BNode* tree, int count);
int search(BNode* tree, int searchNum);
//void deleteOne(BNode* tree, BNode* previous, int deleteNum, int root);
BNode* deleteOne(BNode* tree, int deleteNum);
BNode* nextValue(BNode* tree);
int leafCount(BNode* node);

int main() {
  //Initializing variables
  BNode* tree = new BNode(-1);
  char command[15];
  char fileType[10];
  bool running = true;

  //Ask the user for a command
  cout << "Enter a command (enter add, search, delete, print, or quit)" << endl;
  while(running == true){
    cin >> command;
    
    //Allow both upper and lower case
    for(int i=0; i < strlen(command); i++) {
      command[i] = toupper(command[i]);
    }
    
    if(strcmp(command, "ADD") == false){
      cout << "Enter a input type (console or file)" << endl;
      cin >> fileType;
      
      //Allow both upper and lower case
      for(int i=0; i < strlen(fileType); i++) {
	fileType[i] = toupper(fileType[i]);
      }

      //Ask the user if they want to add from the console or a file
      if(strcmp(fileType, "CONSOLE") == false){
	addConsole(tree);
      }
      else if(strcmp(fileType, "FILE") == false){
	addFile(tree);
      }
      else{
	cout << "Not a valid file type" << endl;
      }

      cout << " Leaf Count: " << leafCount(tree) << endl;

      cout << "Enter a command (enter add, search, delete, print, or quit)" << endl;
    }
    else if(strcmp(command, "SEARCH") == false){
      //Ask what number to fine
      cout << "What number do you want to find?" << endl;
      int searchNum;
      cin >> searchNum;
      int found = search(tree, searchNum);
      //Search for the number
      if(found == searchNum)
	cout << searchNum << " exists in the tree!" << endl;
      else
	cout << searchNum << " does not exist in the tree!" << endl;
      cout << "Enter a command (enter add, search, delete, print, or quit)" << endl;
    }
    else if(strcmp(command, "DELETE") == false){
      //Ask what number to delete
      cout << "What number do you want to delete?" << endl;
      int deleteNum;
      cin >> deleteNum;
      int found = search(tree, deleteNum);
      int root = tree->getData();
      //Check if the number exists in the tree
      if(found == deleteNum)
        tree = deleteOne(tree, deleteNum);
      else
        cout << deleteNum << " does not exist in the tree!" << endl;

      cout << "Enter a command (enter add, search, delete, print, or quit)" << endl;
    }
    else if(strcmp(command, "PRINT") == false){
      //Print the whole tree
      printTree(tree, 0);
      cout << "Enter a command (enter add, search, delete, print, or quit)" << endl;
    }
    else if(strcmp(command, "QUIT") == false){
      running = false;
    }
    else{
      cout << "Please enter a valid command:" << endl;
    }
  }

  return 0;
}

int leafCount(BNode* node){
  if(!node)
    return 0;
  else if(!node->getLeft() && !node->getRight())
    return 1;
  else
    return leafCount(node->getLeft()) + leafCount(node->getRight());
}

//Function to take an input from a file
void addFile(BNode* &tree){
  char* fileName = new char[20];

  //Ask the user to enter the file name
  cout << "Enter the name of the file: (include the '.txt')" << endl;
  cin >> fileName;
  fstream file;
  file.open(fileName);

  //Extract numbers from the file
  int input = 0;
  while(file >> input){
    add(tree, input);
  }
  file.close();
}

//Function to take an input from the console
void addConsole(BNode* &tree){
  bool entering = true;
  char input[10];

  //Ask the user for numbers or the word "done"
  cout << "Enter a number (1-999) or enter \"done\"" << endl;

  //Keep asking for input until the user enters "done"
  while(entering){
    cin >> input;
    //Allow both upper and lower case
    for(int i=0; i < strlen(input); i++) {
      input[i] = toupper(input[i]);
    }

    //Check if the input is a number or "done"
    bool isNum = true;
    for(int i=0; i<strlen(input); i++){
      if(!isdigit(input[i])){
	isNum = false;
      }
    }

    //If the user is done, leave the loop
    if(strcmp(input, "DONE") == false){
      entering = false;
    }
    //Keep asking for numbers
    else if(!isNum){
      cout << "Enter a number (1-999) or enter \"done\"" << endl;
    }
    else{
      int num = atoi(input);
      add(tree, num);
      cout << "Enter a number (1-999) or enter \"done\"" << endl;
    }
  }
}

//Function to add into the tree
void add(BNode* tree, int data){
  //Check if you are adding at the root
  if(tree->getData() == -1) {
    tree->setData(data);
    return;
  }
  else {
    //Check if the new node should go to the right or left
    //If to the left, check if it is at the base of the tree
    if(data < tree->getData() && tree->getLeft()){
      tree = tree->getLeft();
      add(tree, data);
    }
    //Add the new node
    else if(data < tree->getData() && !tree->getLeft()){
      BNode* newNode = new BNode(data);
      tree->setLeft(newNode);
      return;
    }

    //If to the right, check if it is at the base of the tree
    if(data > tree->getData() && tree->getRight()){
      tree = tree->getRight();
      add(tree, data);
    }
    //Add the new node
    else if(data > tree->getData() && !tree->getRight()){
      BNode* newNode = new BNode(data);
      tree->setRight(newNode);
      return;
    }
  }
}

//Function to print out the binary search tree
void printTree(BNode* tree, int count) {
  //Check if there is anything in the tree
  if(!tree || tree->getData() == -1){
    cout << "The tree is empty!" << endl;
    return;
  }

  //Go through the right side of the tree
  if(tree->getRight())
    printTree(tree->getRight(), count+1);

  //Print with right amount of tabs
  for(int i=0; i<count; i++){
    cout << "\t";
  }
  cout << tree->getData() << endl;

  //Go through the left side of the tree
  if(tree->getLeft())
    printTree(tree->getLeft(), count+1);
}

//Function to check if a certain number exists in the tree
int search(BNode* tree, int searchNum) {
  //If the number has been found or the tree is empty
  if(!tree || tree->getData() == searchNum){
    return tree->getData();
  }

  //If the number is bigger, then traverse to the right
  if(tree->getData() < searchNum){
    //If you are at the bottom of the tree
    if(!tree->getRight())
      return tree->getData();
    return search(tree->getRight(), searchNum);
  }

  //If the number is smaller, then traverse to the left
  else {
    //If you are at the bottom of the tree
    if(!tree->getLeft())
      return tree->getData();
    return search(tree->getLeft(), searchNum);
  }
}

//Function to delete a specific node in the tree
BNode* deleteOne(BNode* tree, int deleteNum){
  //If the tree is empty
  if(!tree){
    return tree;
  }
  //If the number is bigger, traverse to the right node
  if(tree->getData() < deleteNum){
    tree->setRight(deleteOne(tree->getRight(), deleteNum));
  }
  //If the number is smaller, traverse to the left node
  else if(tree->getData() > deleteNum){
    tree->setLeft(deleteOne(tree->getLeft(), deleteNum));
  }

  //If the correct node is found
  else if(tree->getData() == deleteNum){
    //If it has no children nodes
    if(!tree->getLeft() && !tree->getRight()){
      return NULL;
    }

    //If it has one child node (left or right)
    else if(!tree->getLeft()){
      BNode* temp = tree->getRight();
      delete tree;
      return temp;
    }
    else if(!tree->getRight()){
      BNode* temp = tree->getLeft();
      delete tree;
      return temp;
    }

    //If it has two children
    BNode* temp = nextValue(tree->getRight());
    tree->setData(temp->getData());
    tree->setRight(deleteOne(tree->getRight(), temp->getData()));
  }
  return tree;
}

//Function to fine the next smallest number in the tree
BNode* nextValue(BNode* tree){
  BNode* current = tree;
  //Traverse to the bottom of the tree
  while(current && current->getLeft()){
    current = current->getLeft();
  }
  return current;
}
