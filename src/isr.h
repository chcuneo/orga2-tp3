/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#ifndef __ISR_H__
#define __ISR_H__

#include "colors.h"
#include "screen.h"

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
    if (scanCode & 0x80) {
        // Release
    } else {
        // Press
        screen_pintar(scanCode, C_FG_WHITE | C_BG_RED, 0, VIDEO_COLS - 1);
    }
}

#endif  
