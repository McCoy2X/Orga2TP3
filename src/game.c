/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "mmu.h"
#include "tss.h"
#include "screen.h"
#include "sched.h"

#include <stdarg.h>


#define POS_INIT_A_X                      1
#define POS_INIT_A_Y                      1
#define POS_INIT_B_X         MAPA_ANCHO - 2
#define POS_INIT_B_Y          MAPA_ALTO - 2

#define CANT_POSICIONES_VISTAS            9
#define MAX_SIN_CAMBIOS                 999

#define BOTINES_CANTIDAD 8

const char relojJugadores[] = "|/-\\";
#define TAMANO_RELOJ 4

uint botines[BOTINES_CANTIDAD][3] = { // TRIPLAS DE LA FORMA (X, Y, MONEDAS)
                                        {30,  3, 50}, {30, 38, 50}, {15, 21, 100}, {45, 21, 100} ,
                                        {49,  3, 50}, {49, 38, 50}, {64, 21, 100}, {34, 21, 100}
                                    };

jugador_t jugadorA;
jugador_t jugadorB;


void* error()
{
	__asm__ ("int3");
	return 0;
}

uint game_xy2lineal (uint x, uint y) {
	return (y * MAPA_ANCHO + x);
}

uint game_posicion_valida(int x, int y) {
	return (x >= 0 && y >= 0 && x < MAPA_ANCHO && y < MAPA_ALTO);
}

pirata_t* id_pirata2pirata(uint id_pirata)
{
    // ~ completar ~
	return NULL;
}

uint game_dir2xy(direccion dir, int *x, int *y)
{
	switch (dir)
	{
		case IZQ: *x = -1; *y =  0; break;
		case DER: *x =  1; *y =  0; break;
		case ABA: *x =  0; *y =  1; break;
		case ARR: *x =  0; *y = -1; break;
    	default: return -1;
	}

	return 0;
}

uint game_valor_tesoro(uint x, uint y)
{
	int i;
	for (i = 0; i < BOTINES_CANTIDAD; i++)
	{
		if (botines[i][0] == x && botines[i][1] == y)
			return botines[i][2];
	}
	return 0;
}

// dada una posicion (x,y) guarda las posiciones de alrededor en dos arreglos, uno para las x y otro para las y
void game_calcular_posiciones_vistas(int *vistas_x, int *vistas_y, int x, int y)
{
	int next = 0;
	int i, j;
	for (i = -1; i <= 1; i++)
	{
		for (j = -1; j <= 1; j++)
		{
			vistas_x[next] = x + j;
			vistas_y[next] = y + i;
			next++;
		}
	}
}


void game_inicializar()
{
}

void game_jugador_inicializar_mapa(jugador_t *jug)
{
}

void game_jugador_inicializar(jugador_t *j)
{
	static int index = 0;

	j->index = index++;
    // ~ completar ~

}

void game_pirata_inicializar(pirata_t *pirata, jugador_t *j, uint index, uint id)
{
}

void game_tick(uint id_pirata)
{
    screen_actualizar_reloj_global();
    int i;
    int j = 0;

    for(i = 0; i < 8; i++) {
        j = i * 2;

        if(jugadorA.piratas[i].enJuego == TRUE) {
            if(id_pirata >= 15 && id_pirata < 23) {
                if(i == id_pirata - 15) {
                    jugadorA.barraTareas[j] = relojJugadores[jugadorA.posReloj[i]];
                    jugadorA.posReloj[i] = (jugadorA.posReloj[i] + 1) % TAMANO_RELOJ;
                }
            }
        } else {
            jugadorA.barraTareas[j] = 'X';
            jugadorA.posReloj[i] = 0;
        }

        if(jugadorB.piratas[i].enJuego == TRUE) {
            if(id_pirata >= 23 && id_pirata < 31) {
                if(i == id_pirata - 23) {
                    jugadorB.barraTareas[j] = relojJugadores[jugadorB.posReloj[i]];
                    jugadorB.posReloj[i] = (jugadorB.posReloj[i] + 1) % TAMANO_RELOJ;
                }
            }
        } else {
            jugadorB.barraTareas[j] = 'X';
            jugadorB.posReloj[i] = 0;
        }
        //j = j + 2;
    }

    print(jugadorA.barraTareas, 4, 48, C_BG_BLACK | C_FG_RED);
    print(jugadorB.barraTareas, 60, 48, C_BG_BLACK | C_FG_BLUE);

}


void game_pirata_relanzar(pirata_t *pirata, jugador_t *j, uint tipo)
{
}

