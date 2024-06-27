#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "vm.h"
#include "util.h"

#define CRC32_POLYNOMIAL 0xEDB88320


uint32_t rng(uint32_t seed) {
    seed = (seed ^ 21) * 2654435769u;
    seed = (seed ^ (seed >> 16)) * 2654435769u;
    seed = (seed ^ (seed >> 16)) * 2654435769u;
    return seed;
}

int sheduler_process( vm_t *vm )
{
    uint16_t ip =  vm->reg[REG_I];
    uint8_t  opcode  = vm->ring->buffer[(ip+0)%CORE_N];
    uint8_t  modrm   = vm->ring->buffer[(ip+1)%CORE_N];
    int16_t offset = *(uint16_t*)(vm->ring->buffer + ip + 2);
    int ret_code = process_instruction( opcode, modrm, offset, vm );

    vm->reg[REG_I] += 4;
    return ret_code;
}
