/*
  ╔══════════════════════════════════════════════════════════╗
  ║                       native.h
  ║ defines the interface for native C++ applications.
  ║
  ╚══════════════════════════════════════════════════════════╝
*/
#ifndef API_NATIVE_H
#define API_NATIVE_H

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

#include "../asm.h"
#include "../util.h"
#include "../preproc.h"
#include "../parser.h"

namespace ArenaASM {
/* ╔═ printFormatted  ══
 * ║
 * ║ shows printable output of the assembled
 * ║ code in "unprocessed_code".
 * ║ This function also  preprocesses input.
 * ║
 * ║ parameters:
 * ║ - unprocessed_code: raw asm code
 * ║   example: "MOV #a, 0\nJMP +3"
 * ║
 * ╚════════════════════════════════════════════╝
 */
void printFormatted(std::string unprocessed_code);

/* ╔═ printFormatted  ══
 * ║
 * ║ writes assembled bytecode to file.
 * ║ code in "unprocessed_code".
 * ║
 * ║ parameters:
 * ║ - output: output struct (from asmBuffer)
 * ║
 * ╚════════════════════════════════════════════╝
 */
void      assembleToFile(output_t* output, std::string fname);

/* ╔═ printFormatted  ══
 * ║
 * ║ writes assembled bytecode to file.
 * ║ code in "unprocessed_code".
 * ║
 * ║ parameters:
 * ║ - unprocessed_code: asm-code to assemble
 * ║ - output: malloc'd output_t*
 * ║   example: output_t *o = malloc(sizeof(output_t));
 * ║                      o = asmBuffer(asmCode, output);
 * ║
 * ╚════════════════════════════════════════════╝
 */
output_t* asmBuffer     (std::string unprocessed_code, output_t* output);
}
#endif
