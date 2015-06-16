/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "mmu.h"
#include "tss.h"
#include "screen.h"
#include "colors.h"
#include "error.h"
#include "gdt.h"
#include "defines.h"
#include "i386.h"


#define POS_INIT_A_X                      1
#define POS_INIT_A_Y                      1
#define POS_INIT_B_X         MAPA_ANCHO - 2
#define POS_INIT_B_Y          MAPA_ALTO - 2

#define CANT_POSICIONES_VISTAS            9
#define MAX_SIN_CAMBIOS                 999

#define BOTINES_CANTIDAD 8

const char pClock[] = "|/-\\";
#define pClock_size 4

uint botines[BOTINES_CANTIDAD][3] = { // TRIPLAS DE LA FORMA (X, Y, MONEDAS)
                                        {30,  3, 50}, {30, 38, 50}, {15, 21, 100}, {45, 21, 100} ,
                                        {49,  3, 50}, {70, 40, 50}, {64, 21, 100}, {34, 21, 100}
                                    };

jugador_t jugadorA;
jugador_t jugadorB;

inline uint game_xy2lineal (uint x, uint y) {
	return (y * MAPA_ANCHO + x);
}

inline uint game_posicion_valida(int x, int y) {
	return (x >= 0 && y >= 0 && x < MAPA_ANCHO && y < MAPA_ALTO);
}

pirata_t* id_pirata2pirata(uint pirate_id) {
    uint player = pirate_id / MAX_CANT_PIRATAS_VIVOS;
    uint pirate = pirate_id - player*MAX_CANT_PIRATAS_VIVOS;
    if (pirate_id < (MAX_CANT_PIRATAS_VIVOS << 1)) {
        jugador_t *p;

        switch (player) {
            case 0:
                p = &jugadorA;
                break;
            case 1:
                p = &jugadorB;
                break;
            default:
                p = NULL;
                break;
        }

        if (p != NULL && p->piratas[pirate].exists) {
            return &(p->piratas[pirate]);
    	}
	}
    return NULL;
}

uint game_dir2xy(direccion dir, int *x, int *y) {
	switch (dir) {
		case IZQ: *x = -1; *y =  0; break;
		case DER: *x =  1; *y =  0; break;
		case ABA: *x =  0; *y =  1; break;
		case ARR: *x =  0; *y = -1; break;
    	default: return -1;
	}

	return 0;
}

inline uint game_xy2addressVirt(int x, int y){
	return MAPA_BASE_VIRTUAL + (game_xy2lineal(x, y) * PAGE_SIZE);
}

inline uint game_xy2addressPhys(int x, int y){
	return MAPA_BASE_FISICA + (game_xy2lineal(x, y) * PAGE_SIZE);
}

inline uint game_pirateIdtoDirectoryAddress(uint id){
	return DIRECTORY_TABLE_PHYS + id * PAGE_SIZE;
}

uint game_valor_tesoro(uint x, uint y) {
	int i;

	for (i = 0; i < BOTINES_CANTIDAD; i++) {
		if (botines[i][0] == x && botines[i][1] == y) {
			return botines[i][2];
        }
	}

	return 0;
}

void game_jugador_setBitMapPos(jugador_t *j, uint x, uint y, uchar val){
	uint pos = game_xy2lineal(x,y);
	uint charInBMArray = pos / 8;
	uint offsetInChar = pos % 8;
	if (val == 1){
		j->map[charInBMArray] = BIT_SET(j->map[charInBMArray], offsetInChar);
	} else {
		j->map[charInBMArray] = BIT_UNSET(j->map[charInBMArray], offsetInChar);
	}
}

char game_jugador_getBitMapPos(jugador_t *j, uint x, uint y){
	uint pos = game_xy2lineal(x,y);
	uint charInBMArray = pos / 8;
	uint offsetInChar = pos % 8;
	return BIT_ISSET(j->map[charInBMArray], offsetInChar);
}

