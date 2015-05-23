
/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "defines.h"
#include "idt.h"
#include "isr.h"
#include "colors.h"
#include "screen.h"
#include "tss.h"

idt_entry idt[255] = { };

idt_descriptor IDT_DESC = {
    sizeof(idt) - 1,
    (unsigned int) &idt
};

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

    if (exception != 8) {
        clear();
    } else {
        line++;
    }

    print(unrecoverableMsgs[exception], line, 1, C_FG_RED);
    debug_screen();

}

#define IDT_ENTRY(numero, segselec, attrs)                                                                                        \
    idt[numero].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## numero) & (unsigned int) 0xFFFF);        \
    idt[numero].segsel = (unsigned short) ((unsigned short) segselec & (unsigned short) 0xFFFF);                                      \
    idt[numero].attr = (unsigned short) ((unsigned short) attrs & (unsigned short) 0xFFFF);                                          \
    idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xFFFF);

void idt_inicializar() {
    // Excepciones del procesador
    IDT_ENTRY(0, 0x40, 0x8E00);
    IDT_ENTRY(1, 0x40, 0x8E00);
    IDT_ENTRY(2, 0x40, 0x8E00);
    IDT_ENTRY(3, 0x40, 0x8E00);
    IDT_ENTRY(4, 0x40, 0x8E00);
    IDT_ENTRY(5, 0x40, 0x8E00);
    IDT_ENTRY(6, 0x40, 0x8E00);
    IDT_ENTRY(7, 0x40, 0x8E00);
    IDT_ENTRY(8, 0x40, 0x8E00);
    IDT_ENTRY(9, 0x40, 0x8E00);
    IDT_ENTRY(10, 0x40, 0x8E00);
    IDT_ENTRY(11, 0x40, 0x8E00);
    IDT_ENTRY(12, 0x40, 0x8E00);
    IDT_ENTRY(13, 0x40, 0x8E00);
    IDT_ENTRY(14, 0x40, 0x8E00);
    IDT_ENTRY(15, 0x40, 0x8E00);
    IDT_ENTRY(16, 0x40, 0x8E00);
    IDT_ENTRY(17, 0x40, 0x8E00);
    IDT_ENTRY(18, 0x40, 0x8E00);
    IDT_ENTRY(19, 0x40, 0x8E00);
    IDT_ENTRY(20, 0x40, 0x8E00);
}
