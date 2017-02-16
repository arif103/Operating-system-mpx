#include "commandHandler.h"
#include <string.h>
#include "mem/heap.h"
#include <system.h>
#include "PCB.h"
#include "proc3.h"
#include "sys_call.h"
#include "MCB.h"


u32int *memStart;
void commandHandler() {

    char* pout = '\0';
    int choice;
    int stopper = 1;
    //initQueues();
    serial_println("Welcome to My MPX. What would you like to do? Press any key between 1-19");
    printOptions();
    while (stopper == 1) {
        pout = reader(pout);
        choice = atoi(pout);
        switch(choice) {
        case 1 :
            help();
	    sys_req(IDLE);
            enter();
            break;
        case 2 :
            setDate();
	    sys_req(IDLE);
            enter();
            break;
        case 3 :
            getDate();
	    sys_req(IDLE);
            enter();
            break;
        case 4 :
            setTime();
	    sys_req(IDLE);
            enter();
            break;
        case 5 :
            getTime();
	    sys_req(IDLE);
            enter();
            break;
        case 6 :
            version();
	    sys_req(IDLE);
            enter();
            break;
        case 7 :
            setPriority();
	    sys_req(IDLE);
            enter();
            break;
	case 8 :
            resume();
	    sys_req(IDLE);
            enter();
            break;
	case 9 :
            suspend();
	    sys_req(IDLE);
            enter();
            break;
        case 10 :
            showReady();
	    sys_req(IDLE);
            enter();
            break;
        case 11 :
            showBlocked();
	    sys_req(IDLE);
            enter();
            break;
	case 12 :
            showAll();
	    sys_req(IDLE);
            enter();
            break;
        case 13 :
            loadr3();
	        //yield();
	    sys_req(IDLE);
            enter();
            break;
        case 16 :
            stopper=2;
	//initQueues();
  	    sys_req(EXIT);
	//	ready->head=NULL;
            break;
        case 17 :
            printOptions();
            break;
	case  14:
  	   showAllocatedMemory();
		enter();
	    break;
	case 15 :
  	   showFreeMemory();
		enter();
	    break;
	case 18 :
  	   allocateMemory();
		enter();
	    break;
	case 19 :
  	   freeMemory1();
		enter();
	    break;
	
        default :
            serial_println("\nWrong input\n");
            enter();
            break;
        }

    }
}

char* reader(char * output) {
    char key, buff[2], buffCom[100];
    int i = 0, stop = 1,count=0;
    memset(output, 0 ,100);
    buff[1] = '\0';
    while (stop == 1) {
        if (inb(COM1+5)&1) {
            key = (char) inb(COM1);
            buff[0] = key;
            if (strcmp(buff, "\r") != 0) {
                buffCom[i++] = key;
            }
            serial_print(buff);
            if (buff[0] == 127) {
                serial_print(" \r");
                i = i - 2;
                if (i < 0) {
                    i = 0;
                }
                buffCom[i] = '\0';
                serial_print(buffCom);
            }
            /*
               else if(buff[0]== 27 )
            {
				
				inb(COM1);
				//serial_printl("#");
				buff[0]= inb(COM1);
				//inb(COM1);
				//key= inb(COM1);
				if(buff[0] =='D')
				{
					if(i != 0){
						serial_print(" \r");
						i=i-2;
						serial_print("\033[1D");
						//i--;
						
						
					}
					serial_print(buffCom);
					//serial_print("\033[D");
				}
				inb(COM1);
			}
			 */
				
			
               if(count == 99)
            {
				memset(buffCom, 0, 100);
                buff[0] = '\0';
                break;
			}
          //  else if ( buff[0] == 0x50)
           // {
			//	serial_print("*");
			//}
            else if (strcmp(buff, "\r") == 0) {
                stop = 2;
                buffCom[i] = '\0';
                int x = 0;
                while (buffCom[x] != '\0') {
                    output[x] = buffCom[x];
                    x++;
                }
                output[x] = '\0';
                memset(buffCom, 0, 100);
                buff[0] = '\0';
            }
            count++;
        }
        
    }
    return output;
}
//gets the time and displays it
void getTime() {
    outb(0x70,0x04);
    unsigned char hours= inb(0x71);
    bcdToDecimal(hours);
    serial_print(":");
    outb(0x70,0x02);
    unsigned char minutes=inb(0x71);
    bcdToDecimal(minutes);
    serial_print(":");
    outb(0x70,0x00);
    unsigned char seconds=inb(0x71);
    bcdToDecimal(seconds);
}
//gets the date and displays it
void getDate() {
    outb(0x70,0x08);
    unsigned char months= inb(0x71);
    bcdToDecimal(months);
    serial_print("/");
    outb(0x70,0x07);
    unsigned char days=inb(0x71);
    bcdToDecimal(days);
    serial_print("/");
    outb(0x70,0x09);
    unsigned char years=inb(0x71);
    bcdToDecimal(years);
}

