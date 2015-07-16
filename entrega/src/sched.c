/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/

#include "sched.h"
#include "i386.h"
#include "tss.h"
#include "mmu.h"

datosSched dSched;
estadoCPU eCPU;

void inicializar_sched() {
    int i;

    dSched.proxJugador = 'A'; // Jugador inicial elegido arbitrariamente
    dSched.tareaActual = 14;
    dSched.modoDebug = FALSE;
    dSched.huboExcepcion = TRUE;
    
    jugadorA.proxPirata = 8;
    jugadorA.piratasEnJuego = 0;
    jugadorA.botinesDescubiertos = 0;
    jugadorA.puntos = 0;
    jugadorA.barraTareas[15] = '\0';

    jugadorB.proxPirata = 8;
    jugadorB.piratasEnJuego = 0;
    jugadorB.botinesDescubiertos = 0;
    jugadorB.puntos = 0;
    jugadorB.barraTareas[15] = '\0';

    for(i = 0; i < 3520; i++) {
        jugadorA.posicionesMapeadas[i] = 0;
        jugadorB.posicionesMapeadas[i] = 0;
    }

    // El puerto y la grilla de 3x3 siempre van a estar mapeadas
    jugadorA.posicionesMapeadas[0] = 1;
    jugadorA.posicionesMapeadas[1] = 1;
    jugadorA.posicionesMapeadas[2] = 1;
    jugadorA.posicionesMapeadas[80] = 1;
    jugadorA.posicionesMapeadas[81] = 1;
    jugadorA.posicionesMapeadas[82] = 1;
    jugadorA.posicionesMapeadas[160] = 1;
    jugadorA.posicionesMapeadas[161] = 1;
    jugadorA.posicionesMapeadas[162] = 1;

    jugadorB.posicionesMapeadas[3357] = 1;
    jugadorB.posicionesMapeadas[3358] = 1;
    jugadorB.posicionesMapeadas[3359] = 1;
    jugadorB.posicionesMapeadas[3437] = 1;
    jugadorB.posicionesMapeadas[3438] = 1;
    jugadorB.posicionesMapeadas[3439] = 1;
    jugadorB.posicionesMapeadas[3517] = 1;
    jugadorB.posicionesMapeadas[3518] = 1;
    jugadorB.posicionesMapeadas[3519] = 1;

    for(i = 0; i < 15; i++) { // Esto lo hago porque el compilador no me deja asignar directamente el string "X X X X X X X X"
        if(i % 2 == 0) {
            jugadorA.barraTareas[i] = 'X';
            jugadorB.barraTareas[i] = 'X';
        } else {
            jugadorA.barraTareas[i] = ' ';
            jugadorB.barraTareas[i] = ' ';
        }
    }

    for(i = 0; i < 8; i++) {
        jugadorA.piratas[i].enJuego = FALSE;
        jugadorB.piratas[i].enJuego = FALSE;

        // Esto esta todo en un mismo ciclo porque la cantidad de botines es 8 al igual 
        // que la cantidad de piratas, si no fuese asi habria que cambiarlo
        jugadorA.botines[i][0] = 0;
        jugadorA.botines[i][1] = 0;
        jugadorA.botines[i][2] = 0;

        jugadorB.botines[i][0] = 0;
        jugadorB.botines[i][1] = 0;
        jugadorB.botines[i][2] = 0;

        jugadorA.posReloj[i] = 0;
        jugadorB.posReloj[i] = 0;
    }
}

uint sched_proxima_a_ejecutar() {
    if(dSched.proxJugador == 'A') {
    	if(jugadorA.piratasEnJuego == 0) {
            if(jugadorB.piratasEnJuego == 0) {
                return 14;
            } else {
                return 23 + jugadorB.proxPirata;
            }            
    	} else {
            return 15 + jugadorA.proxPirata;
        }
    } else if(dSched.proxJugador == 'B') {
    	if(jugadorB.piratasEnJuego == 0) {
            if(jugadorA.piratasEnJuego == 0) {
                return 14;
            } else {
                return 15 + jugadorA.proxPirata;
            }
    	} else {
        	return 23 + jugadorB.proxPirata;
        }
    }

    return 14;
}

void sched_actualizar_jugador(char proxJugador) {
    jugador_t* jProx;
    uint nPirata;
    uint tPirata;
    int i;

    if(proxJugador == 'A') {
        jProx = &jugadorA;

        // Me quedo en el jugador, o cambio al siguiente
        // Esto me garantiza que si cambio de jugador, este va a tener al menos un pirata

        if(jugadorB.piratasEnJuego == 0) {
            dSched.proxJugador = 'A';
        } else {
            dSched.proxJugador = 'B';
        }
    } else {
        jProx = &jugadorB;

        if(jugadorA.piratasEnJuego == 0) {
            dSched.proxJugador = 'B';
        } else {
            dSched.proxJugador = 'A';
        }
    }

    if((*jProx).piratasEnJuego == 0) {
        nPirata = 8;

    } else {
        tPirata = (*jProx).proxPirata;
        nPirata = tPirata; // Se queda en el mismo en el caso de no haber otro pirata
        for(i = 0; i < 7; i++) {
            if(tPirata == 7) {
                tPirata = 0;
            } else {
                tPirata++;
            }

            if(((*jProx).piratas[tPirata]).enJuego == TRUE) { 
                nPirata = tPirata;
                break;
            }
        }
    }

    (*jProx).proxPirata = nPirata;
}

