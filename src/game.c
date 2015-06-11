/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "mmu.h"
#include "tss.h"
#include "screen.h"
#include "error.h"
#include "gdt.h"
#include "defines.h"


#define POS_INIT_A_X                      1
#define POS_INIT_A_Y                      1
#define POS_INIT_B_X         MAPA_ANCHO - 2
#define POS_INIT_B_Y          MAPA_ALTO - 2

#define CANT_POSICIONES_VISTAS            9
#define MAX_SIN_CAMBIOS                 999

#define BOTINES_CANTIDAD 8



uint botines[BOTINES_CANTIDAD][3] = { // TRIPLAS DE LA FORMA (X, Y, MONEDAS)
                                        {30,  3, 50}, {30, 38, 50}, {15, 21, 100}, {45, 21, 100} ,
                                        {49,  3, 50}, {49, 38, 50}, {64, 21, 100}, {34, 21, 100}
                                    };

jugador_t jugadorA;
jugador_t jugadorB;


void* error() {
	__asm__ ("int3");
	return 0;
}

uint game_xy2lineal (uint x, uint y) {
	return (y * MAPA_ANCHO + x);
}

uint game_posicion_valida(int x, int y) {
	return (x >= 0 && y >= 0 && x < MAPA_ANCHO && y < MAPA_ALTO);
}

