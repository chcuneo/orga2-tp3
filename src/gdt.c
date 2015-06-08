/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"

/* Definicion de la GDT */
/* -------------------------------------------------------------------------- */

gdt_entry gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] = (gdt_entry) {
        (ushort)    0x0000,         /* limit[0:15]  */
        (ushort)    0x0000,         /* base[0:15]   */
        (uchar)     0x00,           /* base[23:16]  */
        (uchar)     0x00,           /* type         */
        (uchar)     0x00,           /* s            */
        (uchar)     0x00,           /* dpl          */
        (uchar)     0x00,           /* p            */
        (uchar)     0x00,           /* limit[16:19] */
        (uchar)     0x00,           /* avl          */
        (uchar)     0x00,           /* l            */
        (uchar)     0x00,           /* db           */
        (uchar)     0x00,           /* g            */
        (uchar)     0x00,           /* base[31:24]  */
    },

    // Code segment para protección 0
    [GDT_IDX_CODE0_DESC] = (gdt_entry) {
        (ushort)    0xF3FF,         /* limit[0:15]  */
        (ushort)    0x0000,         /* base[0:15]   */
        (uchar)     0x00,           /* base[23:16]  */
        (uchar)     0x0A,           /* type         */
        (uchar)     0x01,           /* s            */
        (uchar)     0x00,           /* dpl          */
        (uchar)     0x01,           /* p            */
        (uchar)     0x01,           /* limit[16:19] */
        (uchar)     0x00,           /* avl          */
        (uchar)     0x00,           /* l            */
        (uchar)     0x01,           /* db           */
        (uchar)     0x01,           /* g            */
        (uchar)     0x00,           /* base[31:24]  */
    },

    // Data segment para protección 0
    [GDT_IDX_DATA0_DESC] = (gdt_entry) {
        (ushort)    0xF3FF,         /* limit[0:15]  */
        (ushort)    0x0000,         /* base[0:15]   */
        (uchar)     0x00,           /* base[23:16]  */
        (uchar)     0x02,           /* type         */
        (uchar)     0x01,           /* s            */
        (uchar)     0x00,           /* dpl          */
        (uchar)     0x01,           /* p            */
        (uchar)     0x01,           /* limit[16:19] */
        (uchar)     0x00,           /* avl          */
        (uchar)     0x00,           /* l            */
        (uchar)     0x01,           /* db           */
        (uchar)     0x01,           /* g            */
        (uchar)     0x00,           /* base[31:24]  */
    },

    // Code segment para protección 3
    [GDT_IDX_CODE3_DESC] = (gdt_entry) {
        (ushort)    0xF3FF,         /* limit[0:15]  */
        (ushort)    0x0000,         /* base[0:15]   */
        (uchar)     0x00,           /* base[23:16]  */
        (uchar)     0x0A,           /* type         */
        (uchar)     0x01,           /* s            */
        (uchar)     0x03,           /* dpl          */
        (uchar)     0x01,           /* p            */
        (uchar)     0x01,           /* limit[16:19] */
        (uchar)     0x00,           /* avl          */
        (uchar)     0x00,           /* l            */
        (uchar)     0x01,           /* db           */
        (uchar)     0x01,           /* g            */
        (uchar)     0x00,           /* base[31:24]  */
    },

    // Data segment para protección 3
    [GDT_IDX_DATA3_DESC] = (gdt_entry) {
        (ushort)    0xF3FF,         /* limit[0:15]  */
        (ushort)    0x0000,         /* base[0:15]   */
        (uchar)     0x00,           /* base[23:16]  */
        (uchar)     0x02,           /* type         */
        (uchar)     0x01,           /* s            */
        (uchar)     0x03,           /* dpl          */
        (uchar)     0x01,           /* p            */
        (uchar)     0x01,           /* limit[16:19] */
        (uchar)     0x00,           /* avl          */
        (uchar)     0x00,           /* l            */
        (uchar)     0x01,           /* db           */
        (uchar)     0x01,           /* g            */
        (uchar)     0x00,           /* base[31:24]  */
    },

    // DECISION DE DISEÑO: 
    // Data segment para protección 0
    [GDT_IDX_VIDEO_DESC] = (gdt_entry) {
        (ushort)    0xFFFF,         /* limit[0:15]  */
        (ushort)    0x0000,         /* base[0:15]   */
        (uchar)     0x0A,           /* base[23:16]  */
        (uchar)     0x02,           /* type         */
        (uchar)     0x01,           /* s            */
        (uchar)     0x00,           /* dpl          */
        (uchar)     0x01,           /* p            */
        (uchar)     0x02,           /* limit[16:19] */
        (uchar)     0x00,           /* avl          */
        (uchar)     0x00,           /* l            */
        (uchar)     0x01,           /* db           */
        (uchar)     0x01,           /* g            */
        (uchar)     0x0A,           /* base[31:24]  */
    },

    /**
     * Aca comienzan las 18 tareas que soportamos
     */

    // Tarea inicial
    [GDT_IDX_TASKB_DESC] = (gdt_entry) {
        (ushort)    0x0067,         /* limit[0:15]  */
        (ushort)    0x0000,         /* base[0:15]   */
        (uchar)     0x00,           /* base[23:16]  */
        (uchar)     0x0B,           /* type         */
        (uchar)     0x00,           /* s            */
        (uchar)     0x00,           /* dpl          */
        (uchar)     0x01,           /* p            */
        (uchar)     0x00,           /* limit[16:19] */
        (uchar)     0x00,           /* avl          */
        (uchar)     0x00,           /* l            */
        (uchar)     0x00,           /* db           */
        (uchar)     0x00,           /* g            */
        (uchar)     0x10,           /* base[31:24]  */
    },

    // Tarea idle
    [GDT_IDX_TASKI_DESC] = (gdt_entry) {
        (ushort)    0x0067,         /* limit[0:15]  */
        (ushort)    0x0068,         /* base[0:15]   */
        (uchar)     0x00,           /* base[23:16]  */
        (uchar)     0x09,           /* type         */
        (uchar)     0x00,           /* s            */
        (uchar)     0x00,           /* dpl          */
        (uchar)     0x01,           /* p            */
        (uchar)     0x00,           /* limit[16:19] */
        (uchar)     0x00,           /* avl          */
        (uchar)     0x00,           /* l            */
        (uchar)     0x00,           /* db           */
        (uchar)     0x00,           /* g            */
        (uchar)     0x10,           /* base[31:24]  */
    }

    // Desde el indice 15 a 32 (incluido) estan reservados para la tss
};

gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (uint) &gdt
};