void bcdToDecimal(unsigned char n) {
    int ones1, tens1;
    unsigned char tens = ((n & 0xF0) >> 4) *10;
    unsigned char ones = (n & 0x0F);
    tens1 = tens + 0x30;
    ones1 = ones + 0x30;
    while(tens1 > 57)
        tens1 = tens1 - 9;

    serial_print((const char *)&tens1);
    serial_print((const char *)&ones1);
}

unsigned char decimalToBcd(int n) {
    return ((n/10)*16)+(n%10);
}
//set the time
void setTime() {
    char *hr = NULL, *mn = NULL, *sn = NULL;
    int hours,minutes,seconds;
    while(1){
    serial_println("\nPlease enter hours: ");
    reader(hr);
    hours=atoi(hr);
    if(hours >=0 && hours<24)
    {
		break;
	}
	else
	{
		serial_print("\nWrong input for hours");
	}
}
    unsigned char h= decimalToBcd(hours);
    cli();
    outb(0x70, 0x04);
    outb(0x71, h);
    sti();
    while(1){
    serial_println("\nPlease enter minutes: ");
    reader(mn);
    minutes=atoi(mn);
    if(minutes>=0 && minutes<60)
    {
		break;
	}
	else
	{
		serial_print("\nWrong input for minutes");
	}
}
    unsigned char m= decimalToBcd(minutes);
    cli();
    outb(0x70, 0x02);
    outb(0x71, m);
    sti();
    while(1){
    serial_println("\nPlease enter seconds: ");
    reader(sn);
    seconds=atoi(sn);
    if(seconds>=0 && seconds<60){
		break;
	}
	else
	{
		serial_print("\nWrong input for seconds");
	}
}
	
    unsigned char s= decimalToBcd(seconds);
    cli();
    outb(0x70, 0x00);
    outb(0x71, s);
    sti();
}
//sets the date
void setDate() {
    char *mn = NULL, *da = NULL, *yr = NULL;
    int months,days,years;
    while(1){
    serial_println("\nPlease enter months: ");
    reader(mn);
    months=atoi(mn);
    if(months>0 && months<13)
    {
		break;
	}
	else
	{
		serial_print("\nWrong input for months");
	}
}
    unsigned char m= decimalToBcd(months);
    cli();
    outb(0x70, 0x08);
    outb(0x71, m);
    sti();
    while(1){
    serial_println("\nPlease enter days: ");
    reader(da);
    days=atoi(da);
    if(days>0 && days<=31)
    {
		if(months!=2){
		break;
}
else{
	serial_print("\nWrong input for days in February");
}

	}
	else
	{
		serial_print("\nWrong input for days");
	}
}
    unsigned char d= decimalToBcd(days);
    cli();
    outb(0x70, 0x07);
    outb(0x71, d);
    sti();
    serial_println("\nPlease enter years: ");
    reader(yr);
    years=atoi(yr);
    unsigned char y= decimalToBcd(years);
    cli();
    outb(0x70, 0x09);
    outb(0x71, y);
    sti();
}

