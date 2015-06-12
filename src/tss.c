/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "mmu.h"

tss tss_inicial;
tss tss_idle;

tss tss_jugadorA[MAX_CANT_PIRATAS_VIVOS];
tss tss_jugadorB[MAX_CANT_PIRATAS_VIVOS];

void tss_inicializar() {
	tss* tss_inicial_pos = &(tss_inicial);
	tss* tss_idle_pos = &(tss_idle);

	/* TSS Inicial */
    /* Offset = */
    gdt[TSS_INICIAL] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */
        (unsigned short)    (int)tss_inicial_pos,/* base[0:15]   */
        (unsigned char)     (int)tss_inicial_pos >> 16,	/* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (int)tss_inicial_pos >> 24,	/* base[31:24]  */
    };

    /* TSS Idle */
    /* Offset = */
    gdt[TSS_IDLE] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */
        (unsigned short)    (int)tss_idle_pos,   /* base[0:15]   */
        (unsigned char)     (int)tss_idle_pos >> 16, 	/* base[23:16]  */
        (unsigned char)     0x09,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (int)tss_idle_pos >> 24,		/* base[31:24]  */
    };

}

/*Crear tss idle*/
void tss_crear_idle(int cr3, int eflags, int esp, int ebp,
	short es, short cs, short ss, short ds, short fs, short gs) {	
	tss_idle = (tss) {
	    (unsigned short)	0x0000,		/*ptl;*/
	    (unsigned short)	0x0000,		/*unused0;*/
	    (unsigned int)  	esp,		/*esp0;*/
	    (unsigned short)  	ss,			/*ss0;*/
	    (unsigned short)  	0x0000,		/*unused1;*/
	    (unsigned int)    	0x00000000,	/*esp1;*/
	    (unsigned short)  	0x0000,		/*ss1;*/
	    (unsigned short)  	0x0000,		/*unused2;*/
	    (unsigned int)    	0x00000000,	/*esp2;*/
	    (unsigned short)  	0x0000,		/*ss2;*/
	    (unsigned short)  	0x0000,		/*unused3;*/
	    (unsigned int)    	cr3,		/*cr3;*/
	    (unsigned int)    	0x00016000,	/*eip;*/
	    (unsigned int)    	eflags,		/*eflags;*/
	    (unsigned int)    	0x00000000,	/*eax;*/
	    (unsigned int)    	0x00000000,	/*ecx;*/
	    (unsigned int)    	0x00000000,	/*edx;*/
	    (unsigned int)    	0x00000000,	/*ebx;*/
	    (unsigned int)    	esp,		/*esp;*/
	    (unsigned int)    	ebp,		/*ebp;*/
	    (unsigned int)    	0x00000000,	/*esi;*/
	    (unsigned int)    	0x00000000,	/*edi;*/
	    (unsigned short)  	es,			/*es;*/
	    (unsigned short)  	0x0000,		/*unused4;*/
	    (unsigned short)  	cs,			/*cs;*/
	    (unsigned short)  	0x0000,		/*unused5;*/
	    (unsigned short)  	ss,			/*ss;*/
	    (unsigned short) 	0x0000,		/*unused6;*/
	    (unsigned short)  	ds,			/*ds;*/
	    (unsigned short)  	0x0000,		/*unused7;*/
	    (unsigned short)  	fs,			/*fs;*/
	    (unsigned short)  	0x0000,		/*unused8;*/
	    (unsigned short)  	gs,			/*gs;*/
	    (unsigned short)  	0x0000,		/*unused9;*/
	    (unsigned short)  	0x0000,		/*ldt;*/
	    (unsigned short)  	0x0000,		/*unused10;*/
	    (unsigned short)  	0x0000,		/*dtrap;*/
	    (unsigned short)  	0xFFFF,		/*iomap;*/
    };
}

tss tss_crear_libre(int cr3, int eip, int eflags, int esp, int ebp,
	short es, short cs, short ss, short ds, short fs, short gs) {
	int* esp0 = mmu_get_pagina();

	return (tss) {
	    (unsigned short)	0x0000,		/*ptl;*/
	    (unsigned short)	0x0000,		/*unused0;*/
	    (unsigned int)  	esp0,		/*esp0;*/
	    (unsigned short)  	0x0048,		/*ss0;*/
	    (unsigned short)  	0x0000,		/*unused1;*/
	    (unsigned int)    	0x00000000,	/*esp1;*/
	    (unsigned short)  	0x0000,		/*ss1;*/
	    (unsigned short)  	0x0000,		/*unused2;*/
	    (unsigned int)    	0x00000000,	/*esp2;*/
	    (unsigned short)  	0x0000,		/*ss2;*/
	    (unsigned short)  	0x0000,		/*unused3;*/
	    (unsigned int)    	cr3,		/*cr3;*/
	    (unsigned int)    	eip,		/*eip;*/
	    (unsigned int)    	eflags,		/*eflags;*/
	    (unsigned int)    	0x00000000,	/*eax;*/
	    (unsigned int)    	0x00000000,	/*ecx;*/
	    (unsigned int)    	0x00000000,	/*edx;*/
	    (unsigned int)    	0x00000000,	/*ebx;*/
	    (unsigned int)    	esp,		/*esp;*/
	    (unsigned int)    	ebp,		/*ebp;*/
	    (unsigned int)    	0x00000000,	/*esi;*/
	    (unsigned int)    	0x00000000,	/*edi;*/
	    (unsigned short)  	es,			/*es;*/
	    (unsigned short)  	0x0000,		/*unused4;*/
	    (unsigned short)  	cs,			/*cs;*/
	    (unsigned short)  	0x0000,		/*unused5;*/
	    (unsigned short)  	ss,			/*ss;*/
	    (unsigned short) 	0x0000,		/*unused6;*/
	    (unsigned short)  	ds,			/*ds;*/
	    (unsigned short)  	0x0000,		/*unused7;*/
	    (unsigned short)  	fs,			/*fs;*/
	    (unsigned short)  	0x0000,		/*unused8;*/
	    (unsigned short)  	gs,			/*gs;*/
	    (unsigned short)  	0x0000,		/*unused9;*/
	    (unsigned short)  	0x0000,		/*ldt;*/
	    (unsigned short)  	0x0000,		/*unused10;*/
	    (unsigned short)  	0x0000,		/*dtrap;*/
	    (unsigned short)  	0x0000,		/*iomap;*/
    };
}