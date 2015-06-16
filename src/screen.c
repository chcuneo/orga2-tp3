/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/

#include "screen.h"
#include "game.h"
#include "error.h"

static ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
static uint reloj_global = 0;

const char reloj[] = "|/-\\";
#define reloj_size 4

void screen_actualizar_reloj_global() {
    reloj_global = (reloj_global + 1) % reloj_size;

    screen_pintar(reloj[reloj_global], C_BW, 49, 79);
}

void screen_pintar(uchar c, uchar color, uint fila, uint columna) {
    p[fila][columna].c = c;
    p[fila][columna].a = color;
}

void screen_changecolor(uchar color, uint columna, uint fila) {
    p[fila][columna].a = color;
}

void screen_changechar(uchar ch, uint columna, uint fila) {
    p[fila][columna].c = ch;
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
        p[x][y + size - i - 1].c = letras[resto];
        p[x][y + size - i - 1].a = attr;
    }
}

int screen_pintar_rect(unsigned char c, unsigned char color, uint fila, uint columna, uint alto, uint ancho) {
    if (fila > VIDEO_FILS || fila + alto > VIDEO_FILS) {
        return E_OUT_OF_BOUNDS;
    }

    if (columna > VIDEO_COLS || columna + ancho > VIDEO_COLS) {
        return E_OUT_OF_BOUNDS;
    }

    uint i;
    uint j;

    for (i = fila; i < fila + alto; ++i) {
        for (j = columna; j < columna + ancho; ++j) {
            screen_pintar(c, color, i, j);
        }
    }

    return E_OK;
}

int screen_pintar_linea_h(unsigned char c, unsigned char color, uint fila, uint columna, uint ancho) {
    if (fila > VIDEO_FILS) {
        return E_OUT_OF_BOUNDS;
    }

    if (columna > VIDEO_COLS || columna + ancho > VIDEO_COLS) {
        return E_OUT_OF_BOUNDS;
    }

    uint j;

    for (j = columna; j < columna + ancho; ++j) {
        screen_pintar(c, color, fila, j);
    }

    return E_OK;
}

int screen_pintar_linea_v(unsigned char c, unsigned char color, uint fila, uint columna, uint alto) {
    if (fila > VIDEO_FILS || fila + alto > VIDEO_FILS) {
        return E_OUT_OF_BOUNDS;
    }

    if (columna > VIDEO_COLS) {
        return E_OUT_OF_BOUNDS;
    }

    uint i;

    for (i = fila; i < MIN(fila + alto, VIDEO_FILS); ++i) {
        screen_pintar(c, color, i, columna);
    }

    return E_OK;
}

void screen_clear() {
    screen_pintar_rect(0, 0, 0, 0, VIDEO_FILS, VIDEO_COLS);
}

void screen_inicializar() {
	// upper margin
	screen_pintar_linea_h(0, C_BG_BLACK, 0, 0, VIDEO_COLS);

	// pirate arena
	screen_pintar_rect(0, C_BG_LIGHT_GREY, 1, 0, VIDEO_FILS - 6, VIDEO_COLS);

	// pintar bordes de score
	screen_pintar_rect(0, C_BG_BLACK, VIDEO_FILS - 5, 0, 5, VIDEO_COLS / 2 - 7); // left border
	screen_pintar_rect(0, C_BG_BLACK, VIDEO_FILS - 5, VIDEO_COLS / 2 + 7, 5, VIDEO_COLS / 2 - 7); // right border

    screen_pintar_puntajes();
}

extern jugador_t jugadorA, jugadorB;

void screen_pintar_puntajes() {
    // pintar contenedores de score
    screen_pintar_rect(0, C_BG_RED, VIDEO_FILS - 5, VIDEO_COLS / 2 - 6, 5, 6);
    screen_pintar_rect(0, C_BG_BLUE, VIDEO_FILS - 5, VIDEO_COLS / 2, 5, 6);

    print_dec(jugadorA.score, 4, VIDEO_FILS - 3, VIDEO_COLS / 2 - 5, C_BG_RED | C_FG_WHITE);
    print_dec(jugadorB.score, 4, VIDEO_FILS - 3, VIDEO_COLS / 2 + 1, C_BG_BLUE | C_FG_WHITE);
}

