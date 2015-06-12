/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#ifndef __ISR_H__
#define __ISR_H__

#include "defines.h"
#include "game.h"
#include "screen.h"
#include "error.h"
#include "keyboardcodes.h"

char *unrecoverableMsgs[] = {
    "Division by zero",
    "Debugger",
    "NMI",
    "Breakpoint",
    "Overflow",
    "Bounds",
    "Invalid Opcode",
    "Coprocessor not available",
    "Double fault",
    "Coprocessor Segment Overrun (386 or earlier only)",
    "Invalid Task State Segment",
    "Segment not present",
    "Stack Fault",
    "General protection fault",
    "Page fault",
    "Reserved",
    "Math Fault",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception"
};

void unrecoverableHandler(uint exception) {
    int line = 0;

    if (exception == 8) {
        line++;
    }

    print(unrecoverableMsgs[exception], line, 0, C_FG_RED | C_BLINK);
}

void isr_keyboard(uchar scanCode) {
    if (scanCode == KBC_LSHFT_P || scanCode == KBC_RSHFT_P){
        game_atender_teclado(scanCode);
    }
}

int isr_syscall(uint operation, uint param) {
    int ret = 0;

    // ID es el índice del pirata basado en 0
    uint id = rtr() >> 3;

    if (id >= GDT_IDX_START_TSKS) {
        id -= (GDT_IDX_START_TSKS);

        switch (operation) {
            case 0x1:
                ret = game_syscall_pirata_mover(id, param);
                break;
            case 0x2:
                ret = game_syscall_cavar(id);
                break;
            case 0x3:
                ret = game_syscall_pirata_posicion(id, param);
                break;
            default:
                ret = E_UNKNOWN_OPERATION;
                break;
        }
    } else {
        ret = E_UNKNOWN_OPERATION;
    }

    return ret;
}

#endif
