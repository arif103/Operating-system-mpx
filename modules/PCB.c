#include <system.h>
#include "PCB.h"
#include "commandHandler.h"
#include "mpx_supt.h"
#include <string.h>
#include "sys_call.h"
#define pcbMem 1024







//Allocates memory for a new PCB
pcb* allocate(){
  pcb *ptr = sys_alloc_mem(sizeof(pcb));
  if (!ptr)
    return NULL;
  return ptr;
}

//Frees memory associated with a given PCB
int freePCB(pcb * ptr){
  if (sys_free_mem(ptr) != -1)
    return 0;
  else
    return 1;
}

//Will call allocate() to create an empty PCB and initialize PCB information
pcb *setup(char* name, char* class, int priority){
  pcb *new = allocate();
  new->next = NULL;
  new->prev = NULL;
  strcpy(new->name, name);
  strcpy(new->class, class);
  new->priority = priority;
  new->stateA = 1;
  new->stateS = 2;
  new->stackTop=new->stack + 1024 - sizeof(struct context);
  return new;
}


pcb *setup1(char* name, char* class, int priority){
  pcb *new = allocate();
  new->next = NULL;
  new->prev = NULL;
  strcpy(new->name, name);
  strcpy(new->class, class);
  new->priority = priority;
  new->stateA = 1;
  new->stateS = 1;
  new->stackTop=new->stack + 1024 - sizeof(struct context);
  return new;
}

//Searces all queues for a process with a given name
pcb* find(char * name){
  pcb *found;
  if (blockedContains(name) != NULL){
    found = blockedContains(name);
    return found;
  }
  else if (readyContains(name) != NULL) {
    found = readyContains(name);
    return found;
  }
  else
    return NULL;
}

//Inserts a PCB into the appropriate queue
void insert(pcb * node){
  if (node->stateA == 1){
    readyEnq(node);
  }
  else if (node->stateA == 3)
    blockedEnq(node);
}

//Removes a PCB from the queue in which it is currently stored
int remove(pcb *node){
  //whem the node is being removed from the ready queue
  if (node->stateA == 1){
    //when the node being removed is the tail of the ready queue
    if (node->prev == NULL && node->next != NULL){
      ready->tail = node->next;//sets ready tail ptr node to the next node
      ready->tail->prev = NULL;//sets ready tails prev ptr to null
      node->next = NULL;//sets nodes next ptr to null
      node->prev = NULL;//sets nodes prev ptr to null
      ready->count--;
    }
    //when the node being removed is the head of the ready queue
    else if (node->next == NULL){
      ready->head = node->prev;
      ready->head->next = NULL;
      node->next = NULL;
      node->prev = NULL;
      ready->count--;
    }
    //when the node being removed is the only node in the ready queue
    else if (ready->head != NULL && ready->tail == NULL){
      ready->head = NULL;
      ready->tail = NULL;
      node->next = NULL;
      node->prev = NULL;
      ready->count--;
    }
    //when the node being removed is neither the head or tail of the ready queue
    else {
      (*node).next->prev = node->prev;//sets next nodes prev ptr to currNodes prev ptr
      (*node).prev->next = node->next;//sets prev nodes next ptr to currNodes next ptr
      ready->count--;
    }
  }
  //when the node is being removed from the blocked queue
  else if (node->stateA == 3){
    //when the node is being removed is the tail of the blocked queue
    if (node->prev == NULL&& node->next != NULL) {
      blocked->tail = node->next;
      blocked->tail->prev = NULL;
      node->next = NULL;
      node->prev = NULL;
      blocked->count--;
    }
    //when the node being removed is the head of the blocked queue
    else if (node->next == NULL) {
      blocked->head = node->prev;
      blocked->head->next = NULL;
      node->next = NULL;
      node->prev = NULL;
      blocked->count--;
    }
    //when the node being removed is the only node in the blocked queue
    else if (blocked->head != NULL && blocked->tail == NULL){
      blocked->head = NULL;
      node->next = NULL;
      node->prev = NULL;
      blocked->count--;
    }
    //when the node being remobed is neither the head or tail of the ready queue
    else {
      (*node).next->prev = node->prev;
      (*node).prev->next = node->next;
      blocked->count--;
    }
  }
  else
    return 0;
  return 1;
}

