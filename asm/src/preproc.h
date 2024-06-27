/*
  ╔══════════════════════════════════════════════════════════╗
  ║                       preproc.h
  ║ defines the interface for the preprocessor.
  ║
  ║ Code-Generation Steps:
  ║ [preprocessor] ==> analyzer   ==> parser   ==> assembler
  ║ [  preproc.h ] ==> analyzer.h ==> parser.h ==> asm.h
  ╚══════════════════════════════════════════════════════════╝
*/
#ifndef PREPROC_H
#define PREPROC_H

#include <regex>
#include <memory>
#include <iomanip>
#include <unordered_map>
#include <string>
#include <sstream>

/* ╔═ preprocess ══
 * ║
 * ║ preprocesses a given raw asm code.
 * ║ Only supports C-like "#define"s for variables.
 * ║ Explained in the given example:
 * ║
 * ║ parameters:
 * ║ - input: raw assembly language code
 * ║   example: "#define ONE 1;\nMOV #a, ONE"
 * ║ returns:
 * ║ - preprocessed assembly code
 * ║   example: "\nMOV #a,1"
 * ║
 * ╚═══════════════════════════════════╝
 */
std::string preprocess(const std::string& input);

#endif
