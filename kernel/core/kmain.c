/*
  ----- kmain.c -----

  Description..: Kernel main. The first function called after
      the bootloader. Initialization of hardware, system
      structures, devices, and initial processes happens here.
*/

#include <stdint.h>
#include <string.h>
#include <system.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/tables.h>
#include <core/interrupts.h>
#include <mem/heap.h>
#include <mem/paging.h>
#include "modules/PCB.h"
#include "modules/MCB.h"
#include <modules/sys_call.h>

#include <modules/mpx_supt.h>

#include "modules/commandHandler.h"

void kmain(void)
{
   extern uint32_t magic;
   // Uncomment if you want to access the multiboot header
   // extern void *mbd;
    //char *boot_loader_name = (char*)((long*)mbd)[16];

   // 0) Initialize Serial I/O and call mpx_init
   klogv("Starting MPX boot sequence...");
   klogv("Initialized serial I/O on COM1 device...");
   init_serial(COM1);
   set_serial_in(COM1);
   set_serial_out(COM1);
   mpx_init(MODULE_R1);
   

   // 1) Check that the boot was successful and correct when using grub
   // Comment this when booting the kernel directly using QEMU, etc.
   if ( magic != 0x2BADB002 ){
     //kpanic("Boot was not error free. Halting.");
   }

   // 2) Descriptor Tables
   klogv("Initializing descriptor tables...");
   init_gdt();
   init_idt();
   init_pic();
   init_irq();
   sti();

   // 4) Virtual Memory
   klogv("Initializing virtual memory...");
	//initializeHeap1(50000);
   init_paging();

   // 5) Call Command
   klogv("Transferring control to commhand...");


//while(1){
   initQueues();
   initLists();
	initializeHeap1(5000);
   pcb *cH = setup1("comH" ,"system", 1);
   context *commHand = ( context*)(cH ->stackTop);
   //insert(cH);
   memset(commHand, 0,sizeof(context));
   commHand->fs = 0x10;
   commHand->gs = 0x10;
   commHand->ds = 0x10;
   commHand->es = 0x10;
   commHand->cs = 0x8;
   commHand->ebp = (u32int)(cH ->stack);
   commHand->esp = (u32int)(cH ->stackTop);
   commHand->eip = (u32int)commandHandler;
   //The function correlating to the process, ie. proc1
   commHand->eflags = 0x202;

   pcb *idle1 = setup1("idle" ,"system", 1);
   context *idle2 = ( context*)(idle1 ->stackTop);
  // insert(idle1);
   memset(idle2, 0,sizeof(context));
   idle2->fs = 0x10;
   idle2->gs = 0x10;
   idle2->ds = 0x10;
   idle2->es = 0x10;
   idle2->cs = 0x8;
   idle2->ebp = (u32int)(idle1 ->stack);
   idle2->esp = (u32int)(idle1 ->stackTop);
   idle2->eip = (u32int)idle;
   //The function correlating to the process, ie. proc1
   idle2->eflags = 0x202;

   //showAll();

   insert(cH);
   insert(idle1);
   yield();

  // commandHandler();
  // //sys_req(IDLE);
   //commandHandler();

   // showAll();
   // 11) System Shutdown
   klogv("Starting system shutdown procedure...");

   /* Shutdown Procedure */
   klogv("Shutdown complete. You may now turn off the machine. (QEMU: C-a x)");
   hlt();
}
