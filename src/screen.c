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


void screen_actualizar_reloj_global() {
    static uint reloj_global = 0;

    reloj_global = (reloj_global + 1) % reloj_size;

    screen_pintar(reloj[reloj_global], C_BW, 49, 79);
}

void screen_pintar(uchar c, uchar color, uint fila, uint columna) {
    p[fila][columna].c = c;
    p[fila][columna].a = color;
}

uchar screen_valor_actual(uint fila, uint columna) {
    return p[fila][columna].c;
}

void print(const char * text, uint x, uint y, unsigned short attr) {
    int i;

    for (i = 0; text[i] != 0; i++) {
        screen_pintar(text[i], attr, x, y);

        y++;
        if (y == VIDEO_COLS) {
            y = 0;
            x++;
        }
    }
}

void printS(const char * text, uint x, uint y, unsigned short attr) {
    int i;
    for (i = 0; text[i] != 0; i++) {
        screen_pintar(text[i], attr, x, y);

        y++;

        if (y == VIDEO_COLS) {
            y = 0;
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

uint min(uint left, uint right) {
    if (left > right) {
        return right;
    }

    return left;
}

void screen_pintar_rect(unsigned char c, unsigned char color, uint fila, uint columna, uint alto, uint ancho) {
    uint i;
    uint j;
    for (i = fila; i < min(fila + alto, VIDEO_FILS); ++i) {
        for (j = columna; j < min(columna + ancho, VIDEO_COLS); ++j) {
            screen_pintar(c, color, i, j);
        }
    }
}

void screen_pintar_linea_h(unsigned char c, unsigned char color, uint fila, uint columna, uint ancho) {
    uint j;

    for (j = columna; j < min(columna + ancho, VIDEO_COLS); ++j) {
        screen_pintar(c, color, fila, j);
    }
}

void screen_pintar_linea_v(unsigned char c, unsigned char color, uint fila, uint columna, uint alto) {
    uint i;

    for (i = fila; i < min(fila + alto, VIDEO_FILS); ++i) {
        screen_pintar(c, color, i, columna);
    }
}

void clear() {
    screen_pintar_rect(0, 0, 0, 0, VIDEO_FILS, VIDEO_COLS);
}

char *chota[] = {
        "                              -    .|||||.",
        "                                  |||||||||",
        "                          -      ||||||  .",
        "                              -  ||||||   >",
        "                                ||||||| -/",
        "                           --   ||||||'(",
        "                        -       .'      \\",
        "                             .-'    | | |",
        "                            /        \\ \\ \\",
        "              --        -  |      `---:.`.\\",
        "             ____________._>           \\\\_\\\\____ ,--.__",
        "  --    ,--\"\"           /    `-   .     |)_)    '\\     '\\",
        "       /  \"             |      .-'     /          \\      '\\",
        "     ,/                  \\           .'            '\\     |",
        "     | \"   \"   \"          \\         /                '\\,  /",
        "     |           \" , =_____`-.   .-'_________________,--\"\"",
        "   - |  \"    \"    /\"/'      /\\>-' ( <",
        "     \\  \"      \",/ /    -  ( <    |\\_)",
        "      \\   \",\",_/,-'        |\\_)",
        "   -- -'-;.__:-'"
    };

uint strlen(char *text) {
    uint c = 0;
    while (*text != 0) {
        ++c;
        text++;
    }
    return c;
}

void screen_printear_ascii(uint fila, uint columna, uint from) {
    uint i;
    for (i = 0; i < 20; ++i) {
        if (fila + i > VIDEO_FILS) {
            break;
        }

        if (from >= strlen(chota[i])) {
            continue;
        }

        printS((chota[i] + from), fila + i,  columna, 0xF);
    }
}

void screen_refresh_chota() {
    uint clocknegro = 0;
    uint fila = 5;
    uint columna = 0;
    uint sw = -1;

    while (1) {
        if (clocknegro % (5000 - 1) == 0) {
            screen_pintar_rect(0, 0, 0, 0, 26, min(VIDEO_COLS, (columna + 60)));
        }

        if (clocknegro % 5000 == 0) {
            screen_printear_ascii(fila + sw, columna, 0);

            if (sw == -1) {
                sw = 1;
            } else {
                sw = -1;
            }

            columna++;

            if (columna == VIDEO_COLS) {
                //return;
            }
        }

        columna %= VIDEO_COLS;
        clocknegro++;
    } 
}

void screen_pintar_puntajes() {

}