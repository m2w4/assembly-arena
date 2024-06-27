#include "api_wasm.h"

extern battle_t* EMSCRIPTEN_KEEPALIVE web_initBattle( char* warrior1_b64, char* warrior2_b64 )
{
    fprintf(stderr, "Started battle with %s and %s\n", warrior1_b64, warrior2_b64);
    return initBattle(warrior1_b64, warrior2_b64);
}

extern int EMSCRIPTEN_KEEPALIVE web_playSingleRound( battle_t *battle )
{
    vm_t *vm  = battle->warrior1;
    vm_t *vm2 = battle->warrior2;
    if      ( sheduler_process( vm ) < IS_RUNNING)
        battle->has_ended = WARRIOR_2_WON; //2
    else if ( sheduler_process( vm2 ) < IS_RUNNING)
        battle->has_ended = WARRIOR_1_WON; //1
    return battle->has_ended; //IS_RUNNING = 0
}

extern int EMSCRIPTEN_KEEPALIVE web_playAllRounds( battle_t *battle )
{
    int outcome;
    for(int i=0;i<MAX_ITERATIONS/2;++i) {
        outcome = web_playSingleRound( battle );
        if( outcome == WARRIOR_1_WON || outcome == WARRIOR_2_WON )
        {
            return outcome;
        }
    }
    return WARRIOR_DRAW;
}

extern char* EMSCRIPTEN_KEEPALIVE web_printCore( battle_t *battle, char* result )
{

    vm_t *vm  = battle->warrior1;
    vm_t *vm2 = battle->warrior2;

    if(result == NULL)
        result = (char*)malloc(MAX_STRING_LENGTH);

    if (result == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    int offset = 0;
    int i;
    for (i = 0; i < CORE_N; ++i) {
        if (i % 64 == 0) {
            offset += snprintf(result + offset, MAX_STRING_LENGTH - offset, "</br>%04x: ", i);
        }

        if (vm->ring->ownership[i] != 0) {
            // TODO: Add support for multiple warriors
            if (vm->ring->ownership[i] == 1) {
                if (vm->reg[REG_I]%CORE_N == i)
                    offset += snprintf(result + offset, MAX_STRING_LENGTH - offset, "<gb>%02x</gb> ", vm->ring->buffer[i]);
                else
                    offset += snprintf(result + offset, MAX_STRING_LENGTH - offset, "<gf>%02x</gf> ", vm->ring->buffer[i]);
            } else if (vm->ring->ownership[i] == 2) {
                if (vm2->reg[REG_I]%CORE_N == i)
                    offset += snprintf(result + offset, MAX_STRING_LENGTH - offset, "<rb>%02x</rb> ", vm->ring->buffer[i]);
                else if (vm->reg[REG_I]%CORE_N == i)
                    offset += snprintf(result + offset, MAX_STRING_LENGTH - offset, "<yb>%02x</yb> ", vm->ring->buffer[i]);
                else
                    offset += snprintf(result + offset, MAX_STRING_LENGTH - offset, "<rf>%02x</rf> ", vm->ring->buffer[i]);
            }
        } else {
            if (vm->reg[REG_I]%CORE_N == i)
                offset += snprintf(result + offset, MAX_STRING_LENGTH - offset, "<gb>%02x</gb> ", vm->ring->buffer[i]);
            else if (vm2->reg[REG_I]%CORE_N == i)
                offset += snprintf(result + offset, MAX_STRING_LENGTH - offset, "<rb>%02x</rb> ", vm->ring->buffer[i]);
            else if (vm->reg[REG_I]%CORE_N == i)
                offset += snprintf(result + offset, MAX_STRING_LENGTH - offset, "<yb>%d</yb> ", vm->ring->buffer[i]);
            else
                offset += snprintf(result + offset, MAX_STRING_LENGTH - offset, "%02x ", vm->ring->buffer[i]);
        }
    }

    return result;
}
