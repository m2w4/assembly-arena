#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "config.h"
#include "vm.h"

parameter_t process_mrm( uint8_t modrm, int16_t offset, vm_t* vm )
{
    parameter_t params;
    uint8_t mrm = modrm >> 4;
    uint8_t reg = modrm%0x10; //process higher 4-bit
    DEBUG_PRINT("(VM %d) OFFSET=%02x: ", vm->id, offset);
    switch (mrm) //process lower 4-bit
    {
        case REG_REG: //0R
            DEBUG_PRINT("0R: %d <- #%d\n", CHOOSE_DEST_REG( reg ), CHOOSE_SRC_REG( reg ));
            params.dptr = &(vm->reg[CHOOSE_DEST_REG( reg )]);
            params.sptr = &(vm->reg[CHOOSE_SRC_REG ( reg )]);
            break;
        case REG_PREG:  //1R
            DEBUG_PRINT("1R: %d <- #%d\n", vm->reg[CHOOSE_DEST_REG( reg )],RING_PTR_S_REG(vm,reg));
            params.dptr = &(vm->reg[CHOOSE_DEST_REG( reg )]);
            params.sptr = RING_PTR_S_REG(vm,reg);
            break;
        case REG_I16: //2R
            DEBUG_PRINT("2R: %d <- 0x%04x (%d)\n", CHOOSE_DEST_REG( reg ), offset, offset);
            params.dptr = &(vm->reg[CHOOSE_DEST_REG( reg )]);
            vm->imm = offset;
            params.sptr = &( vm->imm );
            //printf("%d\n", *(params.dptr));
            break;
        case PREG_REG:  //3R | WRITE
            DEBUG_PRINT("3R:\n");
            params.dptr = RING_PTR_D_REG(vm,reg);
            params.sptr = &(vm->reg[CHOOSE_SRC_REG ( reg )]);

            vm->ring->ownership[ (vm->reg[REG_I]+vm->reg[CHOOSE_DEST_REG( reg )])%CORE_N] = vm->id;
            vm->ring->ownership[ (vm->reg[REG_I]+vm->reg[CHOOSE_DEST_REG( reg )]+1)%CORE_N] = vm->id;
            break;
        case PREG_PREG:  //4R | WRITE
            DEBUG_PRINT("4R:\n");
            params.dptr = RING_PTR_D_REG(vm,reg);
            params.sptr = RING_PTR_S_REG(vm,reg);

            vm->ring->ownership[ (vm->reg[REG_I]+vm->reg[CHOOSE_DEST_REG( reg )])%CORE_N  ] = vm->id;
            vm->ring->ownership[ (vm->reg[REG_I]+vm->reg[CHOOSE_DEST_REG( reg )]+1)%CORE_N] = vm->id;
            break;
        case PREG_I16:  //5R | WRITE
            DEBUG_PRINT("5R: [0x%04x] <- 0x%04x\n", CHOOSE_DEST_REG( reg ), offset);
            vm->imm = offset;
            params.dptr = RING_PTR_D_REG(vm,reg);
            params.sptr = &( vm->imm );
            vm->ring->ownership[ (vm->reg[REG_I]+vm->reg[CHOOSE_DEST_REG( reg )])%CORE_N] = vm->id;
            vm->ring->ownership[ (vm->reg[REG_I]+vm->reg[CHOOSE_DEST_REG( reg )]+1)%CORE_N] = vm->id;
            break;
        case P16_REG:  //6R
            DEBUG_PRINT("6R: [0x%04x] <- 0x%04x\n",  RING_PTR_IMM(vm), vm->reg[CHOOSE_SRC_REG ( reg )]);
            vm->imm = offset;
            params.dptr = RING_PTR_IMM(vm);
            params.sptr = &(vm->reg[CHOOSE_SRC_REG ( reg )]);
            DEBUG_PRINT("[0x%04x] <- #%04x\n", *(params.dptr), reg);

            vm->ring->ownership[ (vm->reg[REG_I]+vm->reg[CHOOSE_SRC_REG ( reg )])%CORE_N] = vm->id;
            vm->ring->ownership[ (vm->reg[REG_I]+vm->reg[CHOOSE_SRC_REG ( reg )]+1)%CORE_N] = vm->id;
            break;
        default:
            DEBUG_PRINT("ERROR @ param\n");
            params.dptr = 0x00000000;
            params.sptr = 0x00000000;
            //params = NULL;
            break;
    }
    return params;
}