void screen_show_debug(uint exception, const char *message, const tss *registers) {
    unsigned int fila = ((VIDEO_FILS - 33) / 2) - 1;
    unsigned int col = ((VIDEO_COLS - 28) / 2) - 1;
    unsigned short attrData = 0x7F;
    unsigned short attrLabels = 0x70;

    screen_pintar_linea_h(' ', 0x00, fila + 0, col + 0, 30);
    screen_pintar_linea_v(' ', 0x00, fila + 1, col + 0, 34);
    screen_pintar_linea_h(' ', 0x00, fila + 35, col + 0, 30);
    screen_pintar_linea_v(' ', 0x00, fila + 1, col + 29, 34);
    screen_pintar_linea_h(' ', 0x44, fila + 1, col + 1, 28);
    screen_pintar_rect(' ', 0x70, fila + 2, col + 1, 33, 28);

    print("eax", fila + 3, col + 2, attrLabels);
    print_hex(registers->eax, 8, col + 6, fila + 3, attrData);
    print("ebx", fila + 5, col + 2, attrLabels); 
    print_hex(registers->ebx, 8, col + 6, fila + 5, attrData);
    print("ecx", fila + 7, col + 2, attrLabels); 
    print_hex(registers->ecx, 8, col + 6, fila + 7, attrData);
    print("edx", fila + 9, col + 2, attrLabels); 
    print_hex(registers->edx, 8, col + 6, fila + 9, attrData);
    print("esi", fila + 11, col + 2, attrLabels); 
    print_hex(registers->esi, 8, col + 6, fila + 11, attrData);
    print("edi", fila + 13, col + 2, attrLabels); 
    print_hex(registers->edi, 8, col + 6, fila + 13, attrData);
    print("ebp", fila + 15, col + 2, attrLabels); 
    print_hex(registers->ebp, 8, col + 6, fila + 15, attrData);
    print("esp", fila + 17, col + 2, attrLabels); 
    print_hex(registers->esp, 8, col + 6, fila + 17, attrData);

    print("eip", fila + 19, col + 2, attrLabels); 
    print_hex(registers->eip, 8, col + 6, fila + 19, attrData);
    print("cs", fila + 21, col + 3, attrLabels); 
    print_hex(registers->cs, 4, col + 6, fila + 21, attrData);
    print("ds", fila + 23, col + 3, attrLabels); 
    print_hex(registers->ds, 4, col + 6, fila + 23, attrData);
    print("es", fila + 25, col + 3, attrLabels); 
    print_hex(registers->es, 4, col + 6, fila + 25, attrData);
    print("fs", fila + 27, col + 3, attrLabels); 
    print_hex(registers->fs, 4, col + 6, fila + 27, attrData);
    print("gs", fila + 29, col + 3, attrLabels); 
    print_hex(registers->gs, 4, col + 6, fila + 29, attrData);
    print("ss", fila + 31, col + 3, attrLabels); 
    print_hex(registers->ss, 4, col + 6, fila + 31, attrData);
    print("eflags", fila + 33, col + 3, attrLabels); 
    print_hex(registers->eflags, 8, col + 9, fila + 33, attrData); 

    print("cr0", fila + 3, col + 16, attrLabels); 
    print_hex(registers->esp0, 8, col + 20, fila + 3, attrData);
    print("cr2", fila + 5, col + 16, attrLabels);  
    print_hex(registers->esp1, 8, col + 20, fila + 5, attrData);
    print("cr3", fila + 7, col + 16, attrLabels);  
    print_hex(registers->cr3, 8, col + 20, fila + 7, attrData);
    print("cr4", fila + 9, col + 16, attrLabels);  
    print_hex(registers->esp2, 8, col + 20, fila + 9, attrData);

    print("stack", fila + 20, col + 16, attrLabels); 
    int stackElementsRow = fila + 21;
    int i;

    for (i = 0; i < MIN(10, (registers->esp - registers->ebp)); ++i) {
        print_hex(*((int *)(registers->esp + i)), 8, col + 16, stackElementsRow + i, attrData);
    }
}

ca backup[VIDEO_FILS][VIDEO_COLS];
uchar showDebug = 0;

void screen_flip_debug(uint exception, const char *message, const tss *registers) {
    showDebug = BIT_FLIP(showDebug, 0);

    if (showDebug == 1) {
        uint x, y;

        for (x = 0; x < VIDEO_COLS; ++x) {
            for (y = 0; y < VIDEO_FILS; ++y) {
                backup[y][x].c = p[y][x].c;
                backup[y][x].a = p[y][x].a;
            }
        }

        screen_show_debug(exception, message, registers);
    } else {
        uint x, y;

        for (x = 0; x < VIDEO_COLS; ++x) {
            for (y = 0; y < VIDEO_FILS; ++y) {
                p[y][x].c = backup[y][x].c;
                p[y][x].a = backup[y][x].a;
            }
        }
    }
}
