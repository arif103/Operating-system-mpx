
#ifndef _PCB_H
#define _PCB_H

#include <core/serial.h>
#include <core/io.h>
#include "mpx_supt.h"



typedef struct pcb{
  char name[9];
  char class[12];
  int priority;
  int stateA;
  int stateS;

  unsigned char stack[1024];
 // unsigned char *stackBase;
  unsigned char *stackTop;
  struct pcb *next;
  struct pcb *prev;
} pcb;

typedef struct queue{
  int count;
  pcb *head;
  pcb *tail;
} queue;




queue *blocked;
queue *ready;



typedef struct context{
  u32int gs, fs, es, ds;
u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;
u32int eip, cs, eflags;
}context;


context *con;


pcb* allocate();
int freePCB();
pcb* setup();
pcb* setup1();
pcb* find();
void insert();
int remove();

//queue functions
void initQueues();

void readyEnq();
void readyDeq();
int readyQueueSize();
int isReadyEmpty();
pcb* readyContains();
void display(pcb*);
void blockedEnq();
void blockedDeq();
int blockedQueueSize();
int isBlockedEmpty();
pcb* blockedContains();
void showReady();
void showBlocked();

#endif
