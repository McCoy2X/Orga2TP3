/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "game.h"  	

typedef struct datosSched
{
    char proxJugador;
    uint tareaActual;
    char modoDebug;
    char huboExcepcion;
    char excepcionAtendida;
} datosSched;

typedef struct estadoCPU
{
    uint eax;
    uint ebx;
    uint ecx;
    uint edx;
    uint esi;
    uint edi;
    uint ebp;
    uint esp;
    uint eip;
    uint cs;
    uint ds;
    uint es;
    uint fs;
    uint gs;
    uint ss;
    uint eflags;
    uint cr0;
    uint cr2;
    uint cr3;
    uint cr4;
    uint stack1;
    uint stack2;
    uint stack3;
    uint stack4;
    uint stack5;
} estadoCPU;

extern datosSched dSched;
extern estadoCPU eCPU;

void inicializar_sched();
uint sched_tick();
uint sched_proxima_a_ejecutar();
uint sched_tarea_actual();
void sched_syscall(char desalojar);
void sched_pirata_manual();
void sched_pendiente();
void sched_actualizar_jugador(char proxJugador);

#endif	/* !__SCHED_H__ */
