/*
  ╔══════════════════════════════════════════════════════════╗
  ║                       vm.h
  ║ defines the function for the execution of the
  ║ virtual machine.
  ╚══════════════════════════════════════════════════════════╝
*/

#ifndef VM_H
#define VM_H

#include "config.h"

#define CRC32_POLYNOMIAL 0xEDB88320

#define INS_N 4 //32-bit (4 byte) instrction length

//├ instructions
#define MOV  0x00
#define ADD  0x01
#define SUB  0x02
#define XOR  0x03
#define CMP  0x04
#define JMP  0x75
#define JEQ  0x76
#define JGE  0x77
#define JLO  0x78
#define JNEQ 0x79
#define JNGE 0x7a
#define JNLO 0x7b
#define NOP  0xf0

//├ Flags
#define FLAG_EQ 0
#define FLAG_GE 1
#define FLAG_LO 2
#define FLG_N   5 //placeholder for 2 additional flags
//├ Registers
#define REG_A 0 //#a
#define REG_B 1 //#b
#define REG_C 2 //#c
#define REG_D 3 //#d
#define REG_I 4 //Instruction Pointer (IP)

#define REG_N 4

/* ┌─ parameter check macros                   ───┐
 * │ checks if a parameter byte got processed
 * │ susccessfully.
 * └───                                        ───┘
 */
#define IS_PARAMS_VALID( p ) ( (p.dptr != 0 && p.sptr != 0) ? 1 : 0)
#define IS_PARAM_VALID( p )  ( (p.dptr != 0               ) ? 1 : 0)


/* ┌─ register pointer  macros                   ───┐
 * │ returns the pointer of the right register,
 * │ immediate or arena address when processing
 * │ parameters.
 * └───                                          ───┘
 */
#define CHOOSE_DEST_REG(reg) ( (reg-reg%REG_N)/REG_N )
#define CHOOSE_SRC_REG(reg) ( (reg%REG_N ) )


#define POSITIVE(i) do { while((i)<0) (i)+=CORE_N; } while(0) //TODO, never used

#define RING_PTR_D_REG(vm, reg) ( (int16_t*)(vm->ring->buffer + (unsigned)(vm->reg[CHOOSE_DEST_REG( reg%REG_N )%CORE_N] + vm->reg[REG_I] )%CORE_N ) )
#define RING_PTR_S_REG(vm, reg) ( (int16_t*)(vm->ring->buffer + (unsigned)(vm->reg[CHOOSE_SRC_REG ( reg%REG_N )%CORE_N] + vm->reg[REG_I] )%CORE_N ) )
#define RING_PTR_IMM(vm)        ( (int16_t*)(vm->ring->buffer + (unsigned)(vm->imm + vm->reg[REG_I] )%CORE_N ) )



/* ┌─ SyntaxBits             ───┐
 * │  value of the 4 syntax-bits
 * │  in the parameter byte.
 * ├─
 * │    Syntax
 * │      v
 * │  XX [Y]Y ZZZZ
 * └───                      ───┘
 */
enum SyntaxBits {
  REG_REG,       // #x , #x
  REG_PREG,      // #x , [#x]
  REG_I16,       // #x , I16
  PREG_REG,      //[#x], #x
  PREG_PREG,     //[#x], [#x]
  PREG_I16,      //[#x], I16
  P16_REG,       //[I16], #a
  I16            // I16
};

/* ┌─ ring_t                    ───┐
 * │  defines the ring/core/arena.
 * │  Also keeps track of the
 * │  warrior, which have written
 * │  the last bytes. Used for
 * │  coloring.
 * │
 * ├─ Members
 * ├ buffer: Arena ring buffer
 * ├ ownership: Ownership of each
 * │            byte.
 * │
 * └───                        ───┘
 */
typedef struct {
    uint8_t buffer[MAX_CORE_N];
    uint8_t ownership[MAX_CORE_N];
} ring_t;