/*uint sched_tick(uint id) {
    game_tick(id);

    uint nuevoId = id;

    if(dSched.proxJugador == 'A') {
        sched_actualizar_jugador(dSched.proxJugador);

    } else if(dSched.proxJugador == 'B') {
        sched_actualizar_jugador(dSched.proxJugador);
    }

	dSched.tareaActual = nuevoId;
    return nuevoId << 3;
}*/

uint sched_tick() {
    uint nuevoId = sched_proxima_a_ejecutar();

    if(dSched.modoDebug == TRUE && dSched.huboExcepcion == TRUE) {
        if(dSched.excepcionAtendida == FALSE) {
            game_guardar_pantalla();
            game_pintar_debug();
            dSched.excepcionAtendida = TRUE;
        }

        nuevoId = 14;

    } else {
        game_tick(nuevoId);
        sched_actualizar_jugador(dSched.proxJugador);
        
        dSched.tareaActual = nuevoId;
        sched_pendiente();
    }

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

void sched_pendiente() {
    int i;
    int tarea = sched_tarea_actual();
    jugador_t* jugador;
    int cr3j;
    unsigned char tssJ;
    uint posInitX;
    uint posInitY;

    if(tarea != 14) {
        if(tarea >= 15 && tarea < 23) {
            jugador = &jugadorA;
            cr3j = CR3_JUGADORA;
            tssJ = 'A';
            posInitX = 1;
            posInitY = 1;
        } else if(tarea >= 23 && tarea < 31) {
            jugador = &jugadorB;
            cr3j = CR3_JUGADORB;
            tssJ = 'B';
            posInitX = 78;
            posInitY = 42;
        }

        if((*jugador).piratasEnJuego < 8) { // Hay algun slot libre?
            for(i = 0; i < (*jugador).botinesDescubiertos; i++) { // Hay algun botin descubierto no minado?
                if((*jugador).botines[i][0] == 1) {
                    (*jugador).botines[i][0] = 0;

                    int slotLibre;

                    for(slotLibre = 0; slotLibre < 8; slotLibre++) {
                        if((*jugador).piratas[slotLibre].enJuego == FALSE) {
                            break; // LO HE ENCONTRADO, WOW
                        }
                    }

                    (*jugador).piratas[slotLibre].enJuego = TRUE;
                    (*jugador).piratas[slotLibre].esPirata = FALSE;
                    (*jugador).piratas[slotLibre].posX = posInitX;
                    (*jugador).piratas[slotLibre].posY = posInitY;

                    (*jugador).piratasEnJuego++;

                    int cr3 = cr3j + 4 * slotLibre;

                    int* posX = (int*)(0x12000 - 8);
                    int* posY = (int*)(0x12000 - 4);

                    (*posX) = (*jugador).botines[i][1];
                    (*posY) = (*jugador).botines[i][2];

                    posX = (int*)(0x14000 - 8);
                    posY = (int*)(0x14000 - 4);

                    (*posX) = (*jugador).botines[i][1];
                    (*posY) = (*jugador).botines[i][2];

                    //breakpoint();
                    if(tssJ == 'A') {
                       completar_tabla_tss(&tss_jugadorA[slotLibre], (void*)0x11000, (int*)cr3);
                    } else if (tssJ == 'B') {
                        completar_tabla_tss(&tss_jugadorB[slotLibre], (void*)0x13000, (int*)cr3);
                    }

                    int j;

                    for(j = 0; j < 3520; j++) {
                        if((*jugador).posicionesMapeadas[j] == 1) {
                            int dirFisica;
                            int posX = j % 80;
                            int posY = j / 80;
                            int cr3s = *((int*)(cr3));

                            dirFisica = MAPA + (posY * MAPA_ANCHO + posX) * PAGE_SIZE;

                            mmu_mapear_pagina(dirFisica + 0x300000, cr3s, dirFisica, READONLY);
                        }
                    }
                }
            }
        }
    }
}

void sched_pirata_manual() {
	dSched.proxJugador = 'B';
	dSched.tareaActual = 15;

    jugadorA.piratasEnJuego = 1;
    jugadorA.proxPirata = 0;
    jugadorA.botinesDescubiertos = 0;

    jugadorA.piratas[0].enJuego = TRUE;
    jugadorA.piratas[0].esPirata = TRUE;
    jugadorA.piratas[0].posX = 1;
    jugadorA.piratas[0].posY = 1;

    jugadorB.piratasEnJuego = 1;
    jugadorB.proxPirata = 0;
    jugadorB.botinesDescubiertos = 0;

    jugadorB.piratas[0].enJuego = TRUE;
    jugadorB.piratas[0].esPirata = TRUE;
    jugadorB.piratas[0].posX = 78;
    jugadorB.piratas[0].posY = 42;

	completar_tabla_tss(&tss_jugadorA[0], (void*)0x10000, (int*)CR3_JUGADORA);
    completar_tabla_tss(&tss_jugadorB[0], (void*)0x12000, (int*)CR3_JUGADORB);

}