parameter_t process_m( uint8_t modrm, int16_t offset, vm_t* vm )
{
    parameter_t params;
    uint8_t mrm = modrm >> 4;
    uint8_t reg = modrm%0x10; //process higher 4-bit

    switch (mrm) //process lower 4-bit
    {
        case REG_REG: //0R
            //DEBUG_PRINT("[INFO] M:  %d, #%d\n", CHOOSE_DEST_REG( reg ), CHOOSE_SRC_REG( reg ));
            params.dptr = &(vm->reg[CHOOSE_DEST_REG( reg )]);
            break;
        case REG_I16: //1R
            //DEBUG_PRINT("[INFO] M: #%d, 0x%04x\n", CHOOSE_DEST_REG( reg ), offset);
            params.dptr = &(vm->reg[CHOOSE_DEST_REG( reg )]);
            break;
        case REG_PREG: //2R
            params.dptr = &(vm->reg[CHOOSE_DEST_REG( reg )]);
            break;
        case PREG_REG: //3R
            params.dptr = RING_PTR_D_REG(vm,reg);
            break;
        case PREG_PREG: //4R
            params.dptr = RING_PTR_D_REG(vm,reg);
            break;
        case PREG_I16: //5R
            params.dptr = RING_PTR_D_REG(vm,reg);
            break;
        case P16_REG: //6R
            vm->imm = offset;
            params.dptr = RING_PTR_IMM(vm);
            break;
        case I16:     //70
            vm->imm = offset;
            params.dptr = &( vm->imm );
            break;
        default:
            params.dptr = 0x00;
            #ifdef INTERACTIVE
            printf("ERROR @ param (%02x)\n", mrm);
            #endif
            break;
    }
    return params;
}


#define DO_JMP(mrm, offset, vm) do { if (do_jmp(mrm, offset, vm)) return 0; else return -1; } while (0)

static int do_jmp( uint8_t modrm, int16_t offset, vm_t *vm )
{
            parameter_t params;
            if(modrm == 0xff) { //TODO: Remove
                vm->reg[REG_I] += offset;
                vm->reg[REG_I] %= CORE_N;
                if( vm->reg[REG_I] < 0 ) vm->reg[REG_I] += CORE_N;
            } else
            {
                params = process_m(modrm, offset, vm);
                if(!IS_PARAM_VALID(params)) return 0;
                vm->reg[REG_I] += *(params.dptr);
                vm->reg[REG_I] %= CORE_N;
                if( vm->reg[REG_I] < 0 ) vm->reg[REG_I] += CORE_N;
            }
            return 1;
}

