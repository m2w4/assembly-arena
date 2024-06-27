/*
  ╔══════════════════════════════════════════════════════════╗
  ║                       api.h
  ║ defines the interface for native C applications and
  ║ the built-in CLI interface used for fuzzing and IPC
  ║ when compiled with emcc.
  ║
  ╚══════════════════════════════════════════════════════════╝
*/

#ifndef API_H
#define API_H

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include "config.h"
#include "sheduler.h"
#include "util.h"

#define IS_RUNNING    0
#define WARRIOR_1_WON 1
#define WARRIOR_2_WON 2
#define WARRIOR_DRAW  3


//battle class for web-based applications
/* ┌─ ring_t                                                ───┐
 * │  battle struct containing everything for a running game.
 * │  Look at vm.h for vm_t & ring_t references.
 * │
 * ├─ Members
 * ├ warriors: structs of players
 * ├ ring_t:   ptr to arena
 * ├ has_ended: 1 or 2 if ended. 0 if running.
 * ├ curr_iteration: nr of executed instructions
 * └───                                                     ───┘
 */
typedef struct {
    vm_t *warrior1;
    vm_t *warrior2;
    ring_t *ring;
    int   has_ended;
    int   curr_iteration;
} battle_t;


//CLI
//Usage: ./vm [c|p] YQ== Yg==

/* ╔═ initBattle  ═══════════════════════════════════════════════════╗
 * ║
 * ║ Initializes a battle_t object. Mostly used for the following CLI:
 * ║ ./vm [c|p] YQ== Yg==
 * ║
 * ║ parameters:
 * ║ - warriors: base64-encoded warriors (example: YQ==, Yg==)
 * ║
 * ║ returns:
 * ║ - battle_t object to work with
 * ║
 * ╚═════════════════════════════════════════════════════════════════╝
 */
battle_t* initBattle( char* warrior1_b64, char* warrior2_b64 );

/* ╔═ playBattleCLI  ═════════════════════════════════════════════════════════╗
 * ║
 * ║ Calculates a game and returns game output. If compiled with INTERACTIVE
 * ║ flag, it visualizes the whole game and every instruction.
 * ║
 * ║ parameters:
 * ║ - battle_t: battle to play
 * ║
 * ║ returns:
 * ║ - game outcome. 1 if warrior1 wins, 2 if warrior2 wins. 0 when draw.
 * ║
 * ╚═════════════════════════════════════════════════════════════════════════╝
 */
int       playBattleCLI( battle_t *battle );

//Web (look at api_wasm.c)
#ifdef __EMSCRIPTEN__
battle_t* EMSCRIPTEN_KEEPALIVE initWebGame(char* warrior1_b64, char* warrior2_b64);
char*     EMSCRIPTEN_KEEPALIVE calculateWebRound(battle_t* battle);
#endif

#endif
