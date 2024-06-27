/*
  ╔══════════════════════════════════════════════════════════╗
  ║                       asm.h
  ║ defines easy-to-use functions to assemble code given
  ║ as a raw string. If you want to work with the assembled
  ║ data in output_t, look into the /apis/ folder or at the
  ║ definition of "output_t".
  ║
  ║ Code-Generation Steps:
  ║ preprocessor ==> analyze   ==> parser   ==> [assembler]
  ║ preproc.h    ==> analyze.h ==> parser.h ==> [  asm.h  ]
  ╚══════════════════════════════════════════════════════════╝
*/

#ifndef ASM_H
#define ASM_H

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

#include "analyze.h"

/* ┌─ SyntaxBits             ───┐
 * │  value of the 4 syntax-bits
 * │  in the parameter byte.
 * ├─
 * │    Syntax
 * │      v
 * │  XX [Y]Y ZZZZ
 * └───                      ───┘
 */
enum MRM {
  REG_REG,
  REG_PREG,
  REG_I16,
  PREG_REG,
  PREG_PREG,
  PREG_I16,
  P16_REG,
  I16
};


/* ┌─ output_t                           ───┐
 * │  contains assembled data in raw binary
 * │  and printable format.
 * │
 * ├─ Members
 * │ ok: true, if output is valid
 * │ binaryData: assembled raw binary
 * │ readableData: printable formatted binary
 * │ error: printable error
 * └───                                  ───┘
 */
typedef struct {
    bool ok;
    std::vector<uint32_t> binaryData;
    std::stringstream readableData;
    std::stringstream error;
} output_t;


/* ╔═ JumpTable ══
 * ║ Helper class to store label and location pairs.
 * ╚════════════════════════════════════════════════╝
 */
class JumpTable {
public:
    std::map<std::string, int> labels;
};

/* ┌─ instruction_t             ─┐
 * │ specified instruction
 * │ used for instruction
 * │ database.
 * │
 * ├─ Members
 * ├ opcode  : byte to represent
 * ├ params_n: nr of parameters
 * └───                      ───┘
 */
typedef struct {
    uint8_t  opcode;
    uint32_t params_n;
} instruction_t;

/* ╔═ assembleBuffer ══════════════════════════════════════════╗
 * ║
 * ║ assembles preprocesses code and returns assembled output_t
 * ║
 * ║ parameters:
 * ║ - unprocessed_code: asm code to assemble
 * ║ - output: output assembled code
 * ║
 * ║ returns:
 * ║ - output assembled code or nullptr, if invalid
 * ║
 * ╚═══════════════════════════════════════════════════════════╝
 */
output_t* assembleBuffer( std::string unprocessed_code, output_t* output);

#endif
