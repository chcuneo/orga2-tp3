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

#define CODIGO_BASE       0X40000

#define MAPA_BASE_FISICA  0x27000
#define MAPA_BASE_VIRTUAL 0x80000
#define MAPA_BASE_PAGE0	  0x28000

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

void mmu_inicializar();


#endif	/* !__MMU_H__ */