int process_instruction( uint8_t opcode, uint8_t modrm, int16_t offset, vm_t* vm )
{
    parameter_t params;
    switch (opcode) {
        case MOV:
            params = process_mrm(modrm, offset, vm);
            if(!IS_PARAMS_VALID(params)) return -1;
            *(params.dptr) = *(params.sptr);
            break;
        case ADD:
            params = process_mrm(modrm, offset, vm);
            if(!IS_PARAMS_VALID(params)) return -1;
            *(params.dptr) += *(params.sptr);
            break;
        case SUB:
            params = process_mrm(modrm, offset, vm);
            if(!IS_PARAMS_VALID(params)) return -1;
            *(params.dptr) -= *(params.sptr);
            break;
        case XOR:
            params = process_mrm(modrm, offset, vm);
            if(!IS_PARAMS_VALID(params)) return -1;
            *(params.dptr) ^= *(params.sptr);
            break;
        case CMP:
            params = process_mrm(modrm, offset, vm);
            if(!IS_PARAMS_VALID(params)) return -1;
            if( *(params.dptr) == *(params.sptr) )
            {
                vm->flg[FLAG_EQ] = 1;
                vm->flg[FLAG_GE] = 1;
                vm->flg[FLAG_LO] = 0;
            } else if( *(params.dptr) > *(params.sptr) ) {
                vm->flg[FLAG_EQ] = 0;
                vm->flg[FLAG_GE] = 1;
                vm->flg[FLAG_LO] = 0;
            } else {
                vm->flg[FLAG_EQ] = 0;
                vm->flg[FLAG_GE] = 0;
                vm->flg[FLAG_LO] = 1;
            }
            break;
        case JMP:
            DO_JMP(modrm, offset, vm );
            break;
        case JEQ:
            if( vm->flg[FLAG_EQ] == 1) DO_JMP(modrm, offset, vm );
            break;
        case JGE:
            if( vm->flg[FLAG_GE] == 1) DO_JMP(modrm, offset, vm );
            break;
        case JLO:
            if( vm->flg[FLAG_LO] == 1) DO_JMP(modrm, offset, vm );
            break;
        case JNEQ:
            if( vm->flg[FLAG_EQ] != 1) DO_JMP(modrm, offset, vm );
            break;
        case JNGE:
            if( vm->flg[FLAG_EQ] != 1) DO_JMP(modrm, offset, vm );
            break;
        case JNLO:
            if( vm->flg[FLAG_EQ] != 1) DO_JMP(modrm, offset, vm );
            break;
        default:
            #ifdef INTERACTIVE
            printf("Unknown OPCode %02x @ %04x! Player %d died\n", opcode, vm->reg[REG_I], vm->id);
            #endif
            #ifdef DEBUG
            printf("Unknown OPCode %02x @ %04x! Player %d died\n", opcode, vm->reg[REG_I], vm->id);
            #endif
            return -1;
    }
    return 0;

}

vm_t* new_vm( ring_t* ring, int id )
{
    vm_t* vm = (vm_t*)calloc(1, sizeof(vm_t));
    vm->id = id;
    vm->circle = 0;
    vm->ring = ring;
    return vm;
}

vm_t* add_warrior( vm_t* vm, uint8_t *buffer, uint32_t buffer_n, int chunk_id )
{
    int i;

    vm->reg[REG_I] = chunk_id;
    for(i=0;i<buffer_n;++i)
    {
        vm->ring->buffer[i + chunk_id] = buffer[i];
        vm->ring->ownership[i + chunk_id] = vm->id;
    }
    return vm;
}


void debug_print_core( vm_t *vm, vm_t *vm2 )
{
    int i;
    for(i=0;i<CORE_N;++i)
    {
        if(i%64==0) printf("\n%04x: ", i);
        if(vm->ring->ownership[i] != 0)
        {
            //TODO: Add support for multiple warriors
            if(vm->ring->ownership[i] == 1) {
                if(vm->reg[REG_I] == i)
                    printf("<\033[30;42m>%02x\033[0m ", vm->ring->buffer[i]); //green-back
                else
                    printf("\033[32m%02x\033[0m ", vm->ring->buffer[i]); //green-fore
            } else if(vm->ring->ownership[i] == 2) {
                if(vm2->reg[REG_I] == i)
                    printf("\033[30;41m%02x\033[0m ", vm->ring->buffer[i]); //red-back
                else if (vm->reg[REG_I] == i)
                    printf("\033[30;43m%02x\033[0m ", vm->ring->buffer[i]); //yellow-back
                else
                    printf("\033[31m%02x\033[0m ", vm->ring->buffer[i]); //red-fore
            }
        } else {
            if(vm->reg[REG_I] == i)
                printf("\033[33;42m%02x\033[0m ", vm->ring->buffer[i]);
            else if(vm2->reg[REG_I] == i)
                printf("\033[31;42m%02x\033[0m ", vm->ring->buffer[i]);
            else if (vm->reg[REG_I] == i)
                printf("\033[30;43m%d\033[0m ", vm->ring->buffer[i]); //yellow-back
            else
                printf("%02x ", vm->ring->buffer[i]);
        }
    }
    printf("\n");
}
