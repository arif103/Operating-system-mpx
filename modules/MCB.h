#ifndef _MCB_H
#define _MCB_H

#include <core/serial.h>
#include <core/io.h>
#include "mpx_supt.h"
#include "mem/heap.h"


//initializes the cmcb list
typedef struct cmcb{

  int type; //0: free 1: allocated
  //char* begAddress; // maybe not needed u32int pointer change!
  int sizeOfBlock;
  u32int *begAddress;
  struct cmcb *next;
  struct cmcb *prev;

} cmcb;

typedef struct lmcb{
  int type; //0: free 1: allocated
  int sizeOfBlock;
} lmcb;

typedef struct list{
  int count;
  cmcb *head;
} list;

list *memList;


void initMCB();
void initLists();
void showFree();
void showAllocate();
int isEmpty();
void merge();
void insertMCB();
void display1(cmcb *);

#endif