void game_inicializar(){
	game_jugador_inicializar(&jugadorA, 0, POS_INIT_A_X, POS_INIT_A_Y);
	game_jugador_inicializar(&jugadorB, 1, POS_INIT_B_X, POS_INIT_B_Y);
	int x;
	for (x = 0; x < MAX_CANT_PIRATAS_VIVOS * 2; x++) game_tick(GDT_IDX_START_TSKS + x);
}

void game_jugador_inicializar(jugador_t *j, uint idx, uint x, uint y) {
	j->score = 0;
	j->miners = 0;
	j->explorers = 0;
	j->port_coord_x = x;
	j->port_coord_y = y;
	j->index = idx;

	uint i;

	for (i = 0; i < MAX_CANT_PIRATAS_VIVOS; i++) {
		j->piratas[i].exists = 0;
	}

	for (i = 0; i < BIT_SIZE(MAPA_ANCHO, MAPA_ALTO); ++i) {
		j->map[i] = 0;
	}

	game_explorar_posicion(j, j->port_coord_x, j->port_coord_y);
}


void game_explorar_posicion(jugador_t *jugador, int c, int f){
	int x, y;
	int xstart = MAX(c-1, 0);
	int ystart = MAX(f-1, 0);
	
	int xend = MIN(c+1, MAPA_ANCHO - 1);
	int yend = MIN(f+1, MAPA_ALTO - 1);

	for (x = xstart; x <= xend; x++){
		for (y = ystart; y <= yend; y++){
			if (game_jugador_getBitMapPos(jugador, x, y) == 0){
				int p;
				for (p = 0; p < MAX_CANT_PIRATAS_VIVOS; p++){
					if (jugador->piratas[p].exists) {
						game_pirata_paginarPosMapa(&(jugador->piratas[p]), x, y);
					}
				}

				game_jugador_setBitMapPos(jugador, x, y, 1);

				if (game_valor_tesoro(x, y)) {
					game_jugador_lanzar_pirata(jugador, MINERO, x, y);
					screen_changechar('o', x, y + 1 ); 
				}

			}
			game_updateScreen(0, jugador, x, y);
		}
	}
}

int game_jugador_taskAdress(jugador_t *j, pirata_t *p){
	if (j->index == 0){
		if (p->type == EXPLORADOR){ return 0x10000; } else { return 0x11000; }
	} else {
		if (p->type == EXPLORADOR){ return 0x12000; } else { return 0x13000; }
	}
}

void game_pirata_paginarPosMapa (pirata_t *p, int x, int y){
	mmap(game_xy2addressVirt(x, y), game_xy2addressPhys(x, y), game_pirateIdtoDirectoryAddress(p->id), 0, 1);
}

int game_jugador_lanzar_pirata(jugador_t *j, uint tipo, uint x_target, uint y_target) {
	uint i;

	for (i = 0; i < MAX_CANT_PIRATAS_VIVOS; ++i) {
		if (j->piratas[i].exists == 0) {
			break;
		}
	}

	if (i == MAX_CANT_PIRATAS_VIVOS) {
		return E_PIRATE_LIMIT_REACHED;
	}

	pirata_t *pirate = &(j->piratas[i]);

	pirate->index = i;
	pirate->jugador = j;
	pirate->id = i + j->index * MAX_CANT_PIRATAS_VIVOS;
	pirate->coord_x = j->port_coord_x;
	pirate->coord_y = j->port_coord_y;
	pirate->exists = 1;
	pirate->type = tipo;
	pirate->clock = 0;
	
	uint x, y;

	for (x = 0; x < MAPA_ANCHO; ++x) {
		for (y = 0; y < MAPA_ALTO; ++y) {
			if (game_jugador_getBitMapPos(j, x, y)) {
				game_pirata_paginarPosMapa(pirate, x, y);
			}
		}
	}

	switch (tipo) {
		case EXPLORADOR:
			j->explorers++;
			break;
		case MINERO:
			j->miners++;
			break;
		default:
			return E_OUT_OF_BOUNDS;
			break;
	}

	tss_reset_StateSegment(pirate->id);
	mmu_inicializar_dir_pirata(
		game_pirateIdtoDirectoryAddress(pirate->id),
		game_jugador_taskAdress(j, pirate),
		game_xy2addressPhys(j->port_coord_x, j->port_coord_y));
	game_updateScreen(pirate, pirate->jugador, pirate->coord_x, pirate->coord_y);	

	//ESTO LOGRA PASARLE DOS PARAMETROS A UN EXPLORADOR, pero no a un minero, es que tiene mas variables locales, y reserva esos espacios para eso y que se yo
	/*int *pstack = (int *)(game_xy2addressPhys(pirate->coord_x, pirate->coord_y) + 0x1000 - 12);
	pstack += 1;
	*pstack = x_target;
	pstack += 1;
	*pstack = y_target;*/

	return E_OK;
}

