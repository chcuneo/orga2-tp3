/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "game.h"

typedef struct _page_entry {
	uchar p:1;
	uchar rw:1;
	uchar us:1;
	uchar pwt:1;
	uchar pcd:1;
	uchar a:1;
	uchar d:1;
	uchar pat:1;
	uchar g:1;
	uchar ignored:3;
	uint offset:20;
} __attribute__((__packed__)) page_entry;

void mmu_inicializar_dir_kernel();
int mmu_inicializar_dir_pirata(uint directoryBase, uint pirateCodeBaseSrc, uint pirateCodeBaseDst);
int mmu_move_codepage(uint directoryBase, uint codeBaseSrc, uint codeBaseDst);
int isMapped(uint directoryBase, uint virtualAddress);
int remap(uint directoryBase, uint virtualAddress, uint physicalAddress);
int mmap(uint virtualAddress, uint physicalAddress, uint directoryBase, uchar readWrite, uchar userSupervisor);
int munmap(uint directoryBase, uint virtualAddress);

#endif
