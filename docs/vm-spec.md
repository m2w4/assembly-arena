# ArenaASM Instruction Set Architecture (ISA) Documentation

## Overview

ArenaASM uses a 16-bit Instruction Set Architecture.

## Registers

### General-Purpose Registers
| ID      | Register    | Notes       |
| --------| ----------- | ----------- |
| 0       | #a          |             |
| 1       | #b          |             |
| 2       | #c          |             |
| 3       | #d          |             |

### Special Registers
### General-Purpose Registers
| ID      | Register    | Notes       |
| --------| ----------- | ----------- |
| 4       | #i          | Instruction Pointer |

The instruction pointer can't be manipulated directly, but is editable with Jump instructions.

## Flags

| ID | Flag | Notes                     |
| -- | ---- | ------------------------- |
| 00 | EQ   | Equals (eq)               |
| 01 | GE   | Greater than or equal (ge)|
| 02 | LO   | Lower than (lo)           |

## Instruction Encoding

Every instruction has a fixed-length of 32-bit.


<div style="text-align: center">
<b>OPCode | Parameter Byte | Offset</b><br />
<span style="color:#f00">00</span> <span style="color:#0f0">00</span> <span style="color:#0ff">0000</span>
<br />
<span style="color:#f00">MOV</span> <span style="color:#0f0">#a, </span> <span style="color:#0ff">0000</span>
</div>

#### <span style="color:#f00">O</span>PCode
Defines the current instruction.

##### Opcode Table
| Mnemonic | Opcode | Params | Operator                | Example              |
| ---------| -------| -------| ------------------------| -------------------- |
| MOV      | 0x00   | 2      | P1 = P2                  | MOV #a, #b          |
| ADD      | 0x01   | 2      | P1 = P1 + P2             | ADD #a, #b          |
| SUB      | 0x02   | 2      | P1 = P1 - P2             | SUB #a, #b          |
| XOR      | 0x03   | 2      | P1 = P1 XOR P2           | XOR #a, #b          |
| CMP      | 0x04   | 2      | Compare P1 and P2        | CMP #a, #b          |
| JMP      | 0x76   | 1      | Unconditional Jump       | JMP 0x1234          |
| JEQ      | 0x77   | 1      | Jump if Equal            | JEQ 0x5678          |
| JGE      | 0x78   | 1      | Jump if Greater/Equal    | JGE 0x9abc          |
| JLO      | 0x79   | 1      | Jump if Lower            | JLO 0xdef0          |
| JNEQ     | 0x7a   | 1      | Jump if Not Equal        | JNEQ 0x2345         |
| JNGE     | 0x7b   | 1      | Jump if Not Greater/Equal| JNGE 0x6789         |
| JNLO     | 0x7c   | 1      | Jump if Not Lower        | JNLO 0xabcd         |

If the first 4-bits are lower than 0x7 the instruction requires two parameters.
Otherwise the parameter number if one.

Jump instructions always contain the relative offset from the Jump-instruction and the target address.

#### <span style="color:#0f0">P</span>arameter Byte
Defines the current paramters.
Split in two 4-bit:

<span style="color:#ff0">0</span><span style="color:#f0f">0</span>

<span style="color:#ff0">4-bit High-order syntax</span>

This bit defines the structure of the parameters

|         | Syntax       |
| --------| ------------ |
| 0       | REG, REG     |
| 1       | REG, [REG]   |
| 2       | REG, I16     |
| 3       | [REG], REG   |
| 4       | [REG], [REG] |
| 5       | [REG], I16   |
| 6       | [I16], REG   |
| 7       | I16          |

//Maybe added later
//[REG], [I16]

<span style="color:#f0f">4-bit Low-order register bit</span>

|         | Register     |
| --------| ------------ |
| 0       | #a, #a       |
| 1       | #a, #b       |
| 2       | #a, #c       |
| 3       | #a, #d       |
| 4       | #b, #a       |
| 5       | #b, #b       |
| 6       | #b, #c       |
| 7       | #b, #c       |
| 8       | #c, #a       |
| 9       | #c, #b       |
| a       | #c, #c       |
| b       | #c, #d       |
| c       | #d, #a       |
| d       | #d, #b       |
| e       | #d, #c       |
| f       | #d, #d       |

#### <span style="color:#0ff">O</span>ffset
Offset for 16-bit immediates.
If not needed, it can be freely used.

## Example Instructions:
TODO