pirata_t* game_jugador_erigir_pirata(jugador_t *j, uint tipo)
{
    // ~ completar ~

	return NULL;
}


void game_jugador_lanzar_pirata(jugador_t *j, uint tipo, int x, int y)
{
}

void game_pirata_habilitar_posicion(jugador_t *j, pirata_t *pirata, int x, int y)
{
}


void game_explorar_posicion(jugador_t *jugador, int c, int f)
{
}


uint game_syscall_pirata_mover(uint id, direccion dir)
{
    // MAPEAR PAGINAS
    //unsigned int cr3 = 0x00100100 + (15 - id) * 0x00000004;

    uint posY;
    uint posX;
    char nombreJugador;
    uint idJugador;
    unsigned short color;
    int cr3;
    jugador_t* j;

    if(id > 14 && id < 23) {
        posY = (jugadorA.piratas[id - 15]).posY;
        posX = (jugadorA.piratas[id - 15]).posX;
        nombreJugador = 'A';
        idJugador = 15;
        color = C_BG_GREEN | C_FG_BLACK;
        j = &jugadorA;
        cr3 = *((int*)(CR3_JUGADORA + 4 * (id - idJugador)));
    } else if(id >= 23 && id < 31) {
        posY = (jugadorB.piratas[id - 23]).posY;
        posX = (jugadorB.piratas[id - 23]).posX;
        nombreJugador = 'B';
        idJugador = 23;
        color = C_BG_BLUE | C_FG_BLACK;
        j = &jugadorB;
        cr3 = *((int*)(CR3_JUGADORB + 4 * (id - idJugador)));
    }

    if(id > 14 && id < 31) {
        if(((*j).piratas[id - idJugador]).esPirata == TRUE) {

            if(dir == 4) {
                if(posY != 0) {
                    if(posY != 1) {
                        mmu_mapear_pirata_H(id - idJugador, nombreJugador, ((int)posX) - 1, ((int)posY) - 2);
                        game_pirata_check_botines_H(nombreJugador, ((int)posX) - 1, ((int)posY) - 2);
                    }

                    mmu_copiar_pagina((int*)(0x00400000), (int*)(MAPA_VIRTUAL + ((posY - 1) * MAPA_ANCHO + posX) * PAGE_SIZE));
                    mmu_mapear_pagina(0x00400000, cr3, MAPA + ((posY - 1) * MAPA_ANCHO + posX) * PAGE_SIZE);

                    print("E", posX, posY + 1, color);
                    print("E", posX, posY, C_BG_RED | C_FG_BLACK);
                    ((*j).piratas[id - idJugador]).posY = posY - 1;

                }
            } else if(dir == 7) {
                if(posY != MAPA_ALTO - 1) {
                    if(posY != MAPA_ALTO - 2) {
                        mmu_mapear_pirata_H(id - idJugador, nombreJugador, ((int)posX) - 1, ((int)posY) + 2);
                        game_pirata_check_botines_H(nombreJugador, ((int)posX) - 1, ((int)posY) + 2);
                    }

                    mmu_copiar_pagina((int*)(0x00400000), (int*)(MAPA_VIRTUAL + ((posY + 1) * MAPA_ANCHO + posX) * PAGE_SIZE));
                    mmu_mapear_pagina(0x00400000, cr3, MAPA + ((posY + 1) * MAPA_ANCHO + posX) * PAGE_SIZE);

                    print("E", posX, posY + 1, color);
                    print("E", posX, posY + 2, C_BG_RED | C_FG_BLACK);
                    ((*j).piratas[id - idJugador]).posY = posY + 1;

                }
            } else if(dir == 10) {
                if(posX != MAPA_ANCHO - 1) {
                    if(posX != MAPA_ANCHO - 2) {
                        mmu_mapear_pirata_V(id - idJugador, nombreJugador, ((int)posX) + 2, ((int)posY) - 1);
                        game_pirata_check_botines_V(nombreJugador, ((int)posX) + 2, ((int)posY) - 1);
                    }

                    //breakpoint();
                    mmu_copiar_pagina((int*)(0x00400000), (int*)(MAPA_VIRTUAL + (posY * MAPA_ANCHO + (posX + 1)) * PAGE_SIZE));
                    mmu_mapear_pagina(0x00400000, cr3, MAPA + (posY * MAPA_ANCHO + (posX + 1)) * PAGE_SIZE);

                    print("E", posX, posY + 1, color);
                    print("E", posX + 1, posY + 1, C_BG_RED | C_FG_BLACK);
                    ((*j).piratas[id - idJugador]).posX = posX + 1;

                }
            } else if(dir == 13) {
                if(posX != 0) {
                    if(posX != 1) {
                        mmu_mapear_pirata_V(id - idJugador, nombreJugador, ((int)posX) - 2, ((int)posY) - 1);
                        game_pirata_check_botines_V(nombreJugador, ((int)posX) - 2, ((int)posY) - 1);
                    }

                    mmu_copiar_pagina((int*)(0x00400000), (int*)(MAPA_VIRTUAL + (posY * MAPA_ANCHO + (posX - 1)) * PAGE_SIZE));
                    mmu_mapear_pagina(0x00400000, cr3, MAPA + (posY * MAPA_ANCHO + (posX - 1)) * PAGE_SIZE);

                    print("E", posX, posY + 1, color);
                    print("E", posX - 1, posY + 1, C_BG_RED | C_FG_BLACK);
                    ((*j).piratas[id - idJugador]).posX = posX - 1;

                }
            }
        } else {

            if(dir == 4) {
                if(posY != 0) {
                    if(((*j).posicionesMapeadas[(posY - 1) * MAPA_ANCHO + posX]) == 1) {
                        mmu_copiar_pagina((int*)(0x00400000), (int*)(MAPA_VIRTUAL + ((posY - 1) * MAPA_ANCHO + posX) * PAGE_SIZE));
                        mmu_mapear_pagina(0x00400000, cr3, MAPA + ((posY - 1) * MAPA_ANCHO + posX) * PAGE_SIZE);

                        print("M", posX, posY + 1, color);
                        print("M", posX, posY, C_BG_RED | C_FG_BLACK);
                        ((*j).piratas[id - idJugador]).posY = posY - 1;
                    } else {
                        sched_syscall(1); // Lo desalojo
                    }
                }
            } else if(dir == 7) {
                if(posY != MAPA_ALTO - 1) {
                    if(((*j).posicionesMapeadas[(posY + 1) * MAPA_ANCHO + posX]) == 1) {
                        mmu_copiar_pagina((int*)(0x00400000), (int*)(MAPA_VIRTUAL + ((posY + 1) * MAPA_ANCHO + posX) * PAGE_SIZE));
                        mmu_mapear_pagina(0x00400000, cr3, MAPA + ((posY + 1) * MAPA_ANCHO + posX) * PAGE_SIZE);

                        print("M", posX, posY + 1, color);
                        print("M", posX, posY + 2, C_BG_RED | C_FG_BLACK);
                        ((*j).piratas[id - idJugador]).posY = posY + 1;
                    } else {
                        sched_syscall(1);
                    }
                }
            } else if(dir == 10) {
                if(posX != MAPA_ANCHO - 1) {
                    if(((*j).posicionesMapeadas[posY * MAPA_ANCHO + (posX + 1)]) == 1) {
                        mmu_copiar_pagina((int*)(0x00400000), (int*)(MAPA_VIRTUAL + (posY * MAPA_ANCHO + (posX + 1)) * PAGE_SIZE));
                        mmu_mapear_pagina(0x00400000, cr3, MAPA + (posY * MAPA_ANCHO + (posX + 1)) * PAGE_SIZE);

                        print("M", posX, posY + 1, color);
                        print("M", posX + 1, posY + 1, C_BG_RED | C_FG_BLACK);
                        ((*j).piratas[id - idJugador]).posX = posX + 1;
                    } else {
                        sched_syscall(1);
                    }
                }
            } else if(dir == 13) {
                if(posX != 0) {
                    if(((*j).posicionesMapeadas[posY * MAPA_ANCHO + (posX - 1)]) == 1) {
                        mmu_copiar_pagina((int*)(0x00400000), (int*)(MAPA_VIRTUAL + (posY * MAPA_ANCHO + (posX - 1)) * PAGE_SIZE));
                        mmu_mapear_pagina(0x00400000, cr3, MAPA + (posY * MAPA_ANCHO + (posX - 1)) * PAGE_SIZE);

                        print("M", posX, posY + 1, color);
                        print("M", posX - 1, posY + 1, C_BG_RED | C_FG_BLACK);
                        ((*j).piratas[id - idJugador]).posX = posX - 1;
                    } else {
                        sched_syscall(1);
                    }
                }
            }            
        }
    }

    //screen_refrescar();
    return 0;
}