//Queue Commands
void initQueues(){

  ready=sys_alloc_mem(sizeof(queue));

  ready->count = 0;
  ready->head = NULL;
  ready->tail = NULL;

  blocked=sys_alloc_mem(sizeof(queue));
  blocked->count = 0;
  blocked->head = NULL;
  blocked->tail = NULL;

  con = sys_alloc_mem(sizeof(struct context));

  //cop = NULL;

}

//Queue pcb in ready queue based on priority order, highest priority at head
void readyEnq(pcb * node){
  //This is when the ready queue is empty
  if(ready->head == NULL && ready->tail == NULL){
    ready->head = node;
    ready->count++;
  }
  //This is when the ready queue has exaclty one member
  else if (ready->count == 1){
    //When node to be queued has a lower or equal priority to head node
    if (ready->head->priority >= node->priority){
      ready->tail = node;//sets tail to new node
      ready->head->prev = node;//sets header nodes prev to new node
      ready->tail->next = ready->head;//sets tails next to header node
      ready->count++;
    }
    //When node to be queued has a higher priority than head node
    else {
      node->prev = ready->head;//sets new nodes prev ptr to current head
      ready->head->next = node;//sets head nodes next ptr to new node
      ready->tail = ready->head;//sets tail to previous header node
      ready->head = node;//sets header node to new node
      ready->tail->next = node;//set tails next ptr to new node
      ready->tail->prev = NULL;//set tails prev ptr to NULL
      ready->count++;
    }
  }
  //This is when the ready queue has two or more members
  else if (ready->count > 1 && ready->head != NULL && ready->tail != NULL){
    int priority = node->priority;
    //When new node has higher priority than header node
    if (priority > ready->head->priority) {
      node->next = NULL;//sets new nodes next ptr to NULL
      node->prev = ready->head;//sets new nodes prev ptr to current header node
      ready->head->next = node;//sets header nodes prev ptr to new node
      ready->head = node;//sets new node to header node
      ready->count++;
    }
    //When new node has lower or equal priority than tail node
    else if (priority <= ready->tail->priority) {
      node->next = ready->tail;//sets new nodes next ptr to current tail node
      node->prev = NULL;//sets new nodes prev ptr to NULL
      ready->tail->prev = node;//sets current tail nodes prev ptr to new node
      ready->tail = node;//sets tail node to new node
      ready->count++;
    }
    else {
      pcb *currNode = ready->head->prev;
      //Iterates through queue until another node with less or equal priority is ecountered
      while(currNode != NULL){
	//when new nodes priority is equal to the currNodes priority and the
	//currNode is not the tail
	if (priority == currNode->priority && currNode->next != NULL){
	  (*currNode).prev->next = node;//set prev nodes next ptr to new node
	  node->prev = currNode->prev;//set new nodes prev ptr to prev node
	  currNode->prev = node;//set currNodes prev ptr to new node
	  node->next = currNode;//set new nodes next ptr to currNode
	  ready->count++;
	  break;
	}
	//when new nodes priority is greater that the currNodes priority
	else if (priority > (*currNode).priority){
	  (*currNode).next->prev = node;//set prev nodes next ptr to new node
	  node->next = (*currNode).next;//set new nodes next ptr to next node
	  node->prev = currNode;//set new nodes prev ptr to currNode
	  currNode->next = node;
	  ready->count++;
	  break;
	}
	currNode = currNode->prev;
      }
    }
  }
}

//Dequeues the head node from the ready queue
void readyDeq(){
  pcb *temp = ready->head;
  temp = temp->next;
  (*temp).prev = NULL;
  ready->head = temp;
  ready->count--;
}

