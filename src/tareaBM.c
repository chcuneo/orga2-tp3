/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "colors.h"
#include "defines.h"
#include "game.h"
#include "syscall.h"

void task(int x_target, int y_target) {
    /* Tarea */
    /*lecx(x_target);
    lebx(y_target);
    breakpoint();*/
    uint pos = syscall_posicion(-1);
    int x = pos & 0xFF;
    int y = pos >> 8;
    int i;

    x_target = 70;
    y_target = 40;

    for(i=x; i != x_target; i++)
        syscall_mover(IZQ);

    for(i=y; i > y_target; i++)
        syscall_mover(ARR);

    while(1) {syscall_cavar();}

  while(1) { __asm __volatile("mov $2, %%eax":::"eax"); }
}