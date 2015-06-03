/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "defines.h"
#include "game.h"
#include "mmu.h"
#include "i386.h"
#include "error.h"
#include "screen.h" // TODO: borrar
/* Atributos paginas */
// /* -------------------------------------------------------------------------- */

/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */

/**
 * All of our functions will operate depending on where the table directory
 * is located. This is intimately related to the task that is currently running.
 * The cr3 register will hold the page directory address.
 */

/**
 *
 */
uint pageTableLastAddress;

/**
 * Creates a page table at the specified page directory with some given attributes.
 *
 * @param directoryBase page directory address, 4K aligned
 * @param directoryEntry table index within the page directory
 * @param physicalAddress in-memory location of the page table, 4K aligned
 * @ret E_OUT_OF_BOUNDS, E_PAGE_TABLE_PRESENT, E_OK
 */
int create_page_table(
	uint directoryBase,
	uint directoryEntry,
	uint physicalAddress,
	uchar readWrite,
	uchar userSupervisor) {

	if (directoryEntry >= 1024) {
		return E_OUT_OF_BOUNDS;
	}

	page_entry *pageDirectory __attribute__((aligned(4096))) = (page_entry *)directoryBase;

	if (pageDirectory[directoryEntry].p == 1) {
		return E_PAGE_TABLE_PRESENT;
	}

	pageDirectory[directoryEntry] = (page_entry) {
			(uchar) 0x1,
			(uchar) readWrite,
			(uchar) userSupervisor,
			(uchar) 0x0,
			(uchar) 0x0,
			(uchar) 0x0,
			(uchar) 0x0,
			(uchar) 0x0,
			(uchar) 0x0,
			(uchar) 0x0,
			(uint) physicalAddress >> 12
		};

	// Limpiamos la cache del procesador
	tlbflush();

	return E_OK;
}

/**
 * Deletes any given page table.
 *
 * @param directoryBase page directory address, 4K aligned
 * @param directoryEntry table index within the page directory
 * @ret E_OUT_OF_BOUNDS, E_PAGE_TABLE_MISSING, E_OK
 */
int delete_page_table(
	uint directoryBase,
	uint directoryEntry) {

	if (directoryEntry >= 1024) {
		return E_OUT_OF_BOUNDS;
	}

	page_entry *pageDirectory __attribute__((aligned(4096))) = (page_entry *)directoryBase;

	if (pageDirectory[directoryEntry].p == 0) {
		return E_PAGE_TABLE_MISSING;
	}

	uint physicalAddress = pageDirectory[directoryEntry].offset << 12;
	pageDirectory[directoryEntry].p = 0;

	page_entry *pageTable = (page_entry *)physicalAddress;

	int i;

	for (i = 0; i < 1024; ++i) {
		pageTable[i].p = 0;
	}

	// Limpiamos la cache del procesador
	tlbflush();

	return E_OK;
}

/**
 * Creates a page at a given page directory and table, with some specified
 * attributes.
 *
 * @param directoryBase page directory address, 4K aligned
 * @param directoryEntry table index within the page directory
 * @param tableEntry page index within the page table
 * @param physicalAddress in-memory location of the page table, 4K aligned
 * @param readWrite whether the page should have write permission
 * @param userSupervisor whether the page protection level should be supervisor
 * @ret E_OUT_OF_BOUNDS, E_PAGE_TABLE_MISSING, E_PAGE_PRESENT, E_OK
 */
int create_page(
	uint directoryBase,
	uint directoryEntry,
	uint tableEntry,
	uint physicalAddress,
	uchar readWrite,
	uchar userSupervisor) {

	if (directoryEntry >= 1024 || tableEntry >= 1024) {
		return E_OUT_OF_BOUNDS;
	}

	page_entry *pageDirectory __attribute__((aligned(4096))) = (page_entry *)directoryBase;

	if (pageDirectory[directoryEntry].p == 0) {
		return E_PAGE_TABLE_MISSING;
	}

	uint pageTableAddress = pageDirectory[directoryEntry].offset << 12;
	page_entry *pageTable = (page_entry *)pageTableAddress;

	if (pageTable[tableEntry].p == 1) {
		return E_PAGE_PRESENT;
	}

	pageTable[tableEntry] = (page_entry) {
			/**
			Aca guardamos solo los primeros 12 bits del address fisico de la tabla de paginas.
			Los siguientes 12 bits son obtenidos del address lineal que obtengamos en el acceso
			a memoria, y eso nos da el descriptor de pagina, que termina de determinarnos la
			direccion fisica.
			 */
			(uchar) 0x1,
			(uchar) readWrite,
			(uchar) userSupervisor,
			(uchar) 0x0,
			(uchar) 0x0,
			(uchar) 0x0,
			(uchar) 0x0,
			(uchar) 0x0,
			(uchar) 0x0,
			(uchar) 0x0,
			(uint) physicalAddress >> 12
		};

	// Limpiamos la cache del procesador
	tlbflush();

	return E_OK;
}


/**
 * Deletes the specified page.
 *
 * @param directoryBase page directory address, 4K aligned
 * @param directoryEntry table index within the page directory
 * @param tableEntry page index within the page table
 * @ret E_OUT_OF_BOUNDS, E_PAGE_TABLE_MISSING, E_PAGE_MISSING, E_OK
 */
