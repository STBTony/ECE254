/*----------------------------------------------------------------------------
 *      ECE254 Lab Task Management
 *----------------------------------------------------------------------------
 *      Name:    RT_MEMBOX_ext.C
 *      Purpose: Interface functions for blocking 
 *               fixed memory block management system
 *      Rev.:    V4.60
 *----------------------------------------------------------------------------
 *      This code is extends the RealView Run-Time Library.
 *      Created by University of Waterloo ECE254 Lab Staff.
 *---------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 *      Includes
 *---------------------------------------------------------------------------*/
 
#include "rt_TypeDef.h"
#include "RTX_Config.h"
#include "rt_System.h"
#include "rt_MemBox.h"
#include "rt_HAL_CM.h"
#include "rt_Task.h"       /* added in ECE254 lab keil_proc */ 
#include "rt_MemBox_ext.h" /* added in ECE254 lab keil_proc */ 
#include "rt_List.h"

/* ECE254 Lab Comment: You may need to include more header files */

/*----------------------------------------------------------------------------
 *      Global Variables
 *---------------------------------------------------------------------------*/
struct OS_XCB queue;

/*----------------------------------------------------------------------------
 *      Global Functions
 *---------------------------------------------------------------------------*/

/*==========================================================================*/
/*  The following are added for ECE254 Lab Task Management Assignmet       */
/*==========================================================================*/

/*---------------- rt_alloc_box_s, task blocks when out of memory-----------*/

/**  
   @brief: Blocking memory allocation routine.
 */
void *rt_alloc_box_s (void *p_mpool) {
	
	void *ptr = rt_alloc_box (p_mpool);
    if (ptr == NULL){
        int tid = rt_tsk_self();
        P_TCB a = os_active_TCB[tid - 1];
        rt_put_prio(&queue, a);
        rt_block(0xffff, 10);
        return NULL;     
    }
	return ptr;
}


/*----------- rt_free_box_s, pair with _s memory allocators ----------------*/
/**
 * @brief: free memory pointed by ptr, it will unblock a task that is waiting
 *         for memory.
 * @return: OS_R_OK on success and OS_R_NOK if ptr does not belong to gp_mpool 
 */
OS_RESULT rt_free_box_s (void *p_mpool, void *box) {
    int mem = rt_free_box(p_mpool, box);
    P_TCB a;
    if(mem == 0){
        if((queue).p_lnk != NULL){
            a = rt_get_first(&queue);
            a-> ret_val = (U32)box;
            rt_dispatch(a);
        }
    }
    else {
        return (OS_R_NOK);

    }
	return (OS_R_OK);
}
/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
