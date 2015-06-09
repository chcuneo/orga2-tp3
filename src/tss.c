/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "defines.h"
#include "tss.h"
#include "game.h"
#include "error.h"
#include "gdt.h"

tss tss_idle = (tss) {
	.ptl = 0x0,
	.unused0 = 0x0,
	.esp0 = 0x0,
	.ss0 = GDT_OFF_DATA0_DESC,
	.unused1 = 0x0,
	.esp1 = 0x0,
	.ss1 = 0x0,
	.unused2 = 0x0,
	.esp2 = 0x0, // TODO: preguntar bien a que poner las pilas
	.ss2 = 0x0,
	.unused3 = 0x0,
	.cr3 = KERNEL_DIR_TABLE,
	.eip = 0x16000, // TODO: mover a un define
	.eflags = 0x0,
	.eax = 0x0,
	.ecx = 0x0,
	.edx = 0x0,
	.ebx = 0x0,
	.esp = 0x0,
	.ebp = 0x0,
	.esi = 0x0,
	.edi = 0x0,
	.es = 0x0,
	.unused4 = 0x0,
	.cs = GDT_OFF_CODE0_DESC,
	.unused5 = 0x0,
	.ss = GDT_OFF_DATA0_DESC,
	.unused6 = 0x0,
	.ds = GDT_OFF_DATA0_DESC,
	.unused7 = 0x0,
	.fs = 0x0,
	.unused8 = 0x0,
	.gs = 0x0,
	.unused9 = 0x0,
	.ldt = 0x0,
	.unused10 = 0x0,
	.dtrap = 0x0,
	.iomap = 0xFFFF
};

// TODO: verificar
tss tss_inicial = (tss) {
	.ptl = 0x0,
	.unused0 = 0x0,
	.esp0 = 0x0,
	.ss0 = 0x0,
	.unused1 = 0x0,
	.esp1 = 0x0,
	.ss1 = 0x0,
	.unused2 = 0x0,
	.esp2 = 0x0,
	.ss2 = 0x0,
	.unused3 = 0x0,
	.cr3 = 0x0,
	.eip = 0x0,
	.eflags = 0x0,
	.eax = 0x0,
	.ecx = 0x0,
	.edx = 0x0,
	.ebx = 0x0,
	.esp = 0x0,
	.ebp = 0x0,
	.esi = 0x0,
	.edi = 0x0,
	.es = 0x0,
	.unused4 = 0x0,
	.cs = 0x0,
	.unused5 = 0x0,
	.ss = 0x0,
	.unused6 = 0x0,
	.ds = 0x0,
	.unused7 = 0x0,
	.fs = 0x0,
	.unused8 = 0x0,
	.gs = 0x0,
	.unused9 = 0x0,
	.ldt = 0x0,
	.unused10 = 0x0,
	.dtrap = 0x0,
	.iomap = 0xFFFF
};

tss tss_jugadorA[MAX_CANT_PIRATAS_VIVOS];
tss tss_jugadorB[MAX_CANT_PIRATAS_VIVOS];

uint lastUninitializedGDTEntry;

int tss_gdt_inicializar(uint tssAddress) {
	if (lastUninitializedGDTEntry >= 256) {
		return E_GDT_LIMIT_REACHED;
	}

	gdt[lastUninitializedGDTEntry] = (gdt_entry) {
		(ushort)    0x0067,         /* limit[0:15]  */
		(ushort)    tssAddress & (0x0000FFFF),         /* base[0:15]   */
		(uchar)     (tssAddress & (0x00FF0000)) >> 16, /* base[23:16]  */
		(uchar)     0x09,           /* type         */
		(uchar)     0x00,           /* s            */
		(uchar)     0x01,           /* dpl          */
		(uchar)     0x01,           /* p            */
		(uchar)     0x00,           /* limit[16:19] */
		(uchar)     0x00,           /* avl          */
		(uchar)     0x00,           /* l            */
		(uchar)     0x00,           /* db           */
		(uchar)     0x00,           /* g            */
		(uchar)     (tssAddress & (0xFF000000)) >> 24, /* base[31:24]  */
	};

	lastUninitializedGDTEntry++;

	return E_OK;
}

int tss_gdt_inicializar_bache(uint tssBaseAddress, ushort size) {
	uint task;

	for (task = 0; task < size; ++task) {
		int code = tss_gdt_inicializar(tssBaseAddress + 0x68 * task);

		if (code != E_OK) {
			return code;
		}
	}

	return E_OK;
}