/* ┌─ instruction_t                 ───┐
 * │  stores an assembled instruction.
 * │
 * ├─ Members
 * ├ op: Opcode
 * ├ morm: Parameter-Byte
 * ├ offset: immediate
 * │
 * └───                            ───┘
 */
typedef struct {
    uint8_t op;
    uint8_t modrm;
    int16_t offset;
} instruction_t;


/* ┌─ vm_t                               ───┐
 * │ defines a warrior/applications
 * │ with registers, flags and a ptr
 * │ to the ring.
 * ├─ Members
 * ├ reg: #a, #b, #c, #d, #ip
 * │     hint: ip = instruction pointer
 * ├ flg: EQ, GE, LO
 * ├ imm: temporary storage for handling
 * │      immediates.
 * ├ ring: pointer to current ring.
 * │
 * └───                                 ───┘
 */
typedef struct {
    int       id;
    int   circle;
    int16_t reg[5];
    int16_t flg[5];
    int16_t imm; //temporary storage for imm processing
    ring_t *ring;
} vm_t;

/* ┌─ parameter_t                               ───┐
 * │ Stores pointer of the parameters of the
 * │ current instruction. Members self-explained.
 * └───                                         ───┘
 */
typedef struct {
    int16_t *dptr; //destination pointer
    int16_t *sptr; //source pointer
} parameter_t;

/* ╔═ process_mrm ══════════════════════════════════════════╗
 * ║
 * ║ processes the parameter-byte and returns pointer
 * ║ to them.
 * ║
 * ║ parameters:
 * ║ - modrm:  parameter byte
 * ║ - offset: immediate
 * ║ - vm: ptr to vm for arena memory-access
 * ║
 * ║ returns:
 * ║ - pointer_t to parameter arguments
 * ║
 * ╚═════════════════════════════════════════════════════════╝
 */
parameter_t process_mrm( uint8_t modrm, int16_t offset, vm_t* vm );

/* ╔═ process_instruction ══════════════════════════════════╗
 * ║
 * ║ Executes one instruction.
 * ║
 * ║ process_instruction:
 * ║ - opcode: instruction opcode
 * ║ - modrm:  parameter byte
 * ║ - offset: immediate
 * ║
 * ║ returns:
 * ║ - 0, when instruction worked. <0 when error.
 * ║
 * ╚═════════════════════════════════════════════════════════╝
 */
int process_instruction( uint8_t opcode, uint8_t modrm, int16_t offset, vm_t* vm );

/* ╔═ new_vm ═════════════════════════════════════════════════╗
 * ║
 * ║ Create a new VM/Warrior. Can be written to arena
 * ║ using add_warrior.
 * ║
 * ║ parameters:
 * ║ - ring_t: pointer to Arena to which VM should be added.
 * ║ - id: VM ID. SHOULD be incremented (0,1,2,...)
 * ║
 * ║ returns:
 * ║ - vm_t: newly allocated warrior object
 * ║
 * ╚═══════════════════════════════════════════════════════════╝
 */
vm_t* new_vm( ring_t* ring, int id );

/* ╔═ add_warrior ══════════════════════════════════════════╗
 * ║
 * ║ Adds a warrior to a ring in a given chunk.
 * ║
 * ║ parameters:
 * ║ - vm: warrior to add
 * ║ - buffer: raw warrior code
 * ║ - buffer_n: warrior length
 * ║ - chunk_id: address to write warrior  (chunk_n=1)
 * ║
 * ║ returns:
 * ║ - output assembled code or nullptr, if invalid
 * ║
 * ╚═══════════════════════════════════════════════════════════╝
 */
vm_t* add_warrior( vm_t* vm, uint8_t *buffer, uint32_t buffer_n, int chunk_id );

//╔═ debug functions ═╗

/* ╔═ debug_print_core ══════════════════════════════════════════╗
 * ║
 * ║ prints the Ring to CLI. Only needs two VMs, which NEED to
 * ║ be in the same ring.
 * ╚═════════════════════════════════════════════════════════════╝
 */
void debug_print_core( vm_t *vm, vm_t *vm2 );

#endif
