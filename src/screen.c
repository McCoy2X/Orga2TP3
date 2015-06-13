/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/

#include "screen.h"
#include "game.h"


extern jugador_t jugadorA, jugadorB;


static ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;

const char reloj[] = "|/-\\";
#define reloj_size 4

void screen_actualizar_reloj_global()
{
    static uint reloj_global = 0;

    reloj_global = (reloj_global + 1) % reloj_size;

    screen_pintar(reloj[reloj_global], C_BW, 49, 79);
}

void screen_pintar(uchar c, uchar color, uint fila, uint columna)
{
    p[fila][columna].c = c;
    p[fila][columna].a = color;
}

uchar screen_valor_actual(uint fila, uint columna)
{
    return p[fila][columna].c;
}

void print(const char * text, uint x, uint y, unsigned short attr) {
    int i;
    for (i = 0; text[i] != 0; i++)
     {
        screen_pintar(text[i], attr, y, x);

        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_hex(uint numero, int size, uint x, uint y, unsigned short attr) {
    int i;
    char hexa[8];
    char letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_dec(uint numero, int size, uint x, uint y, unsigned short attr) {
    int i;
    char letras[16] = "0123456789";

    for(i = 0; i < size; i++) {
        int resto  = numero % 10;
        numero = numero / 10;
        p[y][x + size - i - 1].c = letras[resto];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_group() {
    print("A_Nightmare_on_Elm_Street_/_Family_Game", 80 - 39, 0, C_FG_DARK_GREY | C_BG_LIGHT_GREY);
}

void screen_inicializar() {
    screen_pintar_rect(0, C_BG_LIGHT_GREY, 0, 0, 50, 80);
    screen_pintar_puntajes();
}

void screen_refrescar() {
    screen_inicializar();
    
    print("000", 35, 47, C_BG_RED | C_FG_WHITE);
    print("000", 42, 47, C_BG_BLUE | C_FG_WHITE);
    
    print("1 2 3 4 5 6 7 8", 4, 46, C_BG_BLACK | C_FG_WHITE);
    print("X X X X X X X X", 4, 48, C_BG_BLACK | C_FG_RED);

    print("1 2 3 4 5 6 7 8", 60, 46, C_BG_BLACK | C_FG_WHITE);
    print("X X X X X X X X", 60, 48, C_BG_BLACK | C_FG_BLUE);

    print("(-) -", 75, 49, C_BG_BLACK | C_FG_WHITE);
}

void screen_pintar_rect(unsigned char c, unsigned char color, int fila, int columna, int alto, int ancho) {
    int i;
    int j;
    for(i = fila; i < fila + ancho; i++) {
        for(j = columna; j < columna + alto; j++) {
            screen_pintar(c, color, j, i);
        }
    }
}

/*void screen_pintar_linea_h(unsigned char c, unsigned char color, int fila, int columna, int ancho) {

}

void screen_pintar_linea_v(unsigned char c, unsigned char color, int fila, int columna, int alto) {

}*/

void screen_pintar_puntajes() {
    screen_pintar_rect(0, C_BG_BLACK, 0, 45, 5, 80);
    screen_pintar_rect(0, C_BG_RED, 33, 45, 5, 7);
    screen_pintar_rect(0, C_BG_BLUE, 40, 45, 5, 7);
}