void game_pirata_check_botines_H(char jugador, int posX, int posY) {
    jugador_t* j;
    int i;
    unsigned short color;

    if(jugador == 'A') {
        j = &jugadorA;
        color = C_BG_GREEN | C_FG_BLACK;
    } else {
        j = &jugadorB;
        color = C_BG_BLUE | C_FG_BLACK;
    }

    if(posX >= 0) {
        if((*j).posicionesMapeadas[posY * MAPA_ANCHO + posX] == 1) {
            if(game_valor_tesoro(posX, posY) != 0) {
                for(i = 0; i < (*j).botinesDescubiertos; i++) { // Reviso si el tesoro ya fue encontrado;
                    if((*j).botines[i][1] == posX && (*j).botines[i][2] == posY) {
                        break;
                    }

                    if(i == (*j).botinesDescubiertos) { // Si recorri todos los botines, y no se encuentra en el arreglo, lo agrego
                        print("o", posX, posY + 1, color);
                        (*j).botines[i][0] = 1;
                        (*j).botines[i][1] = posX;
                        (*j).botines[i][2] = posY;
                        (*j).botinesDescubiertos++;
                    }
                }
            }
        }
    }

    if((*j).posicionesMapeadas[posY * MAPA_ANCHO + (posX + 1)] == 1) {
        if(game_valor_tesoro(posX + 1, posY) != 0) {
            for(i = 0; i < (*j).botinesDescubiertos; i++) { // Reviso si el tesoro ya fue encontrado;
                if((*j).botines[i][1] == (posX + 1) && (*j).botines[i][2] == posY) {
                    break;
                }

                if(i == (*j).botinesDescubiertos) { // Si recorri todos los botines, y no se encuentra en el arreglo, lo agrego
                    print("o", posX + 1, posY + 1, color);
                    (*j).botines[i][0] = 1;
                    (*j).botines[i][1] = posX + 1;
                    (*j).botines[i][2] = posY;
                    (*j).botinesDescubiertos++;
                }
            }
        }
    }

    if(posX + 2 <= 79) {
        if((*j).posicionesMapeadas[posY * MAPA_ANCHO + (posX + 2)] == 1) {
            if(game_valor_tesoro(posX + 2, posY) != 0) {
                for(i = 0; i < (*j).botinesDescubiertos; i++) { // Reviso si el tesoro ya fue encontrado;
                    if((*j).botines[i][1] == (posX + 2) && (*j).botines[i][2] == posY) {
                        break;
                    }

                    if(i == (*j).botinesDescubiertos) { // Si recorri todos los botines, y no se encuentra en el arreglo, lo agrego
                        print("o", posX + 2, posY +1 , color);
                        (*j).botines[i][0] = 1;
                        (*j).botines[i][1] = posX + 2;
                        (*j).botines[i][2] = posY;
                        (*j).botinesDescubiertos++;
                    }
                }
            }
        }
    }
}

