#include <system.h>
#include <string.h>
#include "PCB.h"
#include "sys_call.h"
#include "mpx_supt.h"
#include "MCB.h"


context* con;
extern param params;
pcb* cop;
u32int* sys_call(context* registers) {
    if(cop==NULL) {
        con=registers;

    }
    else {
        if(params.op_code == IDLE) {
            //set cop stack top to parameter registers
            cop->stackTop = (unsigned char*)registers;
		//maybe remove and insert should be here
		remove(cop);
		insert(cop);
        }
       else if(params.op_code == EXIT) {
            remove(cop);
            freePCB(cop);

        }


    }
    

    if(isReadyEmpty()== 0) {
        cop = ready->head;
        //display(cop);
        return (u32int*)cop->stackTop;
    }
    else {
        return (u32int*)con;
    }

}
