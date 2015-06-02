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

    // DECISION DE DISEÑO: 
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

    // DECISION DE DISEÑO: 
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

    // DECISION DE DISEÑO: 
    // Code segment para protección 0
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

    // DECISION DE DISEÑO: 
    // Data segment para protección 0
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
    }

};

gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (uint) &gdt
};
