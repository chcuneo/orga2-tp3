
/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "defines.h"
#include "idt.h"
#include "isr.h"
#include "tss.h"

idt_entry idt[255] = { };

idt_descriptor IDT_DESC = {
    sizeof(idt) - 1,
    (uint) &idt
};

#define IDT_ENTRY(numero, segselec, attrs)                                                                                        \
    idt[numero].offset_0_15 = (ushort) ((uint)(&_isr ## numero) & (uint) 0xFFFF);        \
    idt[numero].segsel = (ushort) ((ushort) segselec & (ushort) 0xFFFF);                                      \
    idt[numero].attr = (ushort) ((ushort) attrs & (ushort) 0xFFFF);                                          \
    idt[numero].offset_16_31 = (ushort) ((uint)(&_isr ## numero) >> 16 & (uint) 0xFFFF);

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