int tss_inicializar() {
	// Tarea inicial
	gdt[GDT_IDX_TASKB_DESC] = (gdt_entry) {
		(ushort)    0x0067,         /* limit[0:15]  */
		(ushort)    ((uint) &tss_inicial) & (0x0000FFFF),         /* base[0:15]   */
		(uchar)     (((uint) &tss_inicial) & (0x00FF0000)) >> 16, /* base[23:16]  */
		(uchar)     0x0B,           /* type         */
		(uchar)     0x00,           /* s            */
		(uchar)     0x00,           /* dpl          */
		(uchar)     0x01,           /* p            */
		(uchar)     0x00,           /* limit[16:19] */
		(uchar)     0x00,           /* avl          */
		(uchar)     0x00,           /* l            */
		(uchar)     0x00,           /* db           */
		(uchar)     0x00,           /* g            */
		(uchar)     (((uint) &tss_inicial) & (0xFF000000)) >> 24, /* base[31:24]  */
	},

	// Tarea idle
	gdt[GDT_IDX_TASKI_DESC] = (gdt_entry) {
		(ushort)    0x0067,         /* limit[0:15]  */
		(ushort)    ((uint) &tss_idle) & (0x0000FFFF),         /* base[0:15]   */
		(uchar)     (((uint) &tss_idle) & (0x00FF0000)) >> 16, /* base[23:16]  */
		(uchar)     0x09,           /* type         */
		(uchar)     0x00,           /* s            */
		(uchar)     0x00,           /* dpl          */
		(uchar)     0x01,           /* p            */
		(uchar)     0x00,           /* limit[16:19] */
		(uchar)     0x00,           /* avl          */
		(uchar)     0x00,           /* l            */
		(uchar)     0x00,           /* db           */
		(uchar)     0x00,           /* g            */
		(uchar)     (((uint) &tss_idle) & (0xFF000000)) >> 24, /* base[31:24]  */
	};

	lastUninitializedGDTEntry = GDT_IDX_TASKI_DESC + 1;

	uint code = tss_gdt_inicializar_bache((uint)&tss_jugadorA, MAX_CANT_PIRATAS_VIVOS);

	if (code != E_OK) {
		return code;
	}


	code = tss_gdt_inicializar_bache((uint)&tss_jugadorB, MAX_CANT_PIRATAS_VIVOS);

	if (code != E_OK) {
		return code;
	}

	#define TSS_ARRAY_PHYS 0x100000
	#define TSS_STACKS_PHYS (TSS_ARRAY_PHYS + 0x68 * 17)
	uint task;

	for (task = 0; task < 8; ++task) {
		tss_jugadorA[task] = (tss) {
			.ptl = 0x0,
			.unused0 = 0x0,
			.esp0 = TSS_STACKS_PHYS + PAGE_SIZE * (task + 1), // TODO: y donde esta la ebp0?
			.ss0 = GDT_OFF_DATA0_DESC, // TODO: verificar
			.unused1 = 0x0,
			.esp1 = 0x0,
			.ss1 = 0x0,
			.unused2 = 0x0,
			.esp2 = 0x0, // TODO: preguntar bien a que poner las pilas
			.ss2 = 0x0,
			.unused3 = 0x0,
			.cr3 = ALIGN(DIRECTORY_TABLE_PHYS + DIRECTORY_TABLE_ENTRY_SIZE * task),
			// La direccion del codigo virtual siempre es ahi
			.eip = 0x400000,
			.eflags = 0x0,
			.eax = 0x0,
			.ecx = 0x0,
			.edx = 0x0,
			.ebx = 0x0,
			// La pila la ponemos inicialmente en el final de la pagina reservada para el codigo
			.esp = 0x4FFFFF,
			.ebp = 0x4FFFFF,
			.esi = 0x0,
			.edi = 0x0,
			.es = 0x0,
			.unused4 = 0x0,
			.cs = GDT_OFF_CODE3_DESC,
			.unused5 = 0x0,
			.ss = GDT_OFF_DATA3_DESC,
			.unused6 = 0x0,
			.ds = GDT_OFF_DATA3_DESC,
			.unused7 = 0x0,
			.fs = 0x0,
			.unused8 = 0x0,
			.gs = 0x0,
			.unused9 = 0x0,
			.ldt = 0x0,
			.unused10 = 0x0,
			.dtrap = 0x0,
			.iomap = 0xFFFF
		};

		// TODO: terminar. Replicar para jugadorB
	}

	return E_OK;
}
