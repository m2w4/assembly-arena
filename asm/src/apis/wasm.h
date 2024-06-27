/*
  ╔══════════════════════════════════════════════════════════╗
  ║                         wasm.h
  ║ defines the interface for webasm applications.
  ║
  ╚══════════════════════════════════════════════════════════╝
*/
#ifndef API_WASM_H
#define API_WASM_H

#include "../asm.h"

#include <stdint.h>
#include <iostream>
#include <map>
#include <string>
#include <memory>
#include <sstream>
#include <vector>
#include <cstring> // for memcpy

#ifndef __EMSCRIPTEN__
#define EMSCRIPTEN_KEEPALIVE
#else
#include <emscripten.h>
#endif

/* ╔═ web_debugPrint (dev) ══
 * ║
 * ║ Similar to web_getReadableData, but used for
 * ║ debugging purposes.
 * ║
 * ║ parameters:
 * ║ - output_t*: pointer to assembled object.
 * ║ returns:
 * ║ - char*: bytecode as String
 * ║
 * ╚════════════════════════════════════════════╝
 */
int web_debugPrint( std::string unprocessed_code );

extern "C" {
EMSCRIPTEN_KEEPALIVE
/* ╔═ web_getReadableData  ══
 * ║
 * ║ Returns the bytecode as formatted
 * ║ printable string.
 * ║
 * ║ parameters:
 * ║ - output_t*: pointer to assembled object.
 * ║ returns:
 * ║ - char*: bytecode as String
 * ║
 * ╚════════════════════════════════════════════╝
 */
char* web_getReadableData( output_t* output );


/* ╔═ web_getBinaryData  ══
 * ║
 * ║ Returns the bytecode as char array
 * ║ bytecode. Use web_getBinaryData_n to
 * ║ determine the length.
 * ║
 * ║ parameters:
 * ║ - output_t*: pointer to assembled object.
 * ║ returns:
 * ║ - char*: raw assembled bytecode
 * ║
 * ╚════════════════════════════════════════════╝
 */
EMSCRIPTEN_KEEPALIVE
unsigned char* web_getBinaryData( output_t* output );

/* ╔═ web_getBinaryData_n  ══
 * ║
 * ║ Returns the length of the assembled
 * ║ bytecode.
 * ║
 * ║ parameters:
 * ║ - unprocessed_code: raw assembly code
 * ║ returns:
 * ║ - uint: length of assembled code
 * ║
 * ╚════════════════════════════════════════════╝
 */
EMSCRIPTEN_KEEPALIVE
unsigned int web_getBinaryData_n( output_t* output );

/* ╔═ web_assembleBuffer  ══
 * ║
 * ║ Assembles raw asm code to output_t object.
 * ║
 * ║ parameters:
 * ║ - unprocessed_code: raw assembly code
 * ║ returns:
 * ║ - output_t*: pointer to assembled object.
 * ║
 * ╚════════════════════════════════════════════╝
 */
EMSCRIPTEN_KEEPALIVE
output_t* web_assembleBuffer(const char* unprocessed_code);
}
#endif
