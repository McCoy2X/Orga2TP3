/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "mmu.h"

tss tss_init;
tss tss_idle;

tss tss_jugadorA[8];
tss tss_jugadorB[8];

void tss_inicializar() {
	/* TSS Inicial */
    /* Offset = */
    gdt[TSS_INICIAL] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */
        (unsigned short)    (unsigned int)&tss_init,/* base[0:15]   */
        (unsigned char)     (unsigned int)&tss_init >> 16,	/* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (unsigned int)&tss_init >> 24,	/* base[31:24]  */
    };

    /* TSS Idle */
    /* Offset = */
    gdt[TSS_IDLE] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */
        (unsigned short)    (unsigned int)&tss_idle,   /* base[0:15]   */
        (unsigned char)     (unsigned int)&tss_idle >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (unsigned int)&tss_idle >> 24,        /* base[31:24]  */
    };

    /* TSS Tarea 1 Jugador 1 */
    /* Offset = */
    gdt[TSS_JUGADORA_1] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */
        (unsigned short)    (unsigned int)&tss_jugadorA[0],   /* base[0:15]   */
        (unsigned char)     (unsigned int)&tss_jugadorA[0] >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (unsigned int)&tss_jugadorA[0] >> 24,        /* base[31:24]  */
    };

    /* TSS Tarea 2 Jugador 1 */
    /* Offset = */
    gdt[TSS_JUGADORA_2] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */
        (unsigned short)    (unsigned int)&tss_jugadorA[1],   /* base[0:15]   */
        (unsigned char)     (unsigned int)&tss_jugadorA[1] >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (unsigned int)&tss_jugadorA[1] >> 24,        /* base[31:24]  */
    };

    /* TSS Tarea 3 Jugador 1 */
    /* Offset = */
    gdt[TSS_JUGADORA_3] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */
        (unsigned short)    (unsigned int)&tss_jugadorA[2],   /* base[0:15]   */
        (unsigned char)     (unsigned int)&tss_jugadorA[2] >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (unsigned int)&tss_jugadorA[2] >> 24,        /* base[31:24]  */
    };

    /* TSS Tarea 4 Jugador 1 */
    /* Offset = */
    gdt[TSS_JUGADORA_4] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */
        (unsigned short)    (unsigned int)&tss_jugadorA[3],   /* base[0:15]   */
        (unsigned char)     (unsigned int)&tss_jugadorA[3] >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (unsigned int)&tss_jugadorA[3] >> 24,        /* base[31:24]  */
    };

    /* TSS Tarea 5 Jugador 1 */
    /* Offset = */
    gdt[TSS_JUGADORA_5] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */
        (unsigned short)    (unsigned int)&tss_jugadorA[4],   /* base[0:15]   */
        (unsigned char)     (unsigned int)&tss_jugadorA[4] >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (unsigned int)&tss_jugadorA[4] >> 24,        /* base[31:24]  */
    };

    /* TSS Tarea 6 Jugador 1 */
    /* Offset = */
    gdt[TSS_JUGADORA_6] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */
        (unsigned short)    (unsigned int)&tss_jugadorA[5],   /* base[0:15]   */
        (unsigned char)     (unsigned int)&tss_jugadorA[5] >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (unsigned int)&tss_jugadorA[5] >> 24,        /* base[31:24]  */
    };

    /* TSS Tarea 7 Jugador 1 */
    /* Offset = */
    gdt[TSS_JUGADORA_7] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */
        (unsigned short)    (unsigned int)&tss_jugadorA[6],   /* base[0:15]   */
        (unsigned char)     (unsigned int)&tss_jugadorA[6] >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (unsigned int)&tss_jugadorA[6] >> 24,        /* base[31:24]  */
    };

    /* TSS Tarea 8 Jugador 1 */
    /* Offset = */
    gdt[TSS_JUGADORA_8] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */
        (unsigned short)    (unsigned int)&tss_jugadorA[7],   /* base[0:15]   */
        (unsigned char)     (unsigned int)&tss_jugadorA[7] >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (unsigned int)&tss_jugadorA[7] >> 24,        /* base[31:24]  */
    };

    /* TSS Tarea 1 Jugador 2 */
    /* Offset = */
    gdt[TSS_JUGADORB_1] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */
        (unsigned short)    (unsigned int)&tss_jugadorB[0],   /* base[0:15]   */
        (unsigned char)     (unsigned int)&tss_jugadorB[0] >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (unsigned int)&tss_jugadorB[0] >> 24,        /* base[31:24]  */
    };

    /* TSS Tarea 2 Jugador 2 */
    /* Offset = */
    gdt[TSS_JUGADORB_2] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */
        (unsigned short)    (unsigned int)&tss_jugadorB[1],   /* base[0:15]   */
        (unsigned char)     (unsigned int)&tss_jugadorB[1] >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (unsigned int)&tss_jugadorB[1] >> 24,        /* base[31:24]  */
    };

    /* TSS Tarea 3 Jugador 2 */
    /* Offset = */
    gdt[TSS_JUGADORB_3] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */
        (unsigned short)    (unsigned int)&tss_jugadorB[2],   /* base[0:15]   */
        (unsigned char)     (unsigned int)&tss_jugadorB[2] >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (unsigned int)&tss_jugadorB[2] >> 24,        /* base[31:24]  */
    };

    /* TSS Tarea 4 Jugador 2 */
    /* Offset = */
    gdt[TSS_JUGADORA_4] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */
        (unsigned short)    (unsigned int)&tss_jugadorB[3],   /* base[0:15]   */
        (unsigned char)     (unsigned int)&tss_jugadorB[3] >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (unsigned int)&tss_jugadorB[3] >> 24,        /* base[31:24]  */
    };

    /* TSS Tarea 5 Jugador 1 */
    /* Offset = */
    gdt[TSS_JUGADORB_5] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */
        (unsigned short)    (unsigned int)&tss_jugadorB[4],   /* base[0:15]   */
        (unsigned char)     (unsigned int)&tss_jugadorB[4] >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (unsigned int)&tss_jugadorB[4] >> 24,        /* base[31:24]  */
    };

    /* TSS Tarea 6 Jugador 1 */
    /* Offset = */
    gdt[TSS_JUGADORB_6] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */
        (unsigned short)    (unsigned int)&tss_jugadorB[5],   /* base[0:15]   */
        (unsigned char)     (unsigned int)&tss_jugadorB[5] >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (unsigned int)&tss_jugadorB[5] >> 24,        /* base[31:24]  */
    };

    /* TSS Tarea 7 Jugador 1 */
    /* Offset = */
    gdt[TSS_JUGADORB_7] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */
        (unsigned short)    (unsigned int)&tss_jugadorB[6],   /* base[0:15]   */
        (unsigned char)     (unsigned int)&tss_jugadorB[6] >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (unsigned int)&tss_jugadorB[6] >> 24,        /* base[31:24]  */
    };

    /* TSS Tarea 8 Jugador 1 */
    /* Offset = */
    gdt[TSS_JUGADORA_8] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */
        (unsigned short)    (unsigned int)&tss_jugadorB[7],   /* base[0:15]   */
        (unsigned char)     (unsigned int)&tss_jugadorB[7] >> 16,    /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (unsigned int)&tss_jugadorB[7] >> 24,        /* base[31:24]  */
    };

    tss_idle.esp      = 0x00027000;
    tss_idle.ebp      = 0x00027000;
    tss_idle.eip      = 0x00016000;
    tss_idle.cr3      = 0x00027000;
    tss_idle.es       = 0x00000048;
    tss_idle.cs       = 0x00000040;
    tss_idle.ss       = 0x00000048;
    tss_idle.ds       = 0x00000048;
    tss_idle.fs       = 0x00000048;
    tss_idle.gs       = 0x00000048;
    tss_idle.eflags   = 0x00000202;
    tss_idle.iomap    = 0xFFFF;
    tss_idle.esp0     = 0x00027000;

    tss_jugadorA[0].esp    = 0x00401000;
    tss_jugadorA[0].ebp    = 0x00401000;
    tss_jugadorA[0].eip    = 0x00400000;
    tss_jugadorA[0].cr3    =
    tss_jugadorA[0].es     =
    tss_jugadorA[0].cs     =
    tss_jugadorA[0].ss     =
    tss_jugadorA[0].ds     =
    tss_jugadorA[0].fs     =
    tss_jugadorA[0].gs     =
    tss_jugadorA[0].eflags =
    tss_jugadorA[0].iomap  =
    tss_jugadorA[0].esp0   =
    tss_jugadorA[0].ss0    =

