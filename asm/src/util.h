/*
  ╔══════════════════════════════════════════════════════════╗
  ║                         util.h
  ║ defines misc. utility functions used all across the
  ║ assembler.
  ╚══════════════════════════════════════════════════════════╝
*/

#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <vector>
#include <string>
#include <ostream>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace Util {

/* ╔═ swapEndianness ══
 * ║
 * ║ swaps int between big and little-endian
 * ║
 * ║ parameters:
 * ║ - value: integer to convert
 * ║
 * ║ returns:
 * ║ - converted integer
 * ║
 * ╚═══════════════════════════════════╝
 */
unsigned int swapEndianness(unsigned int value);

/* ╔═ formatInstruction ══
 * ║
 * ║ std::couts an uint32 as readable hex string.
 * ║
 * ║ parameters:
 * ║ - value: integer to convert
 * ║   example: 0x11223344
 * ║
 * ║ returns:
 * ║ - converted integer
 * ║   example: "11 22 33 44"
 * ║
 * ╚═══════════════════════════════════╝
 */
void formatInstruction(uint32_t i32);

/* ╔═ formatInstruction ══
 * ║
 * ║ returns an uint32 as readable hex string as std::string.
 * ║
 * ║ parameters:
 * ║ - value: integer to convert
 * ║   example: 0x11223344
 * ║
 * ║ returns:
 * ║ - converted integer
 * ║   example: "11 22 33 44"
 * ║
 * ╚═══════════════════════════════════╝
 */
std::string sformatInstruction(uint32_t i32);
};
#endif
