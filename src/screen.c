/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/

#include "screen.h"
#include "game.h"
#include "i386.h"

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

void print_string(const char * text, uint x, uint y, unsigned short attr) {
    int i;
    for (i = 0; text[i] != 0; i++) {
        screen_pintar(text[i], attr, x, y);

        y++;

        if (y == VIDEO_COLS) {
            y = 0;
        }
    }
}

void print_hex(uint numero, int size, uint y, uint x, unsigned short attr) {
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

void debug_screen() {
    // container borders
    screen_pintar_linea_h(0, C_BG_BLACK, VIDEO_FILS/2 - 17, VIDEO_COLS/2 - 15, 30);
    screen_pintar_linea_h(0, C_BG_BLACK, VIDEO_FILS/2 + 17, VIDEO_COLS/2 - 15, 30);
    screen_pintar_linea_v(0, C_BG_BLACK, VIDEO_FILS/2 - 17, VIDEO_COLS/2 - 15, 35);
    screen_pintar_linea_v(0, C_BG_BLACK, VIDEO_FILS/2 - 17, VIDEO_COLS/2 + 15, 35);

    // clean inner space
    screen_pintar_rect(0, C_BG_RED, VIDEO_FILS / 2 - 16, VIDEO_COLS/2 - 14, 33, 29);

    print_string("eax", VIDEO_FILS / 2 - 16 + 2, VIDEO_COLS/2 - 14 + 1, C_FG_WHITE | C_BG_RED);
    print_hex(eax(), 8, VIDEO_FILS / 2 - 16 + 2, VIDEO_COLS/2 - 14 + 5, C_FG_WHITE | C_BG_RED);

    print_string("ebx", VIDEO_FILS / 2 - 16 + 4, VIDEO_COLS/2 - 14 + 1, C_FG_WHITE | C_BG_RED);
    print_hex(ebx(), 8, VIDEO_FILS / 2 - 16 + 4, VIDEO_COLS/2 - 14 + 5, C_FG_WHITE | C_BG_RED);

    print_string("ecx", VIDEO_FILS / 2 - 16 + 6, VIDEO_COLS/2 - 14 + 1, C_FG_WHITE | C_BG_RED);
    print_hex(ecx(), 8, VIDEO_FILS / 2 - 16 + 6, VIDEO_COLS/2 - 14 + 5, C_FG_WHITE | C_BG_RED);

    print_string("edx", VIDEO_FILS / 2 - 16 + 8, VIDEO_COLS/2 - 14 + 1, C_FG_WHITE | C_BG_RED);
    print_hex(edx(), 8, VIDEO_FILS / 2 - 16 + 8, VIDEO_COLS/2 - 14 + 5, C_FG_WHITE | C_BG_RED);

    print_string("esi", VIDEO_FILS / 2 - 16 + 10, VIDEO_COLS/2 - 14 + 1, C_FG_WHITE | C_BG_RED);
    print_hex(esi(), 8, VIDEO_FILS / 2 - 16 + 10, VIDEO_COLS/2 - 14 + 5, C_FG_WHITE | C_BG_RED);

    print_string("ebp", VIDEO_FILS / 2 - 16 + 12, VIDEO_COLS/2 - 14 + 1, C_FG_WHITE | C_BG_RED);
    print_hex(ebp(), 8, VIDEO_FILS / 2 - 16 + 12, VIDEO_COLS/2 - 14 + 5, C_FG_WHITE | C_BG_RED);

    print_string("esp", VIDEO_FILS / 2 - 16 + 14, VIDEO_COLS/2 - 14 + 1, C_FG_WHITE | C_BG_RED);
    print_hex(esp(), 8, VIDEO_FILS / 2 - 16 + 14, VIDEO_COLS/2 - 14 + 5, C_FG_WHITE | C_BG_RED);

    // mirar nota en i386.h
    print_string("eip", VIDEO_FILS / 2 - 16 + 16, VIDEO_COLS/2 - 14 + 1, C_FG_WHITE | C_BG_RED);
    // print_hex(eip(), 8, VIDEO_FILS / 2 - 16 + 16, VIDEO_COLS/2 - 14 + 5, C_FG_WHITE | C_BG_RED);

    print_string("cs", VIDEO_FILS / 2 - 16 + 18, VIDEO_COLS/2 - 14 + 1, C_FG_WHITE | C_BG_RED);
    print_hex(cs(), 8, VIDEO_FILS / 2 - 16 + 18, VIDEO_COLS/2 - 14 + 5, C_FG_WHITE | C_BG_RED);

    print_string("ds", VIDEO_FILS / 2 - 16 + 20, VIDEO_COLS/2 - 14 + 1, C_FG_WHITE | C_BG_RED);
    print_hex(ds(), 8, VIDEO_FILS / 2 - 16 + 20, VIDEO_COLS/2 - 14 + 5, C_FG_WHITE | C_BG_RED);

    print_string("es", VIDEO_FILS / 2 - 16 + 22, VIDEO_COLS/2 - 14 + 1, C_FG_WHITE | C_BG_RED);
    print_hex(es(), 8, VIDEO_FILS / 2 - 16 + 22, VIDEO_COLS/2 - 14 + 5, C_FG_WHITE | C_BG_RED);

    print_string("fs", VIDEO_FILS / 2 - 16 + 24, VIDEO_COLS/2 - 14 + 1, C_FG_WHITE | C_BG_RED);
    print_hex(fs(), 8, VIDEO_FILS / 2 - 16 + 24, VIDEO_COLS/2 - 14 + 5, C_FG_WHITE | C_BG_RED);

    print_string("gs", VIDEO_FILS / 2 - 16 + 26, VIDEO_COLS/2 - 14 + 1, C_FG_WHITE | C_BG_RED);
    print_hex(gs(), 8, VIDEO_FILS / 2 - 16 + 26, VIDEO_COLS/2 - 14 + 5, C_FG_WHITE | C_BG_RED);

    print_string("ss", VIDEO_FILS / 2 - 16 + 28, VIDEO_COLS/2 - 14 + 1, C_FG_WHITE | C_BG_RED);
    print_hex(ss(), 8, VIDEO_FILS / 2 - 16 + 28, VIDEO_COLS/2 - 14 + 5, C_FG_WHITE | C_BG_RED);

    print_string("eflags", VIDEO_FILS / 2 - 16 + 30, VIDEO_COLS/2 - 14 + 1, C_FG_WHITE | C_BG_RED);
    print_hex(ss(), 8, VIDEO_FILS / 2 - 16 + 30, VIDEO_COLS/2 - 14 + 8, C_FG_WHITE | C_BG_RED);


    print_string("cr0", VIDEO_FILS / 2 - 16 + 2, VIDEO_COLS/2 - 14 + 15, C_FG_WHITE | C_BG_RED);
    print_hex(rcr0(), 8, VIDEO_FILS / 2 - 16 + 2, VIDEO_COLS/2 - 14 + 20, C_FG_WHITE | C_BG_RED);

    print_string("cr1", VIDEO_FILS / 2 - 16 + 4, VIDEO_COLS/2 - 14 + 15, C_FG_WHITE | C_BG_RED);
    // print_hex(rcr1(), 8, VIDEO_FILS / 2 - 16 + 4, VIDEO_COLS/2 - 14 + 20, C_FG_WHITE | C_BG_RED);
    // me pincha todo cuando quiero levantar el cr1...

    print_string("cr2", VIDEO_FILS / 2 - 16 + 6, VIDEO_COLS/2 - 14 + 15, C_FG_WHITE | C_BG_RED);
    print_hex(rcr2(), 8, VIDEO_FILS / 2 - 16 + 6, VIDEO_COLS/2 - 14 + 20, C_FG_WHITE | C_BG_RED);

    print_string("cr3", VIDEO_FILS / 2 - 16 + 8, VIDEO_COLS/2 - 14 + 15, C_FG_WHITE | C_BG_RED);
    print_hex(rcr3(), 8, VIDEO_FILS / 2 - 16 + 8, VIDEO_COLS/2 - 14 + 20, C_FG_WHITE | C_BG_RED);

    print_string("stack", VIDEO_FILS / 2 - 16 + 18, VIDEO_COLS/2 - 14 + 15, C_FG_WHITE | C_BG_RED);
    // ver como cargar el stack bien. esto vino de un call, nosotros queremos el stack antes del call.
    // tendriamos que ignorar el push del EIP de la primera y segunda llamada, despues imprimir.

}

void screen_inicializar() {
	// upper margin
	screen_pintar_linea_h(0, C_BG_BLACK, 0, 0, VIDEO_COLS);

	// pirate arena
	screen_pintar_rect(0, C_BG_LIGHT_GREY, 1, 0, VIDEO_FILS - 6, VIDEO_COLS);

	// pintar contenedores de score
	screen_pintar_rect(0, C_BG_RED, VIDEO_FILS - 5, VIDEO_COLS / 2 - 6, 5, 6);
	screen_pintar_rect(0, C_BG_BLUE, VIDEO_FILS - 5, VIDEO_COLS / 2, 5, 6);

	// pintar bordes de score
	screen_pintar_rect(0, C_BG_BLACK, VIDEO_FILS - 5, 0, 5, VIDEO_COLS / 2 - 7); // left border
	screen_pintar_rect(0, C_BG_BLACK, VIDEO_FILS - 5, VIDEO_COLS / 2 + 7, 5, VIDEO_COLS / 2 - 7); // right border

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

        print_string((chota[i] + from), fila + i,  columna, 0xF);
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