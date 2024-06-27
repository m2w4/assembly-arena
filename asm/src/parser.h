/*
  ╔══════════════════════════════════════════════════════════╗
  ║                       parser.h
  ║ defines the interface for the parser-class
  ║ that handles tokized instructions as input and
  ║ returns a parsed instruction.
  ║ Also handles .labels and relative Jumps
  ║
  ║ Code-Generation Steps:
  ║ preprocessor ==> analyze   ==> [ parser ] ==> assembler
  ║ preproc.h    ==> analyze.h ==> [parser.h] ==> asm.h
  ╚══════════════════════════════════════════════════════════╝
*/

#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <map>
#include <string>
#include <stdint.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <memory>
#include <iomanip>
#include <unordered_map>

#include "asm.h"
#include "parameterParser.h"


//instructions in the following format: Mnemoric;OPCode;Parameter_n
static std::map<std::string, instruction_t> isa = {
    {"MOV" ,{0x00,2}},
    {"ADD" ,{0x01,2}},
    {"SUB" ,{0x02,2}},
    {"XOR" ,{0x03,2}},
    {"CMP" ,{0x04,2}},
    {"JMP" ,{0x75,1}},
    {"JEQ" ,{0x76,1}},
    {"JGE" ,{0x77,1}},
    {"JLO" ,{0x78,1}},
    {"JNE" ,{0x79,1}},
    {"JNGE",{0x7a,1}},
    {"JNLO",{0x7b,1}},
    {"NOP" ,{0xf0,0}},
};

/* ╔═ Parser ══
 * ║ Parses tokenizes data to ready-to-execute
 * ║ integers of part of integers.
 * ╚════════════════════════════════════════════════╝
 */
class Parser {
public:
/* ╔═ parseInstruction ══
 * ║
 * ║ parses an tokenized instruction to a
 * ║ 32-bit instruction as a 32-bit integer.
 * ║ Check Analyzer::lexicalAnalyize to tokenize
 * ║ a string.
 * ║
 * ║ parameters:
 * ║ - input: tokenized instruction
 * ║ returns <pair>:
 * ║ - bool:     true, if decode was successful
 * ║ - uint32_t: instruction as 32-bit integer
 * ║
 * ╚════════════════════════════════════════════╝
 */
std::pair<bool, uint32_t> parseInstruction( std::vector<std::pair<int, std::string>> tokens, unsigned int location );


/* ╔═ fetchLabels ══
 * ║
 * ║ adds a tokenized label to the jump-label Table.
 * ║ does nothing, if the tokenized like is not a
 * ║ label.
 * ║
 * ║ parameters:
 * ║ - tokens:   tokenized line, in best case a label
 * ║ - location: offset of current line/label
 * ║ returns:
 * ║ - int: returns the length of the line in bytes.
 * ║        Tip: If this line is an instruction, it
 * ║        will return 4, otherwise 0, because
 * ║        labels and comments won't be compiled
 * ║
 * ╚═══════════════════════════════════════════════════╝
 */
int fetchLabels( std::vector<std::pair<int, std::string>> tokens, unsigned int location );


private:
/* ╔═ getSingleParameterByte ══
 * ║
 * ║ assembles the parameter-byte, which is similar to
 * ║ the ModR/M-byte from x86 architecture.
 * ║
 * ╠═══════════════════════════╗
 * ║              byte
 * ║               vv
 * ║ JMP [#y]: XX [YY] 0100
 * ╠═══════════════════════════╝
 * ║
 * ║ parameters:
 * ║ - tk1: tokenized first parameter
 * ║ returns:
 * ║ - i8: storable parameter-byte
 * ║
 * ╚═══════════════════════════════════════════════════╝
 */
uint8_t getSingleParameterByte( std::pair<int, std::string> tk1 );

/* ╔═ getDualParameterByte ══
 * ║
 * ║ assembles the parameter-byte, which is similar to
 * ║ the ModR/M-byte from x86 architecture.
 * ║
 * ╠═══════════════════════════╗
 * ║                  byte
 * ║                   vv
 * ║ MOV [#y, #y]: XX [YY] ZZZZ
 * ╠═══════════════════════════╝
 * ║
 * ║ parameters:
 * ║ - tk1: tokenized first  parameter
 * ║ - tk1: tokenized second parameter
 * ║ returns:
 * ║ - i8: storable parameter-byte
 * ║
 * ╚═══════════════════════════════════════════════════╝
 */
uint8_t getDualParameterByte( std::pair<int, std::string> tk1, std::pair<int, std::string> tk2 );



/* ╔═ getImmediate (single parameter) ══
 * ║
 * ║ represents immediate used in parameter as a
 * ║ 16-bit integer to be stored in the instruction
 * ║
 * ╠═══════════════════════════╗
 * ║                 imm
 * ║                  vv
 * ║ JMP <1>: XX YY [0100]
 * ╠═══════════════════════════╝
 * ║
 * ║ parameters:
 * ║ - tk1:   tokenized first parameter
 * ║ - location: offset of current line/label
 * ║ returns:
 * ║ - i16: storable i16
 * ║
 * ╚═══════════════════════════════════════════════════╝
 */
int16_t getImmediate( std::pair<int, std::string> tk1, std::pair<int, std::string> tk2, unsigned int location );


/* ╔═ getImmediate (dual parameter) ══
 * ║
 * ║ represents immediates used in parameters as a
 * ║ 16-bit integer to be stored in the instruction
 * ╠═══════════════════════════╗
 * ║                     imm
 * ║                      vv
 * ║ MOV #a, [2]: XX YY [0200]
 * ╠═══════════════════════════╝
 * ║
 * ║ parameters:
 * ║ - tk1:   tokenized first parameter
 * ║ - tk2:   tokenized second parameter
 * ║ - location: offset of current line/label
 * ║ returns:
 * ║ - i16: storable i16
 * ║
 * ╚═══════════════════════════════════════════════════╝
 */
int16_t getImmediate( std::pair<int, std::string> tk1, unsigned int location );

/* ╔═ doesInstructionExist ══
 * ║
 * ║ helper function, which checks if a key
 * ║ exists in a map. As the name suggests
 * ║ it's used to check if an instruction
 * ║ mnemoric exist in the "isa" structure.
 * ║ specified above.
 * ║
 * ║ parameters:
 * ║ - tk1: tokenized parameter
 * ║ returns <pair>:
 * ║ - bool:     true, if decode was successful
 * ║ - uint32_t: instruction as 32-bit integer
 * ║
 * ╚════════════════════════════════════════════╝
 */
static bool doesInstructionExist(const std::map<std::string, instruction_t>& isa, const std::string& keyName);

// Symbol-Table of .labels and their offset in code.
std::shared_ptr<JumpTable> jumpTable = std::make_shared<JumpTable>();
};
#endif
