/*
  ╔══════════════════════════════════════════════════════════╗
  ║                       api_wasm.h
  ║ defines the interface for webasm applications.
  ║
  ╚══════════════════════════════════════════════════════════╝
*/
#ifndef API_WASM_H
#define API_WASM_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <emscripten.h>
#include "config.h"
#include "sheduler.h"
#include "util.h"
#include "vm.h"
#include "api.h"

#define MAX_STRING_LENGTH 65535 // Adjust the size according to your needs

/* ╔═ web_initBattle ════════════════════════════════════╗
 * ║
 * ║ Initialized a battle_t struct to work with.
 * ║
 * ║ parameters:
 * ║ - warrior_b64: base64-encoded compiled warrior code
 * ║
 * ║ returns:
 * ║ - ptr to a battle_t object
 * ║
 * ╚═════════════════════════════════════════════════════╝
 */
extern battle_t* EMSCRIPTEN_KEEPALIVE web_initBattle( char* warrior1_b64, char* warrior2_b64 );

/* ╔═ web_playSingleRound ════════════════════════════════════╗
 * ║
 * ║ Plays a single round / executes one instruction of a battle.
 * ║
 * ║ parameters:
 * ║ - battle: battle object
 * ║
 * ║ returns:
 * ║ - 0: VM running (IS_RUNNING)
 * ║ - 1: Warrior 1 won (WARRIOR_1_WON)
 * ║ - 2: Warrior 2 won (WARRIOR_2_WON)
 * ║ - 3: Game ended with draw.
 * ║
 * ╚═════════════════════════════════════════════════════╝
 */
extern int EMSCRIPTEN_KEEPALIVE web_playSingleRound( battle_t *battle );

/* ╔═ web_printCore ════════════════════════════════════╗
 * ║
 * ║ Returns renderable HTML-code of the current Arena.
 * ║ Includes colors by ownership.
 * ║
 * ║ parameters:
 * ║ - battle: battle object
 * ║ - result: char* with lenth of MAX_STRING_LENGTH
 * ║           to store the output at. Set to NULL
 * ║           when a new one should be allocated.
 * ║
 * ║
 * ║ returns:
 * ║ - renderable char* string of VM memory
 * ║
 * ╚════════════════════════════════════════════════════╝
 */
extern char* EMSCRIPTEN_KEEPALIVE web_printCore( battle_t *battle, char* result );

#endif
