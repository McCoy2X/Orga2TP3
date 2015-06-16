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
    dSched.proxJugador = 'A'; // Jugador inicial elegido arbitrariamente
    dSched.tareaActual = 14;
    
    jugadorA.proxPirata = 8;
    jugadorA.piratasEnJuego = 0;

    jugadorB.proxPirata = 8;
    jugadorB.piratasEnJuego = 0;
}

uint sched_proxima_a_ejecutar() {
    if(dSched.proxJugador == 'A') {
    	if(jugadorA.proxPirata == 8) {
    		// NO HAY PIRATAS
    		return 14;
    	} else {
        	return 14 + jugadorA.proxPirata;
        }
    } else if(dSched.proxJugador == 'B') {
    	if(jugadorB.proxPirata == 8) {
    		return 14;
    	} else {
        	return 23 + jugadorB.proxPirata;
        }
    }

    return 14;
}

uint sched_tick(uint id) {
    game_tick(id);
    uint tPirata;
    uint nPirata;
    int i;
    uint nuevoId = id;

    if(dSched.proxJugador == 'A') {
    	dSched.proxJugador = 'B';
    	if(jugadorB.piratasEnJuego == 0) {
    		nPirata = 8;
    	} else {
	    	tPirata = jugadorB.proxPirata;
	    	nPirata = tPirata; // Se queda en el mismo en el caso de no haber otro pirata
	    	for(i = 0; i < 8; i++) {
		    	if(tPirata == 7) {
		    		tPirata = 0;
		    	} else {
		    		tPirata++;
		    	}

		    	if((jugadorB.piratas[tPirata]).enJuego == TRUE) {
		    		nPirata = tPirata;
		    		break;
		    	}
	    	}
	   	}

	   	jugadorB.proxPirata = nPirata;
    } else if(dSched.proxJugador == 'B') {
    	dSched.proxJugador = 'A';
    	if(jugadorA.piratasEnJuego == 0) {
    		nPirata = 8;
    	} else {
	    	tPirata = jugadorA.proxPirata;
	    	nPirata = tPirata; // Se queda en el mismo en el caso de no haber otro pirata
	    	for(i = 0; i < 8; i++) {
		    	if(tPirata == 7) {
		    		tPirata = 0;
		    	} else {
		    		tPirata++;
		    	}

		    	if((jugadorA.piratas[tPirata]).enJuego == TRUE) {
		    		nPirata = tPirata;
		    		break;
		    	}
	    	}
	   	}

	   	jugadorB.proxPirata = nPirata;
    }

	dSched.tareaActual = nuevoId;
    return nuevoId << 3;
}

uint sched_tarea_actual() {
    return dSched.tareaActual;
}

void sched_syscall(char desalojar) {
	uint id = sched_tarea_actual();

	if(desalojar == TRUE) {
		if(id > 14 && id < 23) {
			(jugadorA.piratas[id - 15]).enJuego = FALSE;
			jugadorA.piratasEnJuego--;
		} else if(id >= 23 && id < 31) {
			(jugadorB.piratas[id - 23]).enJuego = FALSE;
			jugadorB.piratasEnJuego--;
		}
	}
}