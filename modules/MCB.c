#include <system.h>
#include "MCB.h"
#include "commandHandler.h"
#include <string.h>



//Inititializes the lists so they can be used
void initLists(){
	memList=sys_alloc_mem(sizeof(list));
    memList->head = NULL;
    memList->count = 0;

}

//shows the starting adderss and size of each block in the list in order
void showFree(){
    serial_println("--Free memory--");
    cmcb* currNode = memList->head;
    	char *temp1=NULL;
	char *temp2=NULL;
    while (currNode != NULL) {
	if(currNode->type==0){
	//serial_println("*");
	serial_println("Beginning Address");
	temp1=itoa(currNode->begAddress,temp1,10);
	serial_println(temp1);
	temp1=NULL;
	serial_println("Size of Block:");
	temp2=itoa(currNode->sizeOfBlock,temp2,10);
	serial_println(temp2);
	temp2=NULL;
}
	//display1(currNode);
	//number=(number & 0x7fffff) | (currNode->begAddress << 23);
	//ptr= (const char *) number;
	//serial_println((const char *)currNode->begAddress);
	//serial_println("Size:");
	//number=(number & 0x7fffff) | (currNode->sizeOfBlock << 23);
	//ptr= (char *) number;
	//serial_println((const char *)&currNode->sizeOfBlock);
	//serial_println((const char *)currNode->sizeOfBlock);
	currNode = currNode->next;
    }
}

//shows the starting address and size of each block in the list in order
void showAllocate(){
    serial_println("--Allocated memory--");
//serial_println("*");
	char *temp1=NULL;
	char *temp2=NULL;
    cmcb* currNode = memList->head;
	//serial_println("*");
    while (currNode != NULL) {
	//serial_println("#");
	if(currNode->type==1){
	serial_println("Beginning Address");
	temp1=itoa(currNode->begAddress,temp1,10);
	serial_println(temp1);
	temp1=NULL;
	serial_println("Size of Block:");
	temp2=itoa(currNode->sizeOfBlock,temp2,10);
	serial_println(temp2);
	temp2=NULL;
}
	//serial_println(temp1);
	//serial_println("Size:");
	//serial_println(temp2);
	currNode = currNode->next;
    }
}

//Returns 1 if empty, 0 if not empty
int isEmpty(){
    if (memList->count == 0)
    {
	return 1;
}
else{
    return 0;

}
}
//inserts a cmcb into the appropriate place in the list
void insertMCB(cmcb *cm){


    //When list is empty
    if (memList->count == 0){
	memList->head = cm;
	cm->next = NULL;
	cm->prev = NULL;
	memList->count++;
    }
    //When list has one or more members
    else if (memList->count > 0){
	cmcb *currNode = memList->head;
	while (currNode != NULL){
	    //When cm's memory location is before memlist->head
	    if (memList->head->begAddress > cm->begAddress){
		cm->prev = NULL;
		cm->next = memList->head;
		memList->head->prev = cm;
		cm = cm->next;
		//memList->head->prev = cm;
		//memList->head = cm;
		memList->count++;
	    }
	    //When memList is at the last member of the list
	    if (currNode->next == NULL){
		//When cm has an eariler memory location than curr tail
		if (cm->begAddress < currNode->begAddress){
		    currNode->prev = cm;
		    currNode->next = NULL;
		    cm->next = currNode;
		    cm->prev = NULL;
		    memList->head = cm;
		    memList->count++;
		}
		//When cm has a later memory location than curr tail
		if (cm->begAddress > currNode->begAddress){
		    currNode->next = cm;
		    cm->prev = currNode;
		    cm->next = NULL;
		    memList->count++;
		}
	    }
	    //When cm will be in the middle of the list
	    if (currNode->begAddress < cm->begAddress){
		if (currNode->next->begAddress > cm->begAddress){
		    cm->next = currNode->next;
		    cm->prev = currNode;
		    currNode->next->prev = cm;
		    currNode->next = cm;
		    memList->count++;
		}
	    }
	    currNode = currNode->next;
	}
    }
}
//When a cmcb is freed this method checks if any surronding cmcbs are freed and if they are it merges
void merge(cmcb *a)
{
  if(a->next->type==0 && a->prev->type==0)
  {
 	a->prev->sizeOfBlock= a->prev->sizeOfBlock + a->next->sizeOfBlock+a->sizeOfBlock+(2*sizeof(cmcb))+(2*sizeof(lmcb));
	a=a->prev;
	a->next=a->next->next->next;

  }
  else if (a->next->type==0)
	{
		a->sizeOfBlock=a->sizeOfBlock+a->next->sizeOfBlock+sizeof(cmcb)+sizeof(lmcb);
		a->next=a->next->next;
	}
  else if (a->prev->type==0)
	{
		a->prev->sizeOfBlock=a->sizeOfBlock+a->next->sizeOfBlock+sizeof(cmcb)+sizeof(lmcb);
		a=a->prev;
		a->next=a->next->next;
	}
	else return;
}



