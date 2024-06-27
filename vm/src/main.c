#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include "config.h"
#include "sheduler.h"
#include "util.h"
#include "vm.h"
#include "api.h"


    //Old initial battle. Keeping it for reference:
    //#DEF WARRI_N ;size of warrior
    // - - - - - -
    /*
    uint8_t warrior_one[] = {
                             //#DEF OFFSET 12
                             //.start
                             MOV , 0x20, 0x00, 0x01,  //MOV #a, 0x0100 ; WARRI_N + OFFSET
                             MOV , 0x24, 0xf4, 0xff,  //MOV #b, -12      ; .start
                             MOV , 0x28, 0x14, 0x00,  //MOV #c, 0x14   ; WARRI_N/4
                             //.loop
                             MOV , 0x41, 0x00, 0x00,  //MOV [#a], [#b]
                             ADD , 0x20, 0x02, 0x00,  //ADD #a, 2
                             ADD , 0x24, 0x02, 0x00,  //ADD #b, 2
                             SUB , 0x28, 0x01, 0x00,  //SUB #c, 1
                             CMP , 0x28, 0x00, 0x00,  //CMP #c, 0x0000
                             JNEQ, 0x70, 0xe8, 0xff,  //JNE .loop
                             JMP , 0x70, 0xe4, 0x00   //JMP +0x1a04    ; OFFSET
    };                       //.end
    uint32_t warrior_one_n = 40;

    uint8_t warrior_two[] = {
                             MOV, 0x20, 0x14, 0x00,  //MOV #a, 0x14
                             //.loop
                             MOV, 0x50, 0xBA, 0xBE, //MOV [#a], 0xBABE
                             ADD, 0x20, 0x02, 0x00, //ADD #a, 0x4
                             JMP, 0x70, 0xf4, 0xff //JMP .loop
    };
    uint32_t warrior_two_n = 16;
    */


int main(int argc, char* argv[] ) { //TODO
    #ifdef __EMSCRIPTEN__
        fprintf(stderr, "[INFO] Initialized wasm VM module");
        return 0;
    #else
    battle_t* battle;
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <base64_string1> <base64_string2>\n", argv[0]);
        return EXIT_FAILURE;
    }
    battle = initBattle ( argv[1], argv[2] );
    return playBattleCLI( battle );
    #endif
}
