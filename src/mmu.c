/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"
/* Atributos paginas */
/* -------------------------------------------------------------------------- */


/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */

void mmu_inicializar() {
	page_entry *pageDirectory __attribute__((aligned(4096))) = (page_entry *)(MAPA_BASE_FISICA);
	page_entry *pageTable0 = (page_entry *)(MAPA_BASE_PAGE0);

	pageDirectory[0] = (page_entry) {
			/** Chris puto
			Aca guardamos solo los primeros 12 bits del address fisico de la tabla de paginas.
			Los siguientes 12 bits son obtenidos del address lineal que obtengamos en el acceso
			a memoria, y eso nos da el descriptor de pagina, que termina de determinarnos la
			direccion fisica.
			 */
			(unsigned char) 0x1,
			(unsigned char) 0x1,
			(unsigned char) 0x0,
			(unsigned char) 0x0,
			(unsigned char) 0x0,
			(unsigned char) 0x0,
			(unsigned char) 0x0,
			(unsigned char) 0x0,
			(unsigned char) 0x0,
			(unsigned char) 0x0,
			(unsigned int) MAPA_BASE_PAGE0 >> 12
		};

	page_entry def = (page_entry) {
			(unsigned char) 0x1,
			(unsigned char) 0x1,
			(unsigned char) 0x0,
			(unsigned char) 0x0,
			(unsigned char) 0x0,
			(unsigned char) 0x0,
			(unsigned char) 0x0,
			(unsigned char) 0x0,
			(unsigned char) 0x0,
			(unsigned char) 0x0,
			(unsigned int) 0x0 // Este valor lo vamos a reemplazar en cada iteracion
	};

	unsigned int offset = 0;
	long long x;

	// x/1024wx 0x28000
	for (x = 0; x < 1024; ++x) {
		pageTable0[x] = def;
		pageTable0[x].offset = (offset & 0xFFFFF) >> 12;
		offset += 4 * (1 << 10);
	}

	lcr3((unsigned int)pageDirectory);
	breakpoint();
}