void game_updateScreen(pirata_t *p, jugador_t *j, int x, int y){
	int xc = x;
	int yc = y + TOP_MARGIN;
	uchar color;
	if (p){
		if (p->exists == 0) { 
			if (j->index == 0){
				color = C_BG_BLACK | C_FG_LIGHT_RED;
			} else {
				color = C_BG_BLACK | C_FG_LIGHT_BLUE;
			}
		} else {
			if (j->index == 0){
				color = C_BG_RED | C_FG_WHITE;
			} else {
				color = C_BG_BLUE | C_FG_WHITE;
			}
		}
		if (p->type == MINERO) { 
			screen_pintar('M', color, yc, xc ); 
		} else { 
			screen_pintar('E', color, yc, xc ); 
		}
	} else {
		if (j->index == 0){
			color = C_BG_GREEN;
		} else {
			color = C_BG_CYAN;
		}
		screen_changecolor(color, xc, yc);
	}
}

int game_syscall_pirata_mover(uint id, direccion dir){
	pirata_t * pirate = id_pirata2pirata(id);

	if (pirate == NULL) {
		return E_NON_EXISTANT_PIRATE;
	} else {
		int x, y;
		game_dir2xy(dir, &x, &y);
		x += pirate->coord_x;
		y += pirate->coord_y;

		// solo permitir que el explorador se mueva si la posicion ya ha sido explorada
		if (game_posicion_valida(x, y) & (pirate->type == EXPLORADOR || game_jugador_getBitMapPos(pirate->jugador, x, y))) {

			// restaurar posicion anterior
			game_updateScreen(0, pirate->jugador, pirate->coord_x, pirate->coord_y);
			if (game_valor_tesoro(pirate->coord_x, pirate->coord_y))
			screen_changechar('o', pirate->coord_x, pirate->coord_y + 1); // le sumo el upper border.

			uint directoryBase = game_pirateIdtoDirectoryAddress(id);
			remap(directoryBase, CODIGO_BASE, game_xy2addressPhys(x, y));
			mmu_move_codepage(directoryBase, game_xy2addressVirt(pirate->coord_x, pirate->coord_y), CODIGO_BASE);
			if (pirate->type == EXPLORADOR) game_explorar_posicion(pirate->jugador, x, y);
			game_updateScreen(pirate, pirate->jugador, x, y);
			pirate->coord_x = x;
			pirate->coord_y = y;

			return E_OK;
		} else {
			return E_OUT_OF_BOUNDS;
		}
	}
}