void game_pirata_check_botines_V(char jugador, int posX, int posY) {
    jugador_t* j;
    int i;
    unsigned short color;

    if(jugador == 'A') {
        j = &jugadorA;
        color = C_BG_GREEN | C_FG_BLACK;
    } else {
        j = &jugadorB;
        color = C_BG_BLUE | C_FG_BLACK;
    }

    if(posX >= 0) {
        if((*j).posicionesMapeadas[posY * MAPA_ANCHO + posX] == 1) {
            if(game_valor_tesoro(posX, posY) != 0) {
                for(i = 0; i < (*j).botinesDescubiertos; i++) { // Reviso si el tesoro ya fue encontrado;
                    if((*j).botines[i][1] == posX && (*j).botines[i][2] == posY) {
                        break;
                    }
                }

                if(i == (*j).botinesDescubiertos) { // Si recorri todos los botines, y no se encuentra en el arreglo, lo agrego
                    //breakpoint();
                    print("o", posX, posY + 1, color);
                    (*j).botines[i][0] = 1;
                    (*j).botines[i][1] = posX;
                    (*j).botines[i][2] = posY;
                    (*j).botinesDescubiertos++;
                }
            }
        }
    }

    if((*j).posicionesMapeadas[(posY + 1) * MAPA_ANCHO + posX] == 1) {
        if(game_valor_tesoro(posX, posY + 1) != 0) {
            for(i = 0; i < (*j).botinesDescubiertos; i++) { // Reviso si el tesoro ya fue encontrado;
                if((*j).botines[i][1] == posX && (*j).botines[i][2] == (posY + 1)) {
                    break;
                }
            }

            if(i == (*j).botinesDescubiertos) { // Si recorri todos los botines, y no se encuentra en el arreglo, lo agrego
                print("o", posX, posY + 2, color);
                (*j).botines[i][0] = 1;
                (*j).botines[i][1] = posX;
                (*j).botines[i][2] = posY + 1;
                (*j).botinesDescubiertos++;
            }
        }
    }

    if(posX + 2 <= 79) {
        if((*j).posicionesMapeadas[(posY + 2) * MAPA_ANCHO + posX] == 1) {
            if(game_valor_tesoro(posX, posY + 2) != 0) {
                for(i = 0; i < (*j).botinesDescubiertos; i++) { // Reviso si el tesoro ya fue encontrado;
                    if((*j).botines[i][1] == posX && (*j).botines[i][2] == (posY + 2)) {
                        break;
                    }
                }
                
                if(i == (*j).botinesDescubiertos) { // Si recorri todos los botines, y no se encuentra en el arreglo, lo agrego
                    print("o", posX, posY + 3, color);
                    (*j).botines[i][0] = 1;
                    (*j).botines[i][1] = posX;
                    (*j).botines[i][2] = posY + 2;
                    (*j).botinesDescubiertos++;
                }
            }
        }
    }
}

