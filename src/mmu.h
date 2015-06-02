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
	unsigned char p:1;
	unsigned char rw:1;
	unsigned char us:1;
	unsigned char pwt:1;
	unsigned char pcd:1;
	unsigned char a:1;
	unsigned char d:1;
	unsigned char pat:1;
	unsigned char g:1;
	unsigned char ignored:3;
	unsigned int offset:20;
} __attribute__((__packed__)) page_entry;

void mmu_inicializar_dir_kernel();
void mmu_inicializar();

int mmap(
	unsigned int virtualAddress,
	unsigned int physicalAddress,
	unsigned int directoryBase,
	unsigned char readWrite,
	unsigned char userSupervisor);
int munmap(
	unsigned int directoryBase,
	unsigned int virtualAddress);

#endif	/* !__MMU_H__ */