void help() {
   serial_println("-----------Help-Guide-----------\n --------Set-Date-------- \n");
    serial_println("Set Date:  If you want to use the function set date press 3\n What this function does is it take in your input of the month, day, and year in that order seperated by spaces and it sets those into a repository and waits for the user to get them\n");
    serial_println("---------Get-Date-----------\n Get Date: If you want to get the date press 3\n Getting the date will simply give you the date that is currently set in the repository.  It gets this only when the user has previously set the date otherwise there would be no date to get.\n This command doesn't take any user input as all it is doing is giving back the date.\n ");
    serial_println("--------Set-Time-------\n Set Time: To the set the time in the main menu press 2.\n Set time will allow the user to set the time to what the user desires. You do this by putting in the parameters of hours, then minutes, then seconds\n This will save the time in a repository and keep it till the user asks for it\n ");
    serial_println("---------Get-Time-------\n Get Time:If you want to get the time press 5\n Get time will get the time that the user already set and give it back to the user.");
    serial_println("---------Version----------- \n Version: If you want to view the version press 6\n Version will tell you the current version of our OS and the date in which it was completed");
    serial_println("--------Suspend-------- \n Suspend: This function accepts a valid PCB and places it into a suspended state\n");
    serial_println("--------Resume--------\n Resume: This function accepts a valid PCB that is suspended and puts it into a unsuspended state\n");
    serial_println("--------Set Priority--------\n This function accepts a valid PCB and a valid Priority integer and sets the new priority\n");
    serial_println("--------Show All--------\n This function takes no parameters and simply displays all the information about all the current PCBs\n");
    serial_println("--------Show Ready--------\n This function will show all the current information about the PCBs that are in the ready queue.\n");
    serial_println("--------Show Blocked--------\n This function will show all the current information about the PCBs that are in the blocked queue\n");
    serial_println("---------loadr3----------\n This function loads all r3 processes and puts them in the suspended ready\n");
    serial_println("----Show Free Memory-----\n This function shows the user which addresses are free or not\n");
    serial_println("--Show Allocated Memory--\n This function shows the user which addresses are allocated or not\n");
    serial_println("--------Show Free-------\n This function will show all the current information about the PCBs that are in the blocked queue\n");
    serial_println("--------Shutdown-------\n Shutdown: This function does is safely exits the OS.\n");
}

void version() {
    serial_println("R6");
    serial_println("04/26/2016");
}

// Temporary Commands
void create() {
    char name1[100]="\0", class1[100]="\0";
    //priority1[100]="\0";
    char *name = NULL, *class = NULL, *priority1 = NULL;
    pcb* newNode;
    int priority = NULL;
    serial_println("Enter name");
    name = reader(name);
    strcpy(name1,name);
    name = '\0';
    serial_println("Enter class");
    class = reader(class);
    strcpy(class1, class);
    class = '\0';
    serial_println("Enter priority");
    priority1 = reader(priority1);
    priority = atoi(priority1);
    if (priority >= 0 && priority <= 9 && (readyContains(name1) == NULL) &&
            ((strcmp(class1, "system") == 0)|| (strcmp(class1, "application"))== 0)) {
        newNode = setup(name1,class1,priority);
        insert(newNode);
    }
    else
        serial_println("Invalid Input, PCB not created\n");
    priority = NULL;
}


void delete() {
    serial_println("Enter name");
    char* name = NULL;
    name = reader(name);
    pcb* found = find(name);
    remove(found);
}

void block() {
    char *name = NULL;
    serial_println("Enter PCB to Block");
    name = reader(name);
    pcb* found = find(name);
    if(found != NULL) {
        remove(found);
        found->stateA = 3;
        insert(found);
    }
    else {
        serial_println("Invalid Name");
    }
}

void unblock() {
    char *name = NULL;
    serial_println("Enter PCB to Unblock");
    name = reader(name);
    pcb* found = find(name);
    if (found != NULL) {
        remove(found);
        found->stateA = 1;
        insert(found);
    }
    else
        serial_println("Invalid Name");
}

//Permanent Commands

//finds a PCB and puts it into a suspended state
void suspend() {
    char *name = NULL;
    serial_println("Enter name");
    name = reader(name);
    pcb* found = find(name);
    if (found != NULL) {
        found->stateS = 0;
    }
    else
        serial_println("Invalid Input\n");
}
//sets the priorty of a pcb
void setPriority() {
    char *name = NULL, *num = NULL;
    int number;
    serial_println("Enter name");
    name = reader(name);
    pcb* found = find(name);
    serial_println("Enter the new priority");
    num = reader(num);
    number = atoi(num);
    if (found != NULL) {
        found->priority = number;
    }
    else
        serial_println("Invalid Inputs\n");
}
//finds a pcb and takes it out of a suspended state
void resume() {
    char *name = NULL;
    serial_println("Enter name");
    name = reader(name);
    pcb* found = find(name);
    if (found != NULL && found->stateS == 0) {
        found->stateS = 1;
    }
    else
        serial_println("Invalid Input\n");
}

