/*
  ╔══════════════════════════════════════════════════════════╗
  ║                       analyze.h
  ║ defines the interface for the parser-class
  ║ that handles tokized instructions as input and
  ║ returns a parsed instruction.
  ║ Also handles .labels and relative Jumps
  ║
  ║ Code-Generation Steps:
  ║ preprocessor ==> [ analyzer ] ==> parser ==> assembler
  ║ preproc.h    ==> [analyzer.h] ==> parser.h ==> asm.h
  ╚══════════════════════════════════════════════════════════╝
*/

//TODO: rename TokenTypes
//TODO: use TokenTypes instead of int

#ifndef LEX_H
#define LEX_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <unordered_set>

/* ┌─ TokenTypes             ───┐
 * │  Tokens in which the code
 * │  gets tokenized.
 * ├─ Example:
 * │  JMP              #a
 * │ OPCODE W'SPACE REGISTER
 * └───                      ───┘
 */
enum TokenTypes {
    TK_OPCODE,       // OPCODE
    TK_COMMA,        // COMMA
    TK_REGISTER,     // REGISTER
    TK_NUMBER,       // NUMBER
    TK_POINTER,      // POINTER
    TK_LABEL,        // LABEL
    TK_COMMENT,      // COMMENT
    TK_WHITESPACE    // WHITESPACE
};


//Regular Expressions used by the tokenizer to determine the correct token.
static std::vector<std::pair<int, std::string>> token_patterns = {
    {TK_OPCODE,     "MOV|ADD|SUB|XOR|CMP|JMP|JEQ|JGE|JLO|JNE|JNGE|JNLO"},                    // Mnemonics/Opcodes
    {TK_REGISTER,   "#a|#b|#c|#d"},                                                          // Register operands
    {TK_COMMA,      ","},                                                                    // Separator
    {TK_NUMBER,     "^[+-]?(0x[0-9A-Fa-f]+|\\d+)"},                                          // Numeric constants
    {TK_POINTER,    "\\[(#[a-dA-D]|[+-]?0[bB][01]+|[+-]?0[xX][0-9a-fA-F]+|[+-]?[0-9]+)\\]"}, // pointers
    {TK_LABEL,      "\\.[a-zA-Z0-9]+"},                                                      // Labels (start with point, followed by letters or digits)
    {TK_COMMENT,    ";(.*)"},                                                                // Comments
    {TK_WHITESPACE, "\\s+"},                                                                 // Whitespace
};

/* ╔═ Analyzer ══
 * ║ Analyzes strings lexical and syntactical.
 * ║ If its valid, it tokenizes valid assembly code.
 * ║ Tip: pair<tokenType_t/int, string> is equal to token.
 * ╚══════════════════════════════════════════════════════╝
 */
class Analyzer {
public:
/* ╔═ tokenizeLine ══
 * ║
 * ║ Tokenizes a single line after doing
 * ║ lexical analysis.
 * ║
 * ║ parameters:
 * ║ - assembly_code: line of code
 * ║   example: "MOV #a, 42"
 * ║ returns vector of <pair>:
 * ║ - i32: value of TokenTypes (enum)
 * ║ - str: token string (example: "MOV", "#a", ...)
 * ║
 * ╚═══════════════════════════════════════════════════╝
 */ //TODO: rename assembly_code to asm_line
static std::vector<std::pair<int, std::string>>              tokenizeLine (const std::string& assembly_code);

/* ╔═ tokenize ══
 * ║
 * ║ Tokenizes multi-line assembly code after doing
 * ║ lexical analysis. It returns a vector of
 * ║ vectorized/tokenized instructions.
 * ║
 * ║ parameters:
 * ║ - asm_code: raw asm-code string
 * ║   example: "MOV #a, 42\nMOV #a, #b"
 * ║ returns vector of vector of <pair>:
 * ║ - i32: value of TokenTypes (enum)
 * ║ - str: token string (example: "MOV", "#a", ...)
 * ║
 * ╚═══════════════════════════════════════════════════╝
 */ //TODO: rename fileContent to asm_code
static std::vector<std::vector<std::pair<int, std::string>>> tokenize (const std::string& fileContent);
/* ╔═ checkSyntax ══
 * ║
 * ║ Does syntax analysis of a tokenized line.
 * ║
 * ║ parameters:
 * ║ - tokens: tokenized line
 * ║ returns:
 * ║ - bool: true, if syntax is valid.
 * ║
 * ╚═══════════════════════════════════════════════════╝
 */
static bool checkSyntax (std::vector<std::pair<int, std::string>> tokens);


};

#endif
