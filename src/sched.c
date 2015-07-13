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

void inicializar_sched() {
    int i;

    dSched.proxJugador = 'A'; // Jugador inicial elegido arbitrariamente
    dSched.tareaActual = 14;
    
    jugadorA.proxPirata = 8;
    jugadorA.piratasEnJuego = 0;
    jugadorA.botinesDescubiertos = 0;

    jugadorB.proxPirata = 8;
    jugadorB.piratasEnJuego = 0;
    jugadorB.botinesDescubiertos = 0;

    for(i = 0; i < 3520; i++) {
        jugadorA.posicionesMapeadas[i] = 0;
        jugadorB.posicionesMapeadas[i] = 0;
    }

    for(i = 0; i < 8; i++) {
        jugadorA.piratas[i].enJuego = FALSE;
        jugadorB.piratas[i].enJuego = FALSE;

        // Esto esta todo en un mismo ciclo porque la cantidad de botines es 8, si no fuese asi habria que cambiarlo
        jugadorA.botines[i][0] = 0;
        jugadorA.botines[i][1] = 0;
        jugadorA.botines[i][2] = 0;

        jugadorB.botines[i][0] = 0;
        jugadorB.botines[i][1] = 0;
        jugadorB.botines[i][2] = 0;
    }
}

uint sched_proxima_a_ejecutar() {
    if(dSched.proxJugador == 'A') {
    	if(jugadorA.proxPirata == 8) {
            // NO HAY PIRATAS
            if(jugadorB.proxPirata == 8) {
                return 14;
            } else {
                return 23 + jugadorB.proxPirata;
            }            
    	} else {
            return 15 + jugadorA.proxPirata;
        }
    } else if(dSched.proxJugador == 'B') {
    	if(jugadorB.proxPirata == 8) {
            if(jugadorA.proxPirata == 8) {
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

void sched_mapear_jugador(char jugador, uint pirata) {
    int* cr3;
    uint posX;
    uint posY;
    uint dirCodigo;
    jugador_t* j;

    if(jugador == 'A') {
        cr3 = (int*)CR3_JUGADORA;
        j = &jugadorA;
    } else {
        cr3 = (int*)CR3_JUGADORB;
        j = &jugadorB;
    }

    posX = (*j).piratas[pirata].posX;
    posY = (*j).piratas[pirata].posY;

    dirCodigo = MAPA + (posY * MAPA_ANCHO + posX) * PAGE_SIZE;

    //mmu_unmapear_pagina(0x400000, *(cr3));
    breakpoint();
    mmu_mapear_pagina(0x400000, *(cr3), dirCodigo);
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

uint sched_tick(uint id) {
    breakpoint();
    game_tick(id);

    uint nuevoId = id;

    if(dSched.proxJugador == 'A') {
        // Remapeo el codigo de la tarea
        if(jugadorA.piratasEnJuego != 0) {
            breakpoint();
            sched_mapear_jugador('A', jugadorA.proxPirata);

        } else {
            sched_mapear_jugador('B', jugadorB.proxPirata);

        }

        sched_actualizar_jugador(dSched.proxJugador);

    } else if(dSched.proxJugador == 'B') {
        
        if(jugadorB.piratasEnJuego != 0) {
            breakpoint();
            sched_mapear_jugador('B', jugadorB.proxPirata);

        } else {
            sched_mapear_jugador('A', jugadorA.proxPirata);

        }

        sched_actualizar_jugador(dSched.proxJugador);
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

void sched_pendiente() {
    int i;
    int tarea = sched_tarea_actual();

    if(tarea >= 15 && tarea < 23) {
        if(jugadorA.piratasEnJuego < 8) { // Hay algun slot libre?
            for(i = 0; i < jugadorA.botinesDescubiertos; i++) { // Hay algun botin descubierto no minado?
                if(jugadorA.botines[i][0] == 1) {
                    jugadorA.botines[i][0] = 0;

                    int slotLibre;

                    for(slotLibre = 0; slotLibre < 8; slotLibre++) {
                        if(jugadorA.piratas[slotLibre].enJuego == FALSE) {
                            break; // LO HE ENCONTRADO, WOW
                        }
                    }

                    /*jugadorA.piratas[slotLibre].enJuego = TRUE;
                    jugadorA.piratas[slotLibre].esPirata = FALSE;
                    jugadorA.piratas[slotLibre].posX = 1;
                    jugadorA.piratas[slotLibre].posY = 1;

                    jugadorA.piratasEnJuego++;
                    jugadorA.botinesDescubiertos--;

                    int* posX = (int*)0x12000 - 8;
                    int* posY = (int*)0x12000 - 4;

                    (*posX) = jugadorA.botines[i][1];
                    (*posY) = jugadorA.botines[i][2];

                    //breakpoint();
                    //completar_tabla_tss(&tss_jugadorA[slotLibre], (void*)0x11000, (int*)CR3_JUGADORA);
                    */

                }
            }
        }

    } else {
        for(i = 0; i < jugadorB.botinesDescubiertos; i++) {
            if(jugadorB.botines[i][0] == 1) {

            }
        }
    }
}

void sched_pirata_manual() {
	dSched.proxJugador = 'B';
	dSched.tareaActual = 15;

    jugadorA.piratasEnJuego = 1;
    jugadorA.proxPirata = 0;
	jugadorA.posicionesMapeadas[0] = 1;
	jugadorA.posicionesMapeadas[1] = 1;
	jugadorA.posicionesMapeadas[2] = 1;
	jugadorA.posicionesMapeadas[80] = 1;
	jugadorA.posicionesMapeadas[81] = 1;
	jugadorA.posicionesMapeadas[82] = 1;
	jugadorA.posicionesMapeadas[160] = 1;
	jugadorA.posicionesMapeadas[161] = 1;
	jugadorA.posicionesMapeadas[162] = 1;
    jugadorA.botinesDescubiertos = 0;

    jugadorA.piratas[0].enJuego = TRUE;
    jugadorA.piratas[0].esPirata = TRUE;
    jugadorA.piratas[0].posX = 1;
    jugadorA.piratas[0].posY = 1;

    jugadorB.piratasEnJuego = 1;
    jugadorB.proxPirata = 0;
    jugadorB.posicionesMapeadas[3357] = 1;
    jugadorB.posicionesMapeadas[3358] = 1;
    jugadorB.posicionesMapeadas[3359] = 1;
    jugadorB.posicionesMapeadas[3437] = 1;
    jugadorB.posicionesMapeadas[3438] = 1;
    jugadorB.posicionesMapeadas[3439] = 1;
    jugadorB.posicionesMapeadas[3517] = 1;
    jugadorB.posicionesMapeadas[3518] = 1;
    jugadorB.posicionesMapeadas[3519] = 1;
    jugadorB.botinesDescubiertos = 0;

    jugadorB.piratas[0].enJuego = TRUE;
    jugadorB.piratas[0].esPirata = TRUE;
    jugadorB.piratas[0].posX = 78;
    jugadorB.piratas[0].posY = 42;

	completar_tabla_tss(&tss_jugadorA[0], (void*)0x10000, (int*)CR3_JUGADORA);
    completar_tabla_tss(&tss_jugadorB[0], (void*)0x12000, (int*)CR3_JUGADORB);

    mmu_mapear_pagina(0x400000, *((int*)CR3_JUGADORA), INICIO_PIRATAA);
    //mmu_mapear_pagina(0x400000, *((int*)CR3_JUGADORB), INICIO_PIRATAB);
	//mmu_copiar_pagina((int*)0x10000, (int*)0x0400000);
    //mmu_copiar_pagina((int*)0x12000, (int*)0x156E000);

	// int* pila = (int*)0x00400FF4;
	// (*pila) = 1;
	// pila = pila + 4;
	// (*pila) = 1;
}