void yield() {
    asm volatile ("int $60");
}

//loads in the procedures for R3
void loadr3() {
    //proc 1
    pcb *new_pcb1 = setup1("proc1" , 1, 1);
    context *cp1 = ( context*)(new_pcb1 ->stackTop);
    insert(new_pcb1);
    memset(cp1, 0,sizeof(context));
    cp1->fs = 0x10;
    cp1->gs = 0x10;
    cp1->ds = 0x10;
    cp1->es = 0x10;
    cp1->cs = 0x8;
    cp1->ebp = (u32int)(new_pcb1 ->stack);
    cp1->esp = (u32int)(new_pcb1 ->stackTop);
    cp1->eip = (u32int)proc1;
    //The function correlating to the process, ie. proc1
    cp1->eflags = 0x202;
    //proc 2
    struct pcb *new_pcb2 = setup1("proc2" , 1, 1);
    insert(new_pcb2);
    context *cp2 = ( context*)(new_pcb2 ->stackTop);
    memset(cp2, 0,sizeof(context));
    cp2->fs = 0x10;
    cp2->gs = 0x10;
    cp2->ds = 0x10;
    cp2->es = 0x10;
    cp2->cs = 0x8;
    cp2->ebp = (u32int)(new_pcb2 ->stack);
    cp2->esp = (u32int)(new_pcb2 ->stackTop);
    cp2->eip = (u32int)proc2;
    //The function correlating to the process, ie. proc2
    cp2->eflags = 0x202;
    //proc 3
    pcb *new_pcb3 = setup1("proc3" , 1, 1);
    insert(new_pcb3);
    context *cp3 = (context*)(new_pcb3 ->stackTop);
    memset(cp3, 0,sizeof(context));
    cp3->fs = 0x10;
    cp3->gs = 0x10;
    cp3->ds = 0x10;
    cp3->es = 0x10;
    cp3->cs = 0x8;
    cp3->ebp = (u32int)(new_pcb3 ->stack);
    cp3->esp = (u32int)(new_pcb3 ->stackTop);
    cp3->eip = (u32int)proc3;
    //The function correlating to the process, ie. proc3
    cp3->eflags = 0x202;
    //proc 4
    pcb *new_pcb4 = setup1("proc4" , 1, 1);
    insert(new_pcb4);
    context *cp4 = (context*)(new_pcb4 ->stackTop);
    memset(cp4, 0,sizeof(context));
    cp4->fs = 0x10;
    cp4->gs = 0x10;
    cp4->ds = 0x10;
    cp4->es = 0x10;
    cp4->cs = 0x8;
    cp4->ebp = (u32int)(new_pcb4 ->stack);
    cp4->esp = (u32int)(new_pcb4 ->stackTop);
    cp4->eip = (u32int)proc4;
    //The function correlating to the process, ie. proc4
    cp4->eflags = 0x202;
    //proc 5
   pcb *new_pcb5 = setup1("proc5" , 1, 1);
    insert(new_pcb5);
    context *cp5 = (context*)(new_pcb5 ->stackTop);
    memset(cp5, 0,sizeof(context));
    cp5->fs = 0x10;
    cp5->gs = 0x10;
    cp5->ds = 0x10;
    cp5->es = 0x10;
    cp5->cs = 0x8;
    cp5->ebp = (u32int)(new_pcb5 ->stack);
    cp5->esp = (u32int)(new_pcb5 ->stackTop);
    cp5->eip = (u32int)proc5;
    //The function correlating to the process, ie. proc5
    cp5->eflags = 0x202;
}

