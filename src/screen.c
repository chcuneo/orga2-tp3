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
                return;
            }
        }

        columna %= VIDEO_COLS;
        clocknegro++;
    } 
}

void screen_pintar_puntajes() {

}

//TODO:
//- Aprender a calcular realmente 0x000000a4, yo lo saque de la instruccion post ensamblado.
//- EFLAGS es vulnerable al "sub" del stack frame, nose si va a haber que mandarlo por el stack.
//- Pasar numeros a algo mas lindo
void print_state(){
    // Explicacion:
    // Las instrucciones que se realizan por gcc luego de realizar "call print_state" desde asm son:
    //     push ebp
    //     mov ebp, esp
    //     sub esp, 0x000000a4
    //
    // Entonces hago el reverso para obtener esp, ebp y eip.
    // Luego dejo todo como debe estar y sigo
    //
    // eax lo guardo primero ya que las llamadas a asm desde c lo pisan, porque el output lo mandan por eax.
    __asm __volatile("nop" :: );
    unsigned int eaxR = reax();
    unsigned int eflagsR = reflags();
    unsigned int eipR;
    unsigned int espR;
    unsigned int ebpR;

    __asm __volatile("add %0, %%esp" : : "i" (0x000000a8));
    __asm __volatile("pop %0" : "=r" (ebpR): );
    __asm __volatile("pop %0": "=r" (eipR) : );
    __asm __volatile("mov %%esp, %0" : "=r" (espR): );
    __asm __volatile("push %0": : "r" (eipR) );
    __asm __volatile("push %0": : "r" (ebpR) );
    __asm __volatile("sub %0, %%esp" : : "i" (0x000000a8));

    eipR -= 5;    // Tamaño de la operacion: "call print_state". La direccion de ret es la siguiente a "call print_state"

    unsigned int ebxR = rebx();
    unsigned int ecxR = recx();
    unsigned int edxR = redx();
    unsigned int esiR = resi();
    unsigned int ediR = redi();
    unsigned int csR = rcs();
    unsigned int dsR = rds();
    unsigned int esR = res();
    unsigned int fsR = rfs();
    unsigned int gsR = rgs();
    unsigned int ssR = rss();
    unsigned int cr0R = rcr0();
    unsigned int cr2R = rcr2();
    unsigned int cr3R = rcr3();
    unsigned int cr4R = rcr4();

    screen_pintar_linea_h(' ', 0x00, 6, 25, 30);
    screen_pintar_linea_v(' ', 0x00, 7, 25, 34);
    screen_pintar_linea_h(' ', 0x00, 41, 25, 30);
    screen_pintar_linea_v(' ', 0x00, 7, 54, 34);
    screen_pintar_linea_h(' ', 0x44, 7, 26, 28);
    screen_pintar_rect(' ', 0x70, 8, 26, 33, 28);

    unsigned short attrData = 0x7F;
    unsigned short attrLabels = 0x70;
    print("eax", 9, 27, attrLabels);
    print_hex(eaxR, 8, 31, 9, attrData);
    print("ebx", 11, 27, attrLabels); 
    print_hex(ebxR, 8, 31, 11, attrData);
    print("ecx", 13, 27, attrLabels); 
    print_hex(ecxR, 8, 31, 13, attrData);
    print("edx", 15, 27, attrLabels); 
    print_hex(edxR, 8, 31, 15, attrData);
    print("esi", 17, 27, attrLabels); 
    print_hex(esiR, 8, 31, 17, attrData);
    print("edi", 19, 27, attrLabels); 
    print_hex(ediR, 8, 31, 19, attrData);
    print("ebp", 21, 27, attrLabels); 
    print_hex(ebpR, 8, 31, 21, attrData);
    print("esp", 23, 27, attrLabels); 
    print_hex(espR, 8, 31, 23, attrData);

    print("eip", 25, 27, attrLabels); 
    print_hex(eipR, 8, 31, 25, attrData);
    print("cs", 27, 28, attrLabels); 
    print_hex(csR, 4, 31, 27, attrData);
    print("ds", 29, 28, attrLabels); 
    print_hex(dsR, 4, 31, 29, attrData);
    print("es", 31, 28, attrLabels); 
    print_hex(esR, 4, 31, 31, attrData);
    print("fs", 33, 28, attrLabels); 
    print_hex(fsR, 4, 31, 33, attrData);
    print("gs", 35, 28, attrLabels); 
    print_hex(gsR, 4, 31, 35, attrData);
    print("ss", 37, 28, attrLabels); 
    print_hex(ssR, 4, 31, 37, attrData);
    print("eflags", 39, 28, attrLabels); 
    print_hex(eflagsR, 8, 34, 39, attrData); 

    print("cr0", 9, 41, attrLabels); 
    print_hex(cr0R, 8, 45, 9, attrData);
    print("cr2", 11, 41, attrLabels);  
    print_hex(cr2R, 8, 45, 11, attrData);
    print("cr3", 13, 41, attrLabels);  
    print_hex(cr3R, 8, 45, 13, attrData);
    print("cr4", 15, 41, attrLabels);  
    print_hex(cr4R, 8, 45, 15, attrData);
}