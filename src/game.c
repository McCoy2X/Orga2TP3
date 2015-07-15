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

uchar tempPantalla[1080][2];

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
		if (botines[i][0] == x && botines[i][1] == y) {
			return botines[i][2];
        }
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


void game_jugador_inicializar(jugador_t *j)
{
	static int index = 0;

	j->index = index++;
    // ~ completar ~

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
                    mmu_mapear_pagina(0x00400000, cr3, MAPA + ((posY - 1) * MAPA_ANCHO + posX) * PAGE_SIZE, READWRITE);

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
                    mmu_mapear_pagina(0x00400000, cr3, MAPA + ((posY + 1) * MAPA_ANCHO + posX) * PAGE_SIZE, READWRITE);

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
                    mmu_mapear_pagina(0x00400000, cr3, MAPA + (posY * MAPA_ANCHO + (posX + 1)) * PAGE_SIZE, READWRITE);

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
                    mmu_mapear_pagina(0x00400000, cr3, MAPA + (posY * MAPA_ANCHO + (posX - 1)) * PAGE_SIZE, READWRITE);

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
                        mmu_mapear_pagina(0x00400000, cr3, MAPA + ((posY - 1) * MAPA_ANCHO + posX) * PAGE_SIZE, READWRITE);

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
                        mmu_mapear_pagina(0x00400000, cr3, MAPA + ((posY + 1) * MAPA_ANCHO + posX) * PAGE_SIZE, READWRITE);

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
                        mmu_mapear_pagina(0x00400000, cr3, MAPA + (posY * MAPA_ANCHO + (posX + 1)) * PAGE_SIZE, READWRITE);

                        print("M", posX, posY + 1, color);
                        print("M", posX + 1, posY + 1, C_BG_RED | C_FG_BLACK);
                        ((*j).piratas[id - idJugador]).posX = posX + 1;
                    } else { breakpoint();
                        sched_syscall(1);
                    }
                }
            } else if(dir == 13) {
                if(posX != 0) {
                    if(((*j).posicionesMapeadas[posY * MAPA_ANCHO + (posX - 1)]) == 1) {
                        mmu_copiar_pagina((int*)(0x00400000), (int*)(MAPA_VIRTUAL + (posY * MAPA_ANCHO + (posX - 1)) * PAGE_SIZE));
                        mmu_mapear_pagina(0x00400000, cr3, MAPA + (posY * MAPA_ANCHO + (posX - 1)) * PAGE_SIZE, READWRITE);

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

    if((*j).posicionesMapeadas[posY * MAPA_ANCHO + (posX + 1)] == 1) {
        if(game_valor_tesoro(posX + 1, posY) != 0) {
            for(i = 0; i < (*j).botinesDescubiertos; i++) { // Reviso si el tesoro ya fue encontrado;
                if((*j).botines[i][1] == (posX + 1) && (*j).botines[i][2] == posY) {
                    break;
                }
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

    if(posX + 2 <= 79) {
        if((*j).posicionesMapeadas[posY * MAPA_ANCHO + (posX + 2)] == 1) {
            if(game_valor_tesoro(posX + 2, posY) != 0) {
                for(i = 0; i < (*j).botinesDescubiertos; i++) { // Reviso si el tesoro ya fue encontrado;
                    if((*j).botines[i][1] == (posX + 2) && (*j).botines[i][2] == posY) {
                        break;
                    }
                }

                if(i == (*j).botinesDescubiertos) { // Si recorri todos los botines, y no se encuentra en el arreglo, lo agrego
                    print("o", posX + 2, posY + 1 , color);
                    (*j).botines[i][0] = 1;
                    (*j).botines[i][1] = posX + 2;
                    (*j).botines[i][2] = posY;
                    (*j).botinesDescubiertos++;
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

void game_guardar_estado(uint edi, uint esi, uint ebp, uint ignorar, uint ebx, uint edx, 
                        uint ecx, uint eax, uint codigoError, uint eip, uint cs, uint eflags, uint esp, uint ss) {
    eCPU.edi = edi;
    eCPU.esi = esi;
    eCPU.ebp = ebp;
    eCPU.ebx = ebx;
    eCPU.edx = edx;
    eCPU.ecx = ecx;
    eCPU.eax = eax;
    eCPU.ss = ss;
    eCPU.esp = esp;
    eCPU.eflags = eflags;
    eCPU.cs = cs;
    eCPU.eip = eip;
    eCPU.ds = ss;
    eCPU.es = ss;
    eCPU.fs = ss;
    eCPU.gs = ss;
    eCPU.cr0 = rcr0();
    eCPU.cr2 = rcr2();
    eCPU.cr4 = rcr4();

    int id = sched_tarea_actual();

    if(id >= 15 && id < 23) {
        eCPU.cr3 = *((uint*)(CR3_JUGADORA + 4 * (id - 15)));
    } else {
        eCPU.cr3 = *((uint*)(CR3_JUGADORB + 4 * (id - 23)));
    }

    lcr3(eCPU.cr3);

    eCPU.stack1 = *((uint*)(eCPU.esp));
    eCPU.stack2 = *((uint*)(eCPU.esp + 4));
    eCPU.stack3 = *((uint*)(eCPU.esp + 8));
    eCPU.stack4 = *((uint*)(eCPU.esp + 12));
    eCPU.stack5 = *((uint*)(eCPU.esp + 16));

    lcr3(0x27000);

    dSched.huboExcepcion = TRUE;

}

void game_guardar_pantalla() {
    int i;
    int j;    
    uint posArreglo;

    for(i = 25; i <= 54; i++) {
        for(j = 7; j <= 42; j++) {
            posArreglo = (30*(j - 7)) + (i - 25);

            tempPantalla[posArreglo][0] = screen_valor_actual(j,i); 
            tempPantalla[posArreglo][1] = screen_color_actual(j,i);
        }
    }
}

void game_pintar_debug() {
    screen_pintar_rect(' ', C_BG_BLACK, 25, 7, 36, 30);
    screen_pintar_rect(' ', C_BG_LIGHT_GREY, 26, 8, 34, 28);
    screen_pintar_rect(' ', C_BG_RED, 26, 8, 1, 28);

    print("eax", 27, 10, C_FG_BLACK | C_BG_LIGHT_GREY);
    print("ebx", 27, 12, C_FG_BLACK | C_BG_LIGHT_GREY);
    print("ecx", 27, 14, C_FG_BLACK | C_BG_LIGHT_GREY);
    print("edx", 27, 16, C_FG_BLACK | C_BG_LIGHT_GREY);
    print("esi", 27, 18, C_FG_BLACK | C_BG_LIGHT_GREY);
    print("edi", 27, 20, C_FG_BLACK | C_BG_LIGHT_GREY);
    print("ebp", 27, 22, C_FG_BLACK | C_BG_LIGHT_GREY);
    print("esp", 27, 24, C_FG_BLACK | C_BG_LIGHT_GREY);
    print("eip", 27, 26, C_FG_BLACK | C_BG_LIGHT_GREY);
    print(" cs", 27, 28, C_FG_BLACK | C_BG_LIGHT_GREY);
    print(" ds", 27, 30, C_FG_BLACK | C_BG_LIGHT_GREY);
    print(" es", 27, 32, C_FG_BLACK | C_BG_LIGHT_GREY);
    print(" fs", 27, 34, C_FG_BLACK | C_BG_LIGHT_GREY);
    print(" gs", 27, 36, C_FG_BLACK | C_BG_LIGHT_GREY);
    print(" ss", 27, 38, C_FG_BLACK | C_BG_LIGHT_GREY);
    print(" eflags", 27, 40, C_FG_BLACK | C_BG_LIGHT_GREY);

    print("cr0", 41, 10, C_FG_BLACK | C_BG_LIGHT_GREY);
    print("cr2", 41, 12, C_FG_BLACK | C_BG_LIGHT_GREY);
    print("cr3", 41, 14, C_FG_BLACK | C_BG_LIGHT_GREY);
    print("cr4", 41, 16, C_FG_BLACK | C_BG_LIGHT_GREY);
    print("stack", 41, 27, C_FG_BLACK | C_BG_LIGHT_GREY);

    print_hex(eCPU.eax, 8, 31, 10, C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(eCPU.ebx, 8, 31, 12, C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(eCPU.ecx, 8, 31, 14, C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(eCPU.edx, 8, 31, 16, C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(eCPU.esi, 8, 31, 18, C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(eCPU.edi, 8, 31, 20, C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(eCPU.ebp, 8, 31, 22, C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(eCPU.esp, 8, 31, 24, C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(eCPU.eip, 8, 31, 26, C_FG_WHITE | C_BG_LIGHT_GREY);

    print_hex(eCPU.cs, 4, 31, 28, C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(eCPU.ds, 4, 31, 30, C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(eCPU.es, 4, 31, 32, C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(eCPU.fs, 4, 31, 34, C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(eCPU.gs, 4, 31, 36, C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(eCPU.ss, 4, 31, 38, C_FG_WHITE | C_BG_LIGHT_GREY);

    print_hex(eCPU.eflags, 8, 34, 40, C_FG_WHITE | C_BG_LIGHT_GREY);

    print_hex(eCPU.cr0, 8, 45, 10, C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(eCPU.cr2, 8, 45, 12, C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(eCPU.cr3, 8, 45, 14, C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(eCPU.cr4, 8, 45, 16, C_FG_WHITE | C_BG_LIGHT_GREY);

    print_hex(eCPU.stack1, 8, 41, 30, C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(eCPU.stack2, 8, 41, 31, C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(eCPU.stack3, 8, 41, 32, C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(eCPU.stack4, 8, 41, 33, C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(eCPU.stack5, 8, 41, 34, C_FG_WHITE | C_BG_LIGHT_GREY);

}

void game_restaurar_pantalla() {
    int i;
    int j;
    uint posArreglo;

    for(i = 25; i <= 54; i++) {
        for(j = 7; j <= 42; j++) {

            posArreglo = (30*(j - 7)) + (i - 25);
            screen_pintar(tempPantalla[posArreglo][0], tempPantalla[posArreglo][1], j, i);
        }
    }
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
#define KB_y        0x15

void game_atender_teclado(unsigned char tecla)
{
    int i;
    jugador_t* jugador;
    int cr3j;
    unsigned char tssJ;
    uint posInitX;
    uint posInitY;

    if(tecla == KB_shiftA || tecla == KB_shiftB) {

        if(tecla == KB_shiftA) {
            jugador = &jugadorA;
            cr3j = CR3_JUGADORA;
            tssJ = 'A';
            posInitX = 1;
            posInitY = 1;
        } else {
            jugador = &jugadorB;
            cr3j = CR3_JUGADORB;
            tssJ = 'B';
            posInitX = 78;
            posInitY = 42;
        }

        if((*jugador).piratasEnJuego < 8) { // Hay algun slot libre?

            int slotLibre;

            for(slotLibre = 0; slotLibre < 8; slotLibre++) {
                if((*jugador).piratas[slotLibre].enJuego == FALSE) {
                    break; // LO HE ENCONTRADO, WOW
                }
            }

            (*jugador).piratas[slotLibre].enJuego = TRUE;
            (*jugador).piratas[slotLibre].esPirata = TRUE;
            (*jugador).piratas[slotLibre].posX = posInitX;
            (*jugador).piratas[slotLibre].posY = posInitY;

            if((*jugador).piratasEnJuego == 0) {
                (*jugador).proxPirata = slotLibre;
            }

            (*jugador).piratasEnJuego++;

            int cr3 = cr3j + 4 * slotLibre;

            //breakpoint();
            if(tssJ == 'A') {
                completar_tabla_tss(&tss_jugadorA[slotLibre], (void*)0x10000, (int*)cr3);
            } else if (tssJ == 'B') {
                completar_tabla_tss(&tss_jugadorB[slotLibre], (void*)0x12000, (int*)cr3);
            }

            for(i = 0; i < 3520; i++) {
                if((*jugador).posicionesMapeadas[i] == 1) {
                    int dirFisica;
                    int posX = i % 80;
                    int posY = i / 80;
                    int cr3s = *((int*)(cr3));

                    dirFisica = MAPA + (posY * MAPA_ANCHO + posX) * PAGE_SIZE;

                    mmu_mapear_pagina(dirFisica + 0x300000, cr3s, dirFisica, READONLY);
                }
            }
        }
    } else if(tecla == KB_y) {
        if(dSched.modoDebug == FALSE) {
            print("DEBUG", 0, 0, C_FG_BLACK | C_BG_LIGHT_GREY);
            dSched.modoDebug = TRUE;
            dSched.huboExcepcion = FALSE;
            dSched.excepcionAtendida = FALSE;
        
        } else {
            if(dSched.huboExcepcion == TRUE) {
                dSched.huboExcepcion = FALSE;
                
                if(dSched.excepcionAtendida == TRUE) {
                    game_restaurar_pantalla();
                    dSched.excepcionAtendida = FALSE;
                }

            } else {
                print("     ", 0, 0, C_FG_BLACK | C_BG_LIGHT_GREY);
                dSched.modoDebug = FALSE;
                dSched.huboExcepcion = FALSE;
                dSched.excepcionAtendida = FALSE;
            }
        }
    }
}