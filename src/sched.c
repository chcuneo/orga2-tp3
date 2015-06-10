/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/

#include "sched.h"
#include "i386.h"
#include "error.h"

/** Number of scheduler ticks until a task switch is necessary
 */
#define SCHEDULER_TASK_TICKS 1000

/** Currently running task
 * Possible codes are:
 *  0 - Initial task
 *  1 - Idle task
 *  * - Pirate tasks
 *
 * Notice that if (current - 2) / MAX_CANT_PIRATAS_VIVOS == 0, then the task
 * belongs to player A. Otherwise, it belongs to player 2.
 */
uint current;

/** Scheduler tick
 * Whenever this value reaches 0, the scheduler will change tasks, doing a
 * context switch if necessary. Should there be a single task requiring
 * attention, nothing shall be done.
 */
uint tick;

/** Initializes the required data structures for the scheduler to work properly
 * @ret Error code
 * @error E_OK
 */
int scheduler_initialize() {
    current = 0;
    tick = 0;
    return E_OK;
}

/** Processes a single scheduler tick
 * @ret Error code
 * @error E_OK
 */
int scheduler_tick() {
    tick = tick + 1 % SCHEDULER_TASK_TICKS;

    if (tick == 0) {

    }

/*    str cx
    cmp ax, cx
    je .fin
        mov [selector], ax
        jmp far [offset]
.fin:*/
    return E_OK;
}
