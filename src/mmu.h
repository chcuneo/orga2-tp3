/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "defines.h"
#include "game.h"

/**
Aca guardamos solo los primeros 12 bits del address fisico de la tabla de paginas.
Los siguientes 12 bits son obtenidos del address lineal que obtengamos en el acceso
a memoria, y eso nos da el descriptor de pagina, que termina de determinarnos la
direccion fisica.
 */
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
void mmu_inicializar();
int mmu_inicializar_dir_pirata(uint directoryBase, uint pirateCodeBaseSrc, uint pirateCodeBaseDst);
void mmu_move_codepage(uint srcv, uint dstp, pirata_t *p);

int mmap(uint virtualAddress, uint physicalAddress, uint directoryBase, uchar readWrite, uchar userSupervisor);
int munmap(uint directoryBase, uint virtualAddress);

#endif	/* !__MMU_H__ */
