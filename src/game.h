/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "defines.h"
#include "pic.h"

typedef enum direccion_e { ARR = 0x4, ABA = 0x7, DER = 0xA, IZQ = 0xD} direccion;

#define JUGADOR_A                         0
#define JUGADOR_B                         1

#define EXPLORADOR                        0
#define MINERO                            1

#define MAPA_ANCHO                       80
#define MAPA_ALTO                        44


struct jugador_t;

typedef struct pirata_t
{
    uchar exists:1;
    uint index;                     //Indice en array interno (numero de 0 a 7)
    uint id;                        //Id unica: representa el indice general en el array de TSDs y Directorios de Paginacion
    struct jugador_t *jugador;
    uint coord_x;                   //Coordenadas Actuales
    uint coord_y;
    uint type;                      //Tipo: explorador o minero
    uchar clock;
} pirata_t;

typedef struct jugador_t
{
    uint index;                     //Indice del jugador: A es el 0 y B el 1 en este caso
    pirata_t piratas[MAX_CANT_PIRATAS_VIVOS];
    uint port_coord_x;
    uint port_coord_y;
    char map[BIT_SIZE(MAPA_ALTO, MAPA_ANCHO)];
    uint miners;                    //Cantidad de mineros
    uint explorers;                 //Cantidad de exploradores
    uint score;
} jugador_t;

extern jugador_t jugadorA, jugadorB;

// ~ auxiliares dadas ~
uint game_xy2lineal();
pirata_t* id_pirata2pirata(uint id);


// ~ auxiliares sugeridas o requeridas (segun disponga enunciado) ~
void game_pirata_inicializar(pirata_t *pirata, jugador_t *jugador, uint index, uint id);
void game_pirata_erigir(pirata_t *pirata, jugador_t *j, uint tipo);
void game_pirata_habilitar_posicion(jugador_t *j, pirata_t *pirata, int x, int y);
void game_pirata_exploto(uint id);
void game_jugador_setBitMapPos(jugador_t *j, uint x, uint y, uchar val);
char game_jugador_getBitMapPos(jugador_t *j, uint x, uint y);
void game_pirata_paginarPosMapa (pirata_t *p, int x, int y);
void game_jugador_paginarPosMapa_piratasExistentes (jugador_t *j, int x, int y);
uint game_xy2addressPhys(int x, int y);
uint game_xy2addressVirt(int x, int y);
uint game_posicion_valida(int x, int y);
int game_jugador_taskAdress(jugador_t *j, pirata_t *p);
void game_updateScreen(pirata_t * p, jugador_t * j, int x, int y);
uint game_pirateIdtoDirectoryAddress(uint id);
pirata_t* id_pirata2pirata(uint pirate_id);
uint game_dir2xy(direccion dir, int *x, int *y);

void game_inicializar();
void game_jugador_inicializar(jugador_t *j, uint idx, uint x, uint y);
int game_jugador_lanzar_pirata(jugador_t *j, uint tipo, uint x_target, uint y_target);
pirata_t* game_jugador_erigir_pirata(jugador_t *j, uint tipo);
void game_jugador_anotar_punto(jugador_t *j);
void game_explorar_posicion(jugador_t *jugador, int x, int y);

uint game_valor_tesoro(uint x, uint y);
void game_calcular_posiciones_vistas(int *vistas_x, int *vistas_y, int x, int y);

int game_syscall_pirata_posicion(uint id, int idx);
int game_syscall_pirata_mover(uint id, direccion key);
int game_syscall_cavar(uint id);
void game_terminar();
void game_tick();
void game_atender_teclado(unsigned char tecla);

void game_pause_toggle();
uchar game_is_paused();

#endif
