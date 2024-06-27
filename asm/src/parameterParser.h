/*
  ╔══════════════════════════════════════════════════════════╗
  ║                    parameterParser.h
  ║ defines a static class to parse parameters and reg2int
  ║ conversations. Mostly used my Parser class.
  ║
  ╚══════════════════════════════════════════════════════════╝
*/
#ifndef PARAMETERPARSER_H
#define PARAMETERPARSER_H

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

#include "parser.h"

class ParameterParser {
public:
/* ╔═ toInt ══
 * ║
 * ║ converts strings of registers and integers
 * ║ in binary, hex and binary representation
 * ║ to an integer.
 * ║
 * ║ parameters:
 * ║ - input: parameter as string
 * ║   example: "#b", "3", "0xff"
 * ║ returns:
 * ║ - int: integer value
 * ║
 * ╚════════════════════════════════════════════╝
 */
static int toInt( const std::string& input );

/* ╔═ isRegister ══
 * ║
 * ║ checks if the parameter string is a valid
 * ║ register.
 * ║ to an integer.
 * ║
 * ║ parameters:
 * ║ - input: parameter as string (like "#a")
 * ║ returns:
 * ║ - bool: true, if string is a register.
 * ║
 * ╚════════════════════════════════════════════╝
 */
static bool isRegister( const std::string& input_raw );

private:
/* ╔═ parseInteger ══
 * ║
 * ║ Internal function of toInt. Requires that
 * ║ pointer brackets are removed.
 * ║
 * ║ parameters:
 * ║ - input: parameter as string
 * ║   example: "#b", "3", "0xff"
 * ║ returns:
 * ║ - int: integer value
 * ║
 * ╚════════════════════════════════════════════╝
 */
static int parseInteger(const std::string& input);

/* ╔═ parseInteger ══
 * ║
 * ║ If the given string begins with "[" and
 * ║ ends with "]", remove it.
 * ║ pointer brackets are removed.
 * ║
 * ║ parameters:
 * ║ - input: parameter
 * ║   example: "[#a]", "#b"
 * ║ returns:
 * ║ - str: bracket-free string of "input".
 * ║   example: "#a", "#b"
 * ║
 * ╚════════════════════════════════════════════╝
 */
static std::string removeBrackets(const std::string& str);
};
#endif
