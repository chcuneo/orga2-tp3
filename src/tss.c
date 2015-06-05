/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "mmu.h"


tss tss_inicial;
tss tss_idle;

tss tss_jugadorA[MAX_CANT_PIRATAS_VIVOS];
tss tss_jugadorB[MAX_CANT_PIRATAS_VIVOS];


#define TSS_ARRAY_PHYS 0x100000
#define TSS_STACKS_PHYS (TSS_ARRAY_PHYS + 0x68 * 17)

void gdt_tsd_inicializar() {
	uint task;

	for (task = 0; task < 16; ++task) {
		uint address = TSS_ARRAY_PHYS + 0x68 * task;

		GDT_DESC[task + GDT_IDX_TASKI_DESC + 1] = (gdt_entry) {
				(ushort)    0x0067,         /* limit[0:15]  */
				(ushort)    address & (0x0000FFFF),         /* base[0:15]   */
				(uchar)     (address & (0x00FF0000)) >> 16, /* base[23:16]  */
				(uchar)     0x09,           /* type         */
				(uchar)     0x00,           /* s            */
				(uchar)     0x01,           /* dpl          */
				(uchar)     0x00,           /* p            */
				(uchar)     0x00,           /* limit[16:19] */
				(uchar)     0x00,           /* avl          */
				(uchar)     0x00,           /* l            */
				(uchar)     0x00,           /* db           */
				(uchar)     0x00,           /* g            */
				(uchar)     (address & (0xFF000000)) >> 24, /* base[31:24]  */
			};
	}
}

void tss_inicializar() {
	uint task;
	tss *tssDirectory = (tss *)TSS_ARRAY_PHYS;

	for (task = 0; task < 16; ++task) {
		uint address = TSS_STACKS_PHYS + PAGE_SIZE * (task + 1);
		tssDirectory[task].cs = GDT_OFF_CODE3_DESC;
		tssDirectory[task].ds = GDT_OFF_DATA3_DESC;
		tssDirectory[task].ss = GDT_OFF_DATA3_DESC;
		tssDirectory[task].esp0 = address;
	}
}