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
#define PAGE_SIZE               0x00001000
#define TASK_SIZE               4096

#define BOOTSECTOR              0x00001000 /* direccion fisica de comienzo del bootsector (copiado) */
#define KERNEL                  0x00001200 /* direccion fisica de comienzo del kernel */


/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT 30

#define GDT_IDX_NULL_DESC           0
#define GDT_IDX_CODE0_DESC			8
#define GDT_IDX_DATA0_DESC			9
#define GDT_IDX_CODE3_DESC			10
#define GDT_IDX_DATA3_DESC			11
#define GDT_IDX_VIDEO_DESC			12

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC      << 3)
#define GDT_OFF_CODE0_DESC			(GDT_IDX_CODE0_DESC 	<< 3)
#define GDT_OFF_DATA0_DESC			(GDT_IDX_DATA0_DESC		<< 3)
#define GDT_OFF_CODE3_DESC			(GDT_IDX_CODE3_DESC 	<< 3)
#define GDT_OFF_DATA3_DESC			(GDT_IDX_DATA3_DESC		<< 3)
#define GDT_OFF_VIDEO_DESC			(GDT_IDX_VIDEO_DESC		<< 3)
/* Selectores de segmentos */
/* -------------------------------------------------------------------------- */

void* error();

#define ASSERT(x) while(!(x)) {};


#endif  /* !__DEFINES_H__ */