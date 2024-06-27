#include "api.h"
#include "config.h"

battle_t* initBattle( char* warrior1_b64, char* warrior2_b64 )
{
    size_t decoded_len;
    unsigned char *warrior_one = base64_decode(warrior1_b64, &decoded_len);
    uint32_t warrior_one_n =decoded_len;

    unsigned char *warrior_two = base64_decode(warrior2_b64, &decoded_len);
    uint32_t warrior_two_n =decoded_len;

    battle_t *battle;

    ring_t *ring = (ring_t*)calloc(1,sizeof(ring_t));
    vm_t *vm1 = new_vm( ring, 1 );
    vm_t *vm2 = new_vm( ring, 2 );


    uint32_t hash1 = hash(warrior_one, warrior_one_n);
    uint32_t hash2 = hash(warrior_one, warrior_two_n);
    uint32_t hash  = hash1^hash2;
    srand(hash);
    int addr1 = rand()%CORE_N;
    int addr2 = rand()%CORE_N;
    while( check_collision(addr1, addr2, warrior_one_n, warrior_two_n) == 1)
    {
        addr1 = rand()%CORE_N;
        addr2 = rand()%CORE_N;
    }
    add_warrior(vm1, warrior_one, warrior_one_n, addr1);
    add_warrior(vm2, warrior_two, warrior_two_n, addr2);

    battle = (battle_t*)malloc(sizeof(battle_t));
    battle->warrior1 = vm1;
    battle->warrior2 = vm2;
    battle->ring     = ring;
    battle->has_ended= IS_RUNNING;
    battle->curr_iteration = 0;

    //fprintf(stderr, "Battle initialized\n");
    return battle;
}


int playBattleCLI( battle_t *battle )
{

    vm_t *vm1 = battle->warrior1;
    vm_t *vm2 = battle->warrior2;

    int ret;
    for(int i=0;i<MAX_ITERATIONS;++i) {
        ret = sheduler_process( vm1 );

    #ifdef INTERACTIVE
        system("clear");
        debug_print_core(vm1, vm2);
        usleep(250000);
        if (ret < 0 )
        {
            debug_print_core(vm1, vm2);
            printf("\n\t\t\t\t\t\t\t\t\t>> \033[30;41mPlayer 2 won!\033[0m <<\n");
            return 2;
        }

        ret = sheduler_process( vm2 );
        if (ret < 0 )
        {
            debug_print_core(vm1, vm2);
            printf("\n\t\t\t\t\t\t\t\t\t>> \033[30;40mPlayer 1 won!\033[0m <<\n");
            return 2;
        }
    #else
        if (ret < 0 )
        {
            #ifdef DEBUG
            debug_print_core(vm1, vm2);
            #endif
            printf("2"); //player 2 (red) won
            return 2;
        }
        ret = sheduler_process( vm2 );
        if (ret < 0 )
        {
            #ifdef DEBUG
            debug_print_core(vm1, vm2);
            #endif
            printf("1"); //player 1 (green) won
            return 1;
        }

    #endif
    }
    #ifdef DEBUG
    debug_print_core(vm1, vm2);
    #endif
    printf("0");
    return 0;
}

