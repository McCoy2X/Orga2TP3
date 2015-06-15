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
	agregar_descriptor_tss(TSS_INICIAL, &tss_init);
    agregar_descriptor_tss(TSS_IDLE, &tss_idle);

    agregar_descriptor_tss(TSS_JUGADORA_1, &tss_jugadorA[0]);
    agregar_descriptor_tss(TSS_JUGADORA_2, &tss_jugadorA[1]);
    agregar_descriptor_tss(TSS_JUGADORA_3, &tss_jugadorA[2]);
    agregar_descriptor_tss(TSS_JUGADORA_4, &tss_jugadorA[3]);
    agregar_descriptor_tss(TSS_JUGADORA_5, &tss_jugadorA[4]);
    agregar_descriptor_tss(TSS_JUGADORA_6, &tss_jugadorA[5]);
    agregar_descriptor_tss(TSS_JUGADORA_7, &tss_jugadorA[6]);
    agregar_descriptor_tss(TSS_JUGADORA_8, &tss_jugadorA[7]);

    agregar_descriptor_tss(TSS_JUGADORB_1, &tss_jugadorB[0]);
    agregar_descriptor_tss(TSS_JUGADORB_2, &tss_jugadorB[1]);
    agregar_descriptor_tss(TSS_JUGADORB_3, &tss_jugadorB[2]);
    agregar_descriptor_tss(TSS_JUGADORB_4, &tss_jugadorB[3]);
    agregar_descriptor_tss(TSS_JUGADORB_5, &tss_jugadorB[4]);
    agregar_descriptor_tss(TSS_JUGADORB_6, &tss_jugadorB[5]);
    agregar_descriptor_tss(TSS_JUGADORB_7, &tss_jugadorB[6]);
    agregar_descriptor_tss(TSS_JUGADORB_8, &tss_jugadorB[7]);

    tss_idle.esp      = 0x00027000;
    tss_idle.ebp      = 0x00027000;
    tss_idle.eip      = 0x00016000;
    tss_idle.cr3      = 0x00027000;
    tss_idle.es       = 0x0048;
    tss_idle.cs       = 0x0040;
    tss_idle.ss       = 0x0048;
    tss_idle.ds       = 0x0048;
    tss_idle.fs       = 0x0048;
    tss_idle.gs       = 0x0048;
    tss_idle.eflags   = 0x00000202;
    tss_idle.iomap    = 0xFFFF;
    tss_idle.esp0     = 0x00027000;
    tss_idle.ss0      = 0x0048;
}

void agregar_descriptor_tss(int indice, tss* puntero) {
    gdt[indice] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */
        (unsigned short)    (unsigned int)puntero,/* base[0:15]   */
        (unsigned char)     (unsigned int)puntero >> 16,  /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (unsigned int)puntero >> 24,  /* base[31:24]  */
    };
}

void completar_tabla_tss(tss tss_libre, void* codigo_tarea, int* posicion_cr3) {
    int pos_mapa;
    if(((int)codigo_tarea) < 0x120000) { pos_mapa = INICIO_PIRATAA; }
    else { pos_mapa = INICIO_PIRATAB; }
    int* cr3 = mmu_inicializar_dir_pirata(pos_mapa, codigo_tarea);

    tss_libre.esp      = 0x00400FF4;
    tss_libre.ebp      = 0x00401000;
    tss_libre.eip      = (int)codigo_tarea;
    tss_libre.cr3      = (int)cr3;
    tss_libre.es       = 0x0048;
    tss_libre.cs       = 0x0040;
    tss_libre.ss       = 0x0048;
    tss_libre.ds       = 0x0048;
    tss_libre.fs       = 0x0048;
    tss_libre.gs       = 0x0048;
    tss_libre.eflags   = 0x00000202;
    tss_libre.iomap    = 0xFFFF;
    tss_libre.esp0     = (int)mmu_get_pagina;
    tss_idle.ss0       = 0x0048;

    (*posicion_cr3) = (int)cr3;
}