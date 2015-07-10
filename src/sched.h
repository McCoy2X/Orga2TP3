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
} datosSched;

extern datosSched dSched;

void inicializar_sched();
uint sched_tick(uint id);
uint sched_proxima_a_ejecutar();
uint sched_tarea_actual();
void sched_syscall(char desalojar);
void sched_pirata_manual();

#endif	/* !__SCHED_H__ */