int game_syscall_cavar(uint pirateId) {
    static uint treasuresLeft = BOTINES_CANTIDAD;
    pirata_t *p = id_pirata2pirata(pirateId);

    if (p == NULL) {
        return E_NON_EXISTANT_PIRATE;
    } else {
        uint i;

        for (i = 0; i < BOTINES_CANTIDAD; ++i) {
            if (botines[i][0] == p->coord_x && botines[i][1] == p->coord_y) {
                if (botines[i][2] > 0) {
                    p->jugador->score++;
                    botines[i][2]--;
                    screen_pintar_puntajes();
                } else {
                    // TODO: revisar que usar esta funcion este bien
                    game_pirata_exploto(pirateId);
                    --treasuresLeft;
                }

                break;
            }
        }
    }

    if (treasuresLeft == 0) {
        // TODO: asegurarse que esto ande
        game_terminar();
    }

	return E_OK;
}

int game_syscall_pirata_posicion(uint pirate_id, int param) {
    pirata_t *p = id_pirata2pirata(pirate_id);
    int code;

    if (p == NULL) {
        code = E_NON_EXISTANT_PIRATE;
    } else {
        if (param == -1) {
            code = (p->coord_y << 8) | p->coord_x;
        } else {
            if (param < MAX_CANT_PIRATAS_VIVOS) {
                pirata_t *q = &(p->jugador->piratas[param]);

                if (q->exists) {
                    code = (q->coord_y << 8) | q->coord_x;
                } else {
                    code = E_NON_EXISTANT_PIRATE;
                }
            } else {
                code = E_NON_EXISTANT_PIRATE;
            }
        }
    }

    return code;
}

void game_pirata_exploto(uint id) {
	pirata_t *pirate = id_pirata2pirata(id);
	pirate->exists = 0;
	munmap(game_pirateIdtoDirectoryAddress(id), CODIGO_BASE);
	game_updateScreen(pirate, pirate->jugador, pirate->coord_x, pirate->coord_y);
}

void game_jugador_anotar_punto(jugador_t *j) {
	j->score++;
}

#define PCLOCK_ROW (VIDEO_FILS - 2)
#define PCLOCK_COL0_JA 4
#define PCLOCK_COL0_JB 59

void game_tick(uint taskid){
	uint pirateid = taskid - GDT_IDX_START_TSKS;
	uint playerindex = pirateid / 8;
	uint pirateindex = pirateid % 8;
	if (playerindex == 0){
		if (jugadorA.piratas[pirateindex].exists == 1){
			screen_pintar(pClock[(jugadorA.piratas[pirateindex].clock)], C_FG_WHITE, PCLOCK_ROW, PCLOCK_COL0_JA + pirateindex * 2);
			jugadorA.piratas[pirateindex].clock++;
			jugadorA.piratas[pirateindex].clock = jugadorA.piratas[pirateindex].clock % pClock_size;
		} else {
			screen_pintar('x', C_FG_RED, PCLOCK_ROW, PCLOCK_COL0_JA + pirateindex * 2);
		}
	} else {
		if (jugadorB.piratas[pirateindex].exists == 1){
			screen_pintar(pClock[jugadorB.piratas[pirateindex].clock], C_FG_WHITE, PCLOCK_ROW, PCLOCK_COL0_JB + pirateindex * 2);
			jugadorB.piratas[pirateindex].clock++;
			jugadorB.piratas[pirateindex].clock = jugadorB.piratas[pirateindex].clock % pClock_size;
		} else {
			screen_pintar('x', C_FG_BLUE, PCLOCK_ROW, PCLOCK_COL0_JB + pirateindex * 2);
		}
	}
}

void game_terminar() {

}

void game_terminar_si_es_hora() {

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

void game_atender_teclado(unsigned char tecla){
	switch (tecla){
		case KB_shiftA:
			game_jugador_lanzar_pirata(&jugadorA, EXPLORADOR, 0, 0);
			break;
		case KB_shiftB:
			game_jugador_lanzar_pirata(&jugadorB, EXPLORADOR, 0, 0);
			break;
		default:
			break;
	}
}

uchar paused = 0;

void game_pause_toggle() {
	paused = BIT_FLIP(paused, 0);
}

uchar game_is_paused() {
	return paused;
}
