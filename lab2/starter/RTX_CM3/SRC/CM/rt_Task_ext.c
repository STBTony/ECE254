/*----------------------------------------------------------------------------
 *      ECE254 Lab Task Management
 *----------------------------------------------------------------------------
 *      Name:    RT_TASK_ext.C
 *      Purpose: Interface functions for extended task management  
 *      Rev.:    V4.60
 *----------------------------------------------------------------------------
 *      This code extends the RealView Run-Time Library.
 *      Created by University of Waterloo ECE254 Lab Staff.
 *---------------------------------------------------------------------------*/

#include "rt_TypeDef.h"
#include "RTX_Config.h"
#include "rt_System.h"
#include "rt_Task.h"
#include "rt_List.h"
#include "rt_MemBox.h"
#include "rt_Robin.h"
#include "rt_HAL_CM.h"
#include "rt_Task_ext.h"

/*----------------------------------------------------------------------------
 *      Global Variables
 *---------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
 *      Local Functions
 *---------------------------------------------------------------------------*/
/*--------------------------- rt_tsk_count_get ------------------------------*/
/* added in ECE254 lab keil_rtx */
int rt_tsk_count_get (void) {
    int counter = 0;
    U32 i;
    for(i = 0; i <= os_maxtaskrun; i++){
        if(os_active_TCB[i] != NULL){
            counter++;
        }
    }
    if(os_idle_TCB.state != INACTIVE){
        counter++;
    }
	return counter;	
    
}

/*--------------------------- rt_tsk_get ------------------------------------*/
/* added in ECE254 lab keil_proc */
OS_RESULT rt_tsk_get (OS_TID task_id, RL_TASK_INFO *p_task_info) {
    P_TCB p;
    U32 stack_pointer, stack_top;//stack_bottom; 
    
    if (task_id > os_maxtaskrun || os_active_TCB[task_id-1] == NULL) {
        /* Task with "task_id" not found or not started. */
        return (OS_R_NOK);
    }
    p = os_active_TCB[task_id - 1];
    p_task_info->task_id = p->task_id;
    p_task_info->state = p->state; 
    p_task_info->prio = p->prio;  
    p_task_info->ptask = p->ptask; 
    //printf("\n%d\n%d", p->priv_stack, os_stackinfo);
    //p_task_info->stack_usage = (p->priv_stack * 100 / (U16)os_stackinfo);

    stack_pointer = p->priv_stack >> 2 != 0 ? p->priv_stack >> 2 : (U16)os_stackinfo >> 2;
    //stack_bottom = p->stack[stack_pointer];
    stack_top = (U32) p->tsk_stack;
    if(p->state == RUNNING)
        stack_top = rt_get_PSP();
    p_task_info->stack_usage = (U32)(((U32)&p->stack[stack_pointer] - stack_top)*100/(U16)os_stackinfo);
    
	return OS_R_OK;
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