// tss_idle = (tss) {
//     (unsigned short)    0x0000,     /*ptl;*/
//     (unsigned short)    0x0000,     /*unused0;*/
//     (unsigned int)      0x00027000,     /*esp0;*/
//     (unsigned short)    0x0000,         /*ss0;*/
//     (unsigned short)    0x0000,     /*unused1;*/
//     (unsigned int)      0x00000000, /*esp1;*/
//     (unsigned short)    0x0000,     /*ss1;*/
//     (unsigned short)    0x0000,     /*unused2;*/
//     (unsigned int)      0x00000000, /*esp2;*/
//     (unsigned short)    0x0000,     /*ss2;*/
//     (unsigned short)    0x0000,     /*unused3;*/
//     (unsigned int)      0x00027000,     /*cr3;*/
//     (unsigned int)      0x00016000, /*eip;*/
//     (unsigned int)      0x00000202,     /*eflags;*/
//     (unsigned int)      0x00000000, /*eax;*/
//     (unsigned int)      0x00000000, /*ecx;*/
//     (unsigned int)      0x00000000, /*edx;*/
//     (unsigned int)      0x00000000, /*ebx;*/
//     (unsigned int)      0x00027000,     /*esp;*/
//     (unsigned int)      0x00027000,     /*ebp;*/
//     (unsigned int)      0x00000000, /*esi;*/
//     (unsigned int)      0x00000000, /*edi;*/
//     (unsigned short)    0x0048,         /*es;*/
//     (unsigned short)    0x0000,     /*unused4;*/
//     (unsigned short)    0x0040,         /*cs;*/
//     (unsigned short)    0x0000,     /*unused5;*/
//     (unsigned short)    0x0048,         /*ss;*/
//     (unsigned short)    0x0000,     /*unused6;*/
//     (unsigned short)    0x0048,         /*ds;*/
//     (unsigned short)    0x0000,     /*unused7;*/
//     (unsigned short)    0x0048,         /*fs;*/
//     (unsigned short)    0x0000,     /*unused8;*/
//     (unsigned short)    0x0048,         /*gs;*/
//     (unsigned short)    0x0000,     /*unused9;*/
//     (unsigned short)    0x0000,     /*ldt;*/
//     (unsigned short)    0x0000,     /*unused10;*/
//     (unsigned short)    0x0000,     /*dtrap;*/
//     (unsigned short)    0xFFFF,     /*iomap;*/
//    };

}