int delete_page(
	uint directoryBase,
	uint directoryEntry,
	uint tableEntry) {

	if (directoryEntry >= 1024 || tableEntry >= 1024) {
		return E_OUT_OF_BOUNDS;
	}

	page_entry *pageDirectory __attribute__((aligned(4096))) = (page_entry *)directoryBase;

	if (pageDirectory[directoryEntry].p == 0) {
		return E_PAGE_TABLE_MISSING;
	}

	uint pageTableAddress = pageDirectory[directoryEntry].offset << 12;
	page_entry *pageTable = (page_entry *)pageTableAddress;

	if (pageTable[tableEntry].p == 0) {
		return E_PAGE_MISSING;
	}

	pageTable[tableEntry].p = 0;

	// Limpiamos la cache del procesador
	tlbflush();

	return E_OK;
}

/**
 * Maps a virtual address into a physical address given a base address for the
 * page directory offset. Should the page table for this address not exist, we
 * create it.
 *
 * As of the current implementation, whenever we create a new page table, we
 * just place it in linear order beginning from the address MAPA_BASE_TABLA
 * begins. Whenever we map a new page table, we just put it next to the
 * last defined page table.
 */

int mmap(
	uint virtualAddress,
	uint physicalAddress,
	uint directoryBase,
	uchar readWrite,
	uchar userSupervisor) {

	// Obtenemos las partes relevantes del address virtual
	uint directoryEntry = virtualAddress >> 22;
	uint tableEntry = (virtualAddress >> 12) & 0x3FF;

	// Alineamos el address fisico al tamano de pagina
	physicalAddress = (physicalAddress / PAGE_SIZE) * PAGE_SIZE;

	// Creamos (o no, si ya existe) la page table que vamos a necesitar
	int output = create_page_table(
		directoryBase,
		directoryEntry,
		pageTableLastAddress,
		readWrite,
		userSupervisor);

	if (output == E_OK) {
		pageTableLastAddress -= 4096;
	}

	// Creamos la pagina que mapea como queremos
	output = create_page(
		directoryBase,
		directoryEntry,
		tableEntry,
		physicalAddress,
		readWrite,
		userSupervisor);

	if (output != E_OK) {
		return E_INVALID_ADDRESS;
	} else {
		return E_OK;
	}
}

/**
 * Unmaps the page corresponding to a virtual address
 *
 * @param directoryBase page directory address, 4K aligned
 * @param virtualAddress address to remove from pagination
 * @ret E_INVALID_ADDRESS, E_OK
 */
int munmap(
	uint directoryBase,
	uint virtualAddress) {

	uint directoryEntry = virtualAddress >> 22;
	uint tableEntry = (virtualAddress >> 12) & 0x3FF;

	int output = delete_page(directoryBase, directoryEntry, tableEntry);

	if (output != E_OK) {
		return E_INVALID_ADDRESS;
	} else {
		return E_OK;
	}
}

#define KERNEL_DIR_TABLE  0x27000
#define KERNEL_PAGE0	  0x28000

void mmu_inicializar_dir_kernel() {
	create_page_table(KERNEL_DIR_TABLE, 0, KERNEL_PAGE0, 1, 1);

	uint offset = 0;
	long long x;

	// x/1024wx 0x28000
	for (x = 0; x < 1024; ++x) {
		create_page(KERNEL_DIR_TABLE, 0, x, offset, 1, 1);
		offset += PAGE_SIZE;
	}

	lcr3((uint)KERNEL_DIR_TABLE);
}

#define CODIGO_BASE       0x400000
#define MAPA_BASE_FISICA  0x500000
#define MAPA_BASE_VIRTUAL 0x800000

/**
 * @param directoryBase page directory address, 4K aligned
 * @param pirateCodeBaseSrc virtual address of the pirate code
 * @param pirateCodeBaseDst physical address to map the address CODIGO_BASE to
 */
void mmu_inicializar_dir_pirata(uint directoryBase, uint pirateCodeBaseSrc, uint pirateCodeBaseDst) {
	// TODO: mapear las partes del mapa ya descubiertas
	// Armamos el identity mapping
	uint offset = 0;
	long long x;

	for (x = 0; x < 1024; ++x) {
		// La memoria del kernel la ponemos como user y en read
		mmap(offset, offset, directoryBase, 0, 1);
		offset += PAGE_SIZE;
	}

	// Mapeamos el codigo del pirata
	mmap(CODIGO_BASE, pirateCodeBaseDst, directoryBase, 1, 0);

	// Cargamos el cr3 nuevo y backupeamos el nuestro
	offset = rcr3();
	lcr3(directoryBase);

	// Copiamos el codigo del pirata
	int y;
	int *src = (int *)pirateCodeBaseSrc;
	int *dst = (int *)CODIGO_BASE;

	for (y = 0; y < 1024; ++y) {
		*dst = *src;
		++src;
		++dst;
	}

	// Volvemos al cr3 anterior
	lcr3(offset);
}

#define ALIGN(x) (x/PAGE_SIZE) * PAGE_SIZE
#define DIRECTORY_TABLE_PHYS (ALIGN(0x3FFFFF) - 16 * PAGE_SIZE)

void mmu_inicializar() {
	pageTableLastAddress = DIRECTORY_TABLE_PHYS - 4096;
}