void initializeHeap()
{
    char *siz=NULL;
    serial_println("Enter initial heap size:");
    int size=atoi(reader(siz));
    memStart =(u32int *) kmalloc(size+sizeof(cmcb) + sizeof(lmcb)); //size=50000
    //put cmcb at the top of the heap and lmcb at the bottom
      cmcb *c=(cmcb *)memStart;
     lmcb *l = (lmcb*) memStart + sizeof(cmcb) + size;
     c->type=0; // 0 indicates free list but should I literally add it to the free list or just declare it as a type /?
     c->sizeOfBlock=size;
     c->next=NULL;
     c->prev=NULL;
     c->begAddress = memStart;
     l->type=0;
     l->sizeOfBlock=size;
    // queue free; //initialize your memory here!!!!
     //queue allocated
     //free1.head=NULL;
     memList->head=c;

     //free1.tail=NULL;

    // allocated.tail=NULL;

}
void initializeHeap1(int size)
{
	
    memStart =(u32int *) kmalloc(size+sizeof(cmcb) + sizeof(lmcb)); //size=50000
    //put cmcb at the top of the heap and lmcb at the bottom
	cmcb *c;
	lmcb *l;
      c=(cmcb *)memStart;
     l = (lmcb*) memStart + sizeof(cmcb) + size;
     c->type=0; // 0 indicates free list but should I literally add it to the free list or just declare it as a type /?
     c->sizeOfBlock=size;
     c->next=NULL;
     c->prev=NULL;
     c->begAddress = memStart;
     l->type=0;
     l->sizeOfBlock=size;
    // queue free; //initialize your memory here!!!!
     //queue allocated
     //free1.head=NULL;
     memList->head=c;
	//serial_println((cmcb *) c->sizeOfBlock);
     //free1.tail=NULL;

    // allocated.tail=NULL;

}


void allocateMemory()
{
     char *alloc=NULL;
	serial_println("Enter memory:");
     reader(alloc);
     int allocation=atoi(alloc);
	int currSize;

	cmcb* currNode = memList->head;

 	while (currNode!=NULL) {
    	if(currNode->sizeOfBlock >= (int) (allocation+sizeof(cmcb)+sizeof(lmcb)) && currNode->type == 0)
	{
 	currSize = currNode->sizeOfBlock;
	currNode->sizeOfBlock = allocation;
	currNode->type = 1;
	insertMCB(currNode);
    lmcb *newL =(lmcb *) currNode+allocation+sizeof(cmcb);
    cmcb *newC =(cmcb *) newL+sizeof(newL);
    newC->type = 0;
    newC->sizeOfBlock = currSize - allocation - sizeof(cmcb);
	newC->begAddress =currNode->begAddress +allocation +sizeof(cmcb) +sizeof(lmcb);
    insertMCB(newC);
    	return;
	}
	else
currNode = currNode->next;

}
    serial_println("No available memory");
  }



void freeMemory(u32int* c)
{
    cmcb *temp = memList->head;
    while(temp != NULL){
    if (c == temp->begAddress)
    {
        temp->type = 0;
        merge(temp);
    }
    else
        temp = temp->next;
    }
    }

void freeMemory1()
{
	char *address=NULL; 
	serial_println("Enter address:");
    reader(address);
	u32int *ad=(u32int *) atoi(address);
    cmcb *temp = memList->head;
    while(temp != NULL){
    if (ad == temp->begAddress)
    {
        temp->type = 0;
        merge(temp);
    }
    
        temp = temp->next;
    }
    }

    //search allocated list for the block
    //remove the block from the list and put it in freed
    //check to see the adajacent blocks and merge those blocks


//Shows all the free memory
void showFreeMemory()
{
   showFree();

}
//shows all allocated memory
void showAllocatedMemory()
{
    showAllocate();

}

/* A utility function to reverse a string  */
void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        char temp= *(str + start);
	*(str +start) =*(str+end);
	*(str+end)=temp;
        start++;
        end--;
    }
}

char* itoa(u32int num, char* str, u32int base)
{
    int i = 0;
 
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled only with 
    // base 10. Otherwise numbers are conssidered unsigned.
    
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
 
    
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
 
    return str;
}

void printOptions() {
    serial_println("1) Help\n2) Set Date\n3) Get Date\n4) Set time\n5) Get Time\n6) Version\n7) Set Priority\n8) Resume\n9) Suspend\n10) Show Ready\n11) Show Blocked\n 12) Show All\n13) Load R3\n14) Show Allocated\n15) Show Free\n16) Shutdown\n17)Show menu\n18) Allocate Memory\n19) Free memory\n  Enter choice: ");
}

void enter() {
    serial_println(" Enter Choice:  (To print options again enter '17') ");
}
