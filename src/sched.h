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
} datosSched;

extern datosSched dSched;

void inicializar_sched();

#endif	/* !__SCHED_H__ */
