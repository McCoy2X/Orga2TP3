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
    
    jugadorA.hayPiratas = FALSE;
    jugadorA.proxPirata = 8;

    jugadorB.hayPiratas = FALSE;
    jugadorB.proxPirata = 8;
}