void game_pirata_minero() {

}

uint game_syscall_pirata_cavar(uint id) {
    uint posY;
    uint posX;
    jugador_t* j;
    int i;
    uint posXPuntaje;
    uint posYPuntaje;
    unsigned short color;

    if(id > 14 && id < 23) {
        posY = (jugadorA.piratas[id - 15]).posY;
        posX = (jugadorA.piratas[id - 15]).posX;
        j = &jugadorA;
        posXPuntaje = 35;
        posYPuntaje = 47;
        color = C_BG_RED | C_FG_WHITE;
    } else if(id >= 23 && id < 31) {
        posY = (jugadorB.piratas[id - 23]).posY;
        posX = (jugadorB.piratas[id - 23]).posX;
        j = &jugadorB;
        posXPuntaje = 42;
        posYPuntaje = 47;
        color = C_BG_BLUE | C_FG_WHITE;
    }

    for(i = 0; i < BOTINES_CANTIDAD; i++) {
        if(botines[i][0] == posX && botines[i][1] == posY) {
            if(botines[i][2] == 0) {
                return 1;
            } else {
                botines[i][2]--;
                (*j).puntos++;
                print_dec((*j).puntos, 3, posXPuntaje, posYPuntaje, color);
                return 0;
            }
        }
    }   

    return 1;
}

uint game_syscall_cavar(uint id)
{
    // ~ completar ~

	return 0;
}

uint game_syscall_pirata_posicion(uint id, int idx)
{
    if(idx == -1) {
        if(id > 14 && id < 23) {
            return ((jugadorA.piratas[id - 15]).posY << 8) | (jugadorA.piratas[id - 15]).posX;
        } else {
            return ((jugadorB.piratas[id - 23]).posY << 8) | (jugadorB.piratas[id - 23]).posX;
        }
    } else {
        if(id >= 23 && id < 31) {
            return ((jugadorA.piratas[(uint)idx - 15]).posY << 8) | (jugadorA.piratas[(uint)idx - 15]).posX;
        } else {
            return ((jugadorB.piratas[(uint)idx - 23]).posY << 8) | (jugadorB.piratas[(uint)idx - 23]).posX;
        }
    }
    // ~ completar ~
    return 0;
}

uint game_syscall_manejar(uint syscall, uint param1)
{
    // ~ completar ~
    return 0;
}

void game_pirata_exploto(uint id)
{
}

pirata_t* game_pirata_en_posicion(uint x, uint y)
{
	return NULL;
}


void game_jugador_anotar_punto(jugador_t *j)
{
}



void game_terminar_si_es_hora()
{
}


#define KB_w_Aup    0x11 // 0x91
#define KB_s_Ado    0x1f // 0x9f
#define KB_a_Al     0x1e // 0x9e
#define KB_d_Ar     0x20 // 0xa0
#define KB_e_Achg   0x12 // 0x92
#define KB_q_Adir   0x10 // 0x90
#define KB_i_Bup    0x17 // 0x97
#define KB_k_Bdo    0x25 // 0xa5
#define KB_j_Bl     0x24 // 0xa4
#define KB_l_Br     0x26 // 0xa6
#define KB_shiftA   0x2a // 0xaa
#define KB_shiftB   0x36 // 0xb6


void game_atender_teclado(unsigned char tecla)
{
}
