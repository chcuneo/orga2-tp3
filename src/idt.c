
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
    IDT_ENTRY(0 , GDT_OFF_CODE0_DESC, 0x8E00);
    IDT_ENTRY(1 , GDT_OFF_CODE0_DESC, 0x8E00);
    IDT_ENTRY(2 , GDT_OFF_CODE0_DESC, 0x8E00);
    IDT_ENTRY(3 , GDT_OFF_CODE0_DESC, 0x8E00);
    IDT_ENTRY(4 , GDT_OFF_CODE0_DESC, 0x8E00);
    IDT_ENTRY(5 , GDT_OFF_CODE0_DESC, 0x8E00);
    IDT_ENTRY(6 , GDT_OFF_CODE0_DESC, 0x8E00);
    IDT_ENTRY(7 , GDT_OFF_CODE0_DESC, 0x8E00);
    IDT_ENTRY(8 , GDT_OFF_CODE0_DESC, 0x8E00);
    IDT_ENTRY(9 , GDT_OFF_CODE0_DESC, 0x8E00);
    IDT_ENTRY(10, GDT_OFF_CODE0_DESC, 0x8E00);
    IDT_ENTRY(11, GDT_OFF_CODE0_DESC, 0x8E00);
    IDT_ENTRY(12, GDT_OFF_CODE0_DESC, 0x8E00);
    IDT_ENTRY(13, GDT_OFF_CODE0_DESC, 0x8E00);
    IDT_ENTRY(14, GDT_OFF_CODE0_DESC, 0x8E00);
    IDT_ENTRY(15, GDT_OFF_CODE0_DESC, 0x8E00);
    IDT_ENTRY(16, GDT_OFF_CODE0_DESC, 0x8E00);
    IDT_ENTRY(17, GDT_OFF_CODE0_DESC, 0x8E00);
    IDT_ENTRY(18, GDT_OFF_CODE0_DESC, 0x8E00);
    IDT_ENTRY(19, GDT_OFF_CODE0_DESC, 0x8E00);
    IDT_ENTRY(20, GDT_OFF_CODE0_DESC, 0x8E00);

    IDT_ENTRY(32, GDT_OFF_CODE0_DESC, 0x8E00);
    IDT_ENTRY(33, GDT_OFF_CODE0_DESC, 0x8E00);

    IDT_ENTRY(70, GDT_OFF_CODE0_DESC, 0xEE00);
}
