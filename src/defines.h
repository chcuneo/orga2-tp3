/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Tipos basicos */
/* -------------------------------------------------------------------------- */
#define NULL                    0
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;

/* Constantes basicas */
/* -------------------------------------------------------------------------- */
#define PAGE_SIZE               0x1000
#define TASK_SIZE               0x1000

/**
 * Number of page tables within a directory table
 */
#define DIRECTORY_TABLE_ENTRY_SIZE 0x1000

/**
 * Number of pages within a page table
 */
#define PAGE_TABLE_SIZE 0x1000

#define BOOTSECTOR              0x00001000 /* direccion fisica de comienzo del bootsector (copiado) */
#define KERNEL                  0x00001200 /* direccion fisica de comienzo del kernel */

/*******************************************************************************
 * GAME CONFIG
 ******************************************************************************/

/**
 * Maximum tasks per player
 */
#define MAX_CANT_PIRATAS_VIVOS           8

/*******************************************************************************
 * MEMORY LAYOUT CONFIG
 ******************************************************************************/

/**
 * Kernel directory table's physical address
 */
#define KERNEL_DIR_TABLE  0x27000

/**
 * Kernel's memory address to map 4MB from.
 */
#define KERNEL_PAGE0	  0x28000

/**
 * Virtual address containing a page of code for every task we create.
 */
#define CODIGO_BASE       0x400000

/**
 * Virtual address containing the base address of the game map for every task
 * we create. This should work as a pointer to a matrix of pages.
 */
#define MAPA_BASE_VIRTUAL 0x800000

/**
 * Map's physical address. Should keep the same structure as the physical map.
 */
#define MAPA_BASE_FISICA  0x500000

/**
 * Physical address of the directory tables for pirate tasks
 */
#define DIRECTORY_TABLE_PHYS (ALIGN(0x3FFFFF - MAX_CANT_PIRATAS_VIVOS * 2 * DIRECTORY_TABLE_ENTRY_SIZE))

/**
 * Physical address of the level 0 stacks for pirate tasks
 */
#define TSS_STACKS_PHYS 0x100000

/*******************************************************************************
 * GDT CONFIG
 ******************************************************************************/

/**
 * Number of entries in the GDT
 */
#define GDT_COUNT (9 + 5 + MAX_CANT_PIRATAS_VIVOS*2 + 1)

/**
 * Indices of segments within the GDT
 */
#define GDT_IDX_NULL_DESC           0
#define GDT_IDX_CODE0_DESC          8
#define GDT_IDX_DATA0_DESC          9
#define GDT_IDX_CODE3_DESC          10
#define GDT_IDX_DATA3_DESC          11
#define GDT_IDX_VIDEO_DESC          12
#define GDT_IDX_TASKB_DESC          13
#define GDT_IDX_TASKI_DESC          14
#define GDT_IDX_START_TSKS          15

/**
 * Offsets of segments within the GDT, 0 based.
 */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC      << 3)
#define GDT_OFF_CODE0_DESC          (GDT_IDX_CODE0_DESC     << 3)
#define GDT_OFF_DATA0_DESC          (GDT_IDX_DATA0_DESC     << 3)
#define GDT_OFF_CODE3_DESC          (GDT_IDX_CODE3_DESC     << 3)
#define GDT_OFF_DATA3_DESC          (GDT_IDX_DATA3_DESC     << 3)
#define GDT_OFF_VIDEO_DESC          (GDT_IDX_VIDEO_DESC     << 3)
#define GDT_OFF_TASKB_DESC          (GDT_IDX_TASKB_DESC     << 3)
#define GDT_OFF_TASKI_DESC          (GDT_IDX_TASKI_DESC     << 3)

/*******************************************************************************
 * USEFUL MACROS
 ******************************************************************************/
#define ASSERT(x) while(!(x)) {};

#define BIT_ISSET(x, i) (x & (0x1 << (i)))
#define BIT_SET(x, i) (x | (1 << (i)))
#define BIT_UNSET(x, i) (x & !(1 << (i)))
#define BIT_FLIP(x, i) (x ^ (1 << (i)))
#define BIT_SIZE(h, w) (h * w / 8)

#define MIN(x, y) ((x < y)? x:y)
#define MAX(x, y) ((y < x)? x:y)
#define ALIGN(x) ((x) & 0xFFFFF000)

#endif
