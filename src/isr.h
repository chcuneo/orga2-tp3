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
#include "colors.h"
#include "keyboardcodes.h"
#include "tss.h"

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

uchar hasExceptionHappened = 0;

uint firstExceptionNumber;
tss firstException;

uint lastExceptionNumber;
tss lastException;

void loadRegisters(
    uint eip,
    uint ebp,
    uint esp,
    uint eflags) {

    uint eax = reax();

    if (!hasExceptionHappened) {
        firstException = (tss) {
            .ptl = 0x0,
            .unused0 = 0x0,
            .esp0 = rcr0(),
            .ss0 = 0x0,
            .unused1 = 0x0,
            .esp1 = rcr2(),
            .ss1 = 0x0,
            .unused2 = 0x0,
            .esp2 = rcr4(),
            .ss2 = 0x0,
            .unused3 = 0x0,
            .cr3 = rcr3(),
            .eip = eip,
            .eflags = eflags,
            .eax = eax,
            .ecx = recx(),
            .edx = redx(),
            .ebx = rebx(),
            .esp = esp,
            .ebp = ebp,
            .esi = resi(),
            .edi = redi(),
            .es = res(),
            .unused4 = 0x0,
            .cs = rcs(),
            .unused5 = 0x0,
            .ss = rss(),
            .unused6 = 0x0,
            .ds = rds(),
            .unused7 = 0x0,
            .fs = rfs(),
            .unused8 = 0x0,
            .gs = rgs(),
            .unused9 = 0x0,
            .ldt = 0x0,
            .unused10 = 0x0,
            .dtrap = 0x0,
            .iomap = 0x0
        };
    }

    lastException = (tss) {
        .ptl = 0x0,
        .unused0 = 0x0,
        .esp0 = rcr0(),
        .ss0 = 0x0,
        .unused1 = 0x0,
        .esp1 = rcr2(),
        .ss1 = 0x0,
        .unused2 = 0x0,
        .esp2 = rcr4(),
        .ss2 = 0x0,
        .unused3 = 0x0,
        .cr3 = rcr3(),
        .eip = eip,
        .eflags = eflags,
        .eax = eax,
        .ecx = recx(),
        .edx = redx(),
        .ebx = rebx(),
        .esp = esp,
        .ebp = ebp,
        .esi = resi(),
        .edi = redi(),
        .es = res(),
        .unused4 = 0x0,
        .cs = rcs(),
        .unused5 = 0x0,
        .ss = rss(),
        .unused6 = 0x0,
        .ds = rds(),
        .unused7 = 0x0,
        .fs = rfs(),
        .unused8 = 0x0,
        .gs = rgs(),
        .unused9 = 0x0,
        .ldt = 0x0,
        .unused10 = 0x0,
        .dtrap = 0x0,
        .iomap = 0x0
    };
}

void unrecoverableHandler(uint exception) {
    if (!hasExceptionHappened) {
        firstExceptionNumber = exception;
        hasExceptionHappened = 1;
    }

    lastExceptionNumber = exception;

    uint id = rtr() >> 3;

    if (id >= GDT_IDX_START_TSKS) {
        id -= GDT_IDX_START_TSKS;

        game_pirata_exploto(id);
    } else {
        screen_clear();
        print(unrecoverableMsgs[firstExceptionNumber], 0, 0, C_FG_WHITE);
    }
}

uchar pausedByDebugScreen = 0;

void isr_keyboard(uchar scanCode) {
    if (scanCode == KBC_LSHFT_P || scanCode == KBC_RSHFT_P) {
        game_atender_teclado(scanCode);
    } else if (scanCode == KBC_Y_R && BIT_ISSET(pausedByDebugScreen, 1) == 0) {
        pausedByDebugScreen = BIT_FLIP(pausedByDebugScreen, 0);
        screen_flip_debug(firstExceptionNumber, unrecoverableMsgs[firstExceptionNumber], &firstException);
        game_pause_toggle();
    } else if (scanCode == KBC_SPACE_R && BIT_ISSET(pausedByDebugScreen, 0) == 0) {
        pausedByDebugScreen = BIT_FLIP(pausedByDebugScreen, 1);
        screen_flip_debug(lastExceptionNumber, unrecoverableMsgs[lastExceptionNumber], &lastException);
        game_pause_toggle();
    }
}

int isr_syscall(uint operation, uint param) {
    int ret = 0;
    uint id = rtr() >> 3;

    if (id >= GDT_IDX_START_TSKS) {
        id -= GDT_IDX_START_TSKS;

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