pirata_t* id_pirata2pirata(uint pirate_id) {
    if (pirate_id < (MAX_CANT_PIRATAS_VIVOS << 1)) {
        uint player = pirate_id / MAX_CANT_PIRATAS_VIVOS;
        uint pirate = pirate_id - player*MAX_CANT_PIRATAS_VIVOS;
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

uint game_xy2addressVirt(int x, int y){
	return MAPA_BASE_VIRTUAL + (((x * MAPA_ANCHO) + y) * 0x1000);
}

uint game_xy2addressPhys(int x, int y){
	return MAPA_BASE_FISICA + (((x * MAPA_ANCHO) + y) * 0x1000);
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

// dada una posicion (x,y) guarda las posiciones de alrededor en dos arreglos, uno para las x y otro para las y
void game_calcular_posiciones_vistas(int *vistas_x, int *vistas_y, int x, int y) {
	int next = 0;
	int i, j;

	for (i = -1; i <= 1; i++) {
		for (j = -1; j <= 1; j++) {
			vistas_x[next] = x + j;
			vistas_y[next] = y + i;
			next++;
		}
	}
}

void game_inicializar(){
	game_jugador_inicializar(&jugadorA, 0, POS_INIT_A_X, POS_INIT_A_Y);
	game_jugador_inicializar(&jugadorB, 1, POS_INIT_B_X, POS_INIT_B_Y);
}

void game_jugador_inicializar_mapa(jugador_t *jug) {
	int bitmapSize = MAPA_ANCHO * MAPA_ALTO / 8;
	int x;

	for (x = 0; x < bitmapSize; ++x) {
		jug->map[x] = 0;
	}
	game_explorar_posicion(jug, jug->port_coord_x, jug->port_coord_y);
}

void game_jugador_inicializar(jugador_t *j, uint idx, uint x, uint y) {
	j->score = 0;
	j->miners = 0;
	j->explorers = 0;
	j->port_coord_x = x;
	j->port_coord_y = y;
	j->index = idx;
	int i;
	for (i = 0; i < MAX_CANT_PIRATAS_VIVOS; i++) j->piratas[i].exists = 0;
	game_jugador_inicializar_mapa(j);
}

void game_tick(uint id_pirata){
}


void game_pirata_relanzar(pirata_t *pirata, jugador_t *j, uint tipo) {
}

void game_pirata_inicializar(pirata_t *pirata, jugador_t *j, uint index, uint id) {
	pirata->index = index;
	pirata->jugador = j;
	pirata->id = id;
	pirata->coord_x = j->port_coord_x;
	pirata->coord_y = j->port_coord_y;
	
	//Esto se puede mover a mmu.c, pero creo que deberia quedar aca, mi corazon dice que aqui pertenece
	int x, y;
	for (x = 0; x < MAPA_ALTO; x++){
		for (y = 0; y < MAPA_ANCHO; y++){
			if (game_jugador_getBitMapPos(j, x, y)) game_pirata_paginarPosMapa(pirata, x, y);
		}
	}
}

pirata_t* game_jugador_erigir_pirata(jugador_t *j, uint tipo){
	int i;
	for (i = 0; i < MAX_CANT_PIRATAS_VIVOS; i++){
		if (j->piratas[i].exists == 0){
			game_pirata_inicializar(&(j->piratas[i]), j, i, j->index * MAX_CANT_PIRATAS_VIVOS + i);
			if (tipo == EXPLORADOR) { j->explorers++; } else { j->miners++; }
			return &(j->piratas[i]);
		}
	}
	return NULL;
}


void game_jugador_lanzar_pirata(jugador_t *j, uint tipo){
	pirata_t *pirate = game_jugador_erigir_pirata(j, tipo);
	if (pirate){
		uint taskaddrs = 0x0; //TODO: Setear bien la direccion del task

		mmu_inicializar_dir_pirata(DIRECTORY_TABLE_PHYS + pirate->id * PAGE_SIZE, taskaddrs, game_xy2addressPhys(j->port_coord_x, j->port_coord_y));
		gdt[GDT_IDX_START_TSKS + pirate->id].p = 0x01;	//TODO: ver si esto se hace o no

		pirate->exists = 1;
	}
}

void game_pirata_habilitar_posicion(jugador_t *j, pirata_t *pirata, int x, int y)
{
}

void game_pirata_paginarPosMapa (pirata_t *p, int x, int y){
	mmap(game_xy2addressVirt(x, y), game_xy2addressPhys(x, y), DIRECTORY_TABLE_PHYS + (p->id * PAGE_SIZE), 0, 0); //TODO: chequear atributos
}

void game_jugador_paginarPosMapa_piratasExistentes (jugador_t *j, int x, int y){
	int p;
	for (p = 0; p < MAX_CANT_PIRATAS_VIVOS; p++){
		if (j->piratas[p].exists) game_pirata_paginarPosMapa(&(j->piratas[p]), x, y);
	}
}

void game_explorar_posicion(jugador_t *jugador, int c, int f){
	int x, y;
	int xstart;
	if (0 > c-1) { xstart = 0; } else { xstart = c-1; }
	int ystart;
	if (0 > f-1) { ystart = 0; } else { ystart = f-1; }
	
	int xend;
	if (MAPA_ANCHO - 1 < c+1) { xend = MAPA_ANCHO - 1; } else { xend = c+1; }
	int yend;
	if (MAPA_ALTO - 1 < f+1) { yend = MAPA_ALTO - 1; } else { ystart = f+1; }

	for (x = xstart; x <= xend; x++){
		for (y = ystart; y <= yend; y++){
			//TODO: ver si este if entero deberia ir dentro de game_jugador_paginarPosMapa_piratasExistentes
			if (game_jugador_getBitMapPos(jugador, x, y) == 0){
				game_jugador_paginarPosMapa_piratasExistentes(jugador, x, y);
				game_jugador_setBitMapPos(jugador, x, y, 1);
			}
		}
	}
}

int game_syscall_pirata_mover(uint id, direccion dir)
{
	int x, y;
	game_dir2xy(dir, &x, &y);
	pirata_t * pirate;
	if (id < MAX_CANT_PIRATAS_VIVOS) { pirate = &(jugadorA.piratas[id]); } else { pirate = &(jugadorB.piratas[id - MAX_CANT_PIRATAS_VIVOS]); }
	int xdst = pirate->coord_x + x, ydst = pirate->coord_y + y;
	if (game_posicion_valida(xdst, ydst)){
		mmu_move_codepage(game_xy2addressPhys(pirate->coord_x, pirate->coord_y), game_xy2addressPhys(xdst, ydst), pirate);
		game_explorar_posicion(pirate->jugador, xdst, ydst);
	}
    return 0;
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
                    game_jugador_anotar_punto(p->jugador);
                    botines[i][2]--;
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

void game_pirata_exploto(uint id)
{
}

pirata_t* game_pirata_en_posicion(uint x, uint y) {
	int i;

	for (i = 0; i < MAX_CANT_PIRATAS_VIVOS; i++){
		if (jugadorA.piratas[i].coord_x == x && jugadorA.piratas[i].coord_y == y){
			return &(jugadorA.piratas[i]);
		}
	}

	for (i = 0; i < MAX_CANT_PIRATAS_VIVOS; i++) {
		if (jugadorB.piratas[i].coord_x == x && jugadorB.piratas[i].coord_y == y){
			return &(jugadorB.piratas[i]);
		}
	}

	return NULL;
}


void game_jugador_anotar_punto(jugador_t *j) {
	j->score++;
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
			game_jugador_lanzar_pirata(&jugadorA, EXPLORADOR);
			break;
		case KB_shiftB:
			game_jugador_lanzar_pirata(&jugadorB, EXPLORADOR);
			break;
		default:
			break;
	}
}