//Returns the size of the ready queue
int readyQueueSize(){
  return ready->count;
}

//Returns true(1) if the ready queue is empty otherwise returns false(0)
int isReadyEmpty(){
  if(ready->head == NULL)
    return 1;
  else
    return 0;
}

//Returns the pointer of the pcb if it is found in the ready queue otherwise null
pcb * readyContains(char * name){
  pcb* currNode = ready->head;
  while(currNode){
    if(strcmp(name, currNode->name) == 0){
      return currNode;
    }
    currNode = currNode->prev;
  }
  return NULL;
}

//Queue pcb in blocked queue stored in first in first out order
void blockedEnq(pcb * node){
  //When blocked queue is empty
  if (blocked->head == NULL && blocked->tail == NULL){
    blocked->head = node;//sets header node to new node
    blocked->tail = NULL;//sets tail node to null
    node->next = NULL;//sets new nodes next ptr to null
    node->prev = NULL;//sets new nodes prev ptr to null
    blocked->count++;
  }
  //When blocked queue only has one member
  else if (blocked->count == 1){
    blocked->head->prev = node;//sets header nodes prev ptr to new node
    node->next = blocked->head;//sets new nodes next ptr to header node
    node->prev = NULL;//sets new nodes prev ptr to null
    blocked->tail = node;//sets tail node to new node
    blocked->count++;
  }
  //When blocked queue has more than two members
  else{
    blocked->tail->prev = node;//sets tail node prev ptr to new node
    node->next = blocked->tail;//sets new nodes next ptr to tail node
    node->prev = NULL;
    blocked->tail = node;
    blocked->count++;
  }
}
//Pushes out the head of the queue
void blockedDeq(){
  pcb *temp = blocked->head;
  temp = temp->next;
  temp->prev = NULL;
  blocked->head = temp;
  blocked->count--;
}
//Returns the size of the block queue
int blockedQueueSize(){
  return blocked->count;
}
//Checks if the blocked queue is empty or not
int isBlockedEmpty(){
  if(blocked->count == 0)
    return 1;
  else
    return 0;
}
//Checks the blocked queue for a certain pcb and if so returns it
pcb* blockedContains(char * name){
  pcb* currNode = blocked->head;
  while (currNode){
    if (strcmp(name, currNode->name) == 0)
      return currNode;
    currNode = currNode->prev;
  }
  return NULL;
}
//Checks for a node and displays all of the information about that node
void display(pcb * node){
  char priority[2];
  serial_print("Name: ");
  serial_println((*node).name);
  serial_print("Class: ");
  serial_println((*node).class);
  serial_print("Priority: ");
  priority[0] =(char) (*node).priority + 48;
  priority[1] = '\0';
  serial_println(priority);
  serial_print("Status: ");
  if((*node).stateA == 1){
    serial_println("Ready");
  }
  else if((*node).stateA == 3){
    serial_println("Blocked");
  }
  else
    serial_println("Running");
  serial_print("Status: ");
  if((*node).stateS == 1){
    serial_println("Suspended");
  }
  else
    serial_println("Not suspended");
  serial_println("");
}
//Gets a pcb and then displays all the information about it
void show(){
  char *name = NULL;
  serial_println("Enter name");
  name = reader(name);
  display(ready->head);
  pcb *found = find(name);
  display(found);
}
//Displays all the information for the nodes in the ready queue
void showReady(){
  serial_println("--Ready Queue Members--");
  serial_println("");
  pcb* currNode = ready->head;
  while (currNode) {
    display(currNode);
    currNode = currNode->prev;
  }
}
//Displays all the information for the nodes in the blocked queue
void showBlocked(){
  serial_println("--Blocked Queue Members--");
  serial_println("");
  pcb* currNode = blocked->head;
  while (currNode) {
    display(currNode);
    currNode = currNode->prev;
  }
}
//Shows all the information for both ready and blocked
void showAll(){
  showReady();
  showBlocked();
}
