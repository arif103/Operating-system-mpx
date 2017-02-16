#ifndef _COMMANDHANDLER_H
#define _COMMANDHANDLER_H

#include <core/serial.h>
#include <core/io.h>
#include "mpx_supt.h"



void commandHandler();
void printOptions();
void enter();
unsigned char decimalToBcd(int n);
void bcdToDecimal(unsigned char n);
char* reader();
void getTime();
void getDate();
void setTime();
void setDate();
void version();
void help();
void create();
void delete();

void block();
void unblock();
void suspend();
void setPriority();
void resume();
void show();
void showReady();
void showBlocked();
void showAll();
void yield();
void loadr3();
void initializeHeap();
void allocateMemory();
int isEmpty();
void freeMemory();
void freeMemory1();
void showFreeMemory();
void showAllocatedMemory();
void initializeHeap1();
void reverse(char [], int );
char* itoa(u32int, char* , u32int );

#endif
