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

void screen_foreground(uchar color, uint fila, uint columna) {
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

// gdt_entry gdt[GDT_COUNT] = {
//     [GDT_IDX_NULL_DESC] = (gdt_entry) {

char *logo[] = {
    [17] = "     \\  \"      \",/ /    -  ( <    |\\_)",
    [10] = "             ____________._>           \\\\_\\\\____ ,--.__",
    [2] = "                          -      ||||||  .",
    [14] = "     | \"   \"   \"          \\         /                '\\,  /",
    [19] = "   -- -'-;.__:-'",
    [8] = "                            /        \\ \\ \\",
    [12] = "       /  \"             |      .-'     /          \\      '\\",
    [6] = "                        -       .'      \\",
    [7] = "                             .-'    | | |",
    [4] = "                                ||||||| -/",
    [15] = "     |           \" , =_____`-.   .-'_________________,--\"\"",
    [3] = "                              -  ||||||   >",
    [1] = "                                  |||||||||",
    [18] = "      \\   \",\",_/,-'        |\\_)",
    [0] = "                              -    .|||||.",
    [16] = "   - |  \"    \"    /\"/'      /\\>-' ( <",
    [9] = "              --        -  |      `---:.`.\\",
    [11] = "  --    ,--\"\"           /    `-   .     |)_)    '\\     '\\",
    [13] = "     ,/                  \\           .'            '\\     |",
    [5] = "                           --   ||||||'("
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

        if (from >= strlen(logo[i])) {
            continue;
        }

        printS((logo[i] + from), fila + i,  columna, 0xF);
    }
}

void screen_refresh_logo() {
    uint clocknegro = 0;
    uint fila = 5;
    uint columna = 0;
    int onda = -3;
    _Bool sw = 1;
    uint timec = 5000;

    while (1) {
        if (clocknegro % (timec - 1) == 0) {
            screen_pintar_rect(0, 0, 0, 0, 27, min(VIDEO_COLS, (columna + 60)));
        }

        if (clocknegro % timec == 0) {
            screen_printear_ascii(fila + (onda/2), columna, 0);

            if (sw) { onda++; } else { onda--; }
            if (onda == 3 || onda == -3) sw = !sw;
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
    unsigned int eaxR = reax();
    unsigned int eflagsR;
    unsigned int eipR;
    unsigned int espR;
    unsigned int ebpR;

    __asm __volatile("add %0, %%esp" : : "i" (0x000000ec));
    __asm __volatile("pop %0" : "=r" (ebpR): );
    __asm __volatile("pop %0": "=r" (eipR) : );
    __asm __volatile("pop %0": "=r" (eflagsR) : );
    __asm __volatile("mov %%esp, %0" : "=r" (espR): );
    __asm __volatile("push %0": : "r" (eflagsR) );
    __asm __volatile("push %0": : "r" (eipR) );
    __asm __volatile("push %0": : "r" (ebpR) );
    __asm __volatile("sub %0, %%esp" : : "i" (0x000000ec));

    eipR -= 1;      // Tamaño de la operacion: "pushf".
    eipR -= 5;      // Tamaño de la operacion: "call print_state". 

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

    unsigned int espTemp = espR;
    unsigned int stackArr[11];
    int x;
    for (x = 0; espTemp < ebpR && x < 12; x++){
        __asm __volatile("mov (%0), %%eax" : : "r" (espTemp));
        __asm __volatile("mov %%eax, %0" : "=r" (stackArr[x]) :);
        espTemp += 4;
    }

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
    print_hex(eaxR, 8, col + 6, fila + 3, attrData);
    print("ebx", fila + 5, col + 2, attrLabels); 
    print_hex(ebxR, 8, col + 6, fila + 5, attrData);
    print("ecx", fila + 7, col + 2, attrLabels); 
    print_hex(ecxR, 8, col + 6, fila + 7, attrData);
    print("edx", fila + 9, col + 2, attrLabels); 
    print_hex(edxR, 8, col + 6, fila + 9, attrData);
    print("esi", fila + 11, col + 2, attrLabels); 
    print_hex(esiR, 8, col + 6, fila + 11, attrData);
    print("edi", fila + 13, col + 2, attrLabels); 
    print_hex(ediR, 8, col + 6, fila + 13, attrData);
    print("ebp", fila + 15, col + 2, attrLabels); 
    print_hex(ebpR, 8, col + 6, fila + 15, attrData);
    print("esp", fila + 17, col + 2, attrLabels); 
    print_hex(espR, 8, col + 6, fila + 17, attrData);

    print("eip", fila + 19, col + 2, attrLabels); 
    print_hex(eipR, 8, col + 6, fila + 19, attrData);
    print("cs", fila + 21, col + 3, attrLabels); 
    print_hex(csR, 4, col + 6, fila + 21, attrData);
    print("ds", fila + 23, col + 3, attrLabels); 
    print_hex(dsR, 4, col + 6, fila + 23, attrData);
    print("es", fila + 25, col + 3, attrLabels); 
    print_hex(esR, 4, col + 6, fila + 25, attrData);
    print("fs", fila + 27, col + 3, attrLabels); 
    print_hex(fsR, 4, col + 6, fila + 27, attrData);
    print("gs", fila + 29, col + 3, attrLabels); 
    print_hex(gsR, 4, col + 6, fila + 29, attrData);
    print("ss", fila + 31, col + 3, attrLabels); 
    print_hex(ssR, 4, col + 6, fila + 31, attrData);
    print("eflags", fila + 33, col + 3, attrLabels); 
    print_hex(eflagsR, 8, col + 9, fila + 33, attrData); 

    print("cr0", fila + 3, col + 16, attrLabels); 
    print_hex(cr0R, 8, col + 20, fila + 3, attrData);
    print("cr2", fila + 5, col + 16, attrLabels);  
    print_hex(cr2R, 8, col + 20, fila + 5, attrData);
    print("cr3", fila + 7, col + 16, attrLabels);  
    print_hex(cr3R, 8, col + 20, fila + 7, attrData);
    print("cr4", fila + 9, col + 16, attrLabels);  
    print_hex(cr4R, 8, col + 20, fila + 9, attrData);

    print("stack", fila + 20, col + 16, attrLabels); 
    int stackElementsRow = fila + 21;
    int i;
    for (i = 0; i < x; i++){
        print_hex(stackArr[i], 8, col + 16, stackElementsRow + i, attrData);
    }

    //Restauro registros no protegidos por convencion
    __asm __volatile("mov %0,%%ecx" : : "r" (ecxR));
    __asm __volatile("mov %0,%%edx" : : "r" (edxR));
    __asm __volatile("mov %0,%%eax" : : "r" (eaxR)); 
}