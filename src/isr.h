/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#ifndef __ISR_H__
#define __ISR_H__

#include "colors.h"
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
    "reserved",
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

uchar kbdMap[] = {};

void isr_keyboard(uchar scanCode) {
    //Cada caso puede ser una tecla definida en el keyboardcodes.h
    switch (scanCode) {
        case KBC_F_P:
            screen_pintar('F', C_FG_WHITE | C_BG_RED, 0, VIDEO_COLS - 1);
            break;
        case KBC_H_R:
            screen_pintar('H', C_FG_WHITE | C_BG_RED, 0, VIDEO_COLS - 1);
            break;
        default:
            break;
    }
}

int isr_syscall(uint operation, uint param) {
    uint ret = 0;

    // ID es el índice del pirata basado en 0
    uint id = (rtr() >> 3) - (GDT_IDX_TASKI_DESC + 1);

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

    return ret;
}

#endif
