/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/

#include "sched.h"
#include "i386.h"

datosSched dSched;

void inicializar_sched() {
    dSched.proxJugador = 'I';
    dSched.tareaActual = 13;
    
    jugadorA.hayPiratas = FALSE;
    jugadorA.proxPirata = 8;

    jugadorB.hayPiratas = FALSE;
    jugadorB.proxPirata = 8;
}

uint sched_proxima_a_ejecutar() {
    if(dSched.proxJugador == 'A') {
        return 14 + jugadorA.proxPirata;
    } else if(dSched.proxJugador == 'B') {
        return 22 + jugadorB.proxPirata;
    } else {
        return 14; // TAREA IDLE
    }
}

uint sched_tick(uint id) {
    game_tick(id);
    return id << 3;
}

uint sched_tarea_actual() {
    return dSched.tareaActual;
}