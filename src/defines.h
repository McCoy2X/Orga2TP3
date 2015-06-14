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
#define DIR_PAGINAS_KERNEL  	0x027000
#define TASK_SIZE               4096

#define MEM_MANAGER				0x00100000
#define TSS_IDLE_DATA			0x00100064
#define PAGE_COPY				0x00101000
#define MAPA 					0x00500000

#define BOOTSECTOR              0x00001000 /* direccion fisica de comienzo del bootsector (copiado) */
#define KERNEL                  0x00001200 /* direccion fisica de comienzo del kernel */


/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT 				31

#define GDT_IDX_NULL_DESC       0
#define GDT_NIVEL0_CODIGO       8
#define GDT_NIVEL0_DATOS    	9
#define GDT_NIVEL3_CODIGO       10
#define GDT_NIVEL3_DATOS        11
#define GDT_PANTALLA            12

#define TSS_INICIAL             13
#define TSS_IDLE                14

#define TSS_JUGADORA_1          15
#define TSS_JUGADORA_2          16
#define TSS_JUGADORA_3          17
#define TSS_JUGADORA_4          18
#define TSS_JUGADORA_5          19
#define TSS_JUGADORA_6          20
#define TSS_JUGADORA_7          21
#define TSS_JUGADORA_8          22

#define TSS_JUGADORB_1          23
#define TSS_JUGADORB_2          24
#define TSS_JUGADORB_3          25
#define TSS_JUGADORB_4          26
#define TSS_JUGADORB_5          27
#define TSS_JUGADORB_6          28
#define TSS_JUGADORB_7          29
#define TSS_JUGADORB_8          30

/* Valores CR3 para las tareas */
/* -------------------------------------------------------------------------- */
#define CR3_JUGADORA_1           0x00100100
#define CR3_JUGADORA_2           0x00100104
#define CR3_JUGADORA_3           0x00100108
#define CR3_JUGADORA_4           0x00100112
#define CR3_JUGADORA_5           0x00100116
#define CR3_JUGADORA_6           0x00100120
#define CR3_JUGADORA_7           0x00100124
#define CR3_JUGADORA_8           0x00100128
#define CR3_JUGADORB_1           0x00100132
#define CR3_JUGADORB_2           0x00100136
#define CR3_JUGADORB_3           0x00100140
#define CR3_JUGADORB_4           0x00100144
#define CR3_JUGADORB_5           0x00100148
#define CR3_JUGADORB_6           0x00100152
#define CR3_JUGADORB_7           0x00100156
#define CR3_JUGADORB_8           0x00100160

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC      << 3)

/* Selectores de segmentos */
/* -------------------------------------------------------------------------- */

void* error();

#define ASSERT(x) while(!(x)) {};


#endif  /* !__DEFINES_H__ */
