#include "parser.h"

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

static uint16_t swapBytes(uint16_t num);


uint8_t Parser::getSingleParameterByte( std::pair<int, std::string> tk1 )
{
    uint8_t stx = 0x00, mrm = 0x00;
    if (tk1.first == TK_REGISTER)
    {
        mrm = ParameterParser::toInt(tk1.second);
        stx  = REG_REG;
    }
    if (tk1.first == TK_POINTER )
    {
        //if pointer points to register => stx = 3
        if (ParameterParser::isRegister( tk1.second ) ) {
            mrm = ParameterParser::toInt(tk1.second);
            stx = PREG_REG;
        } else {
            stx = P16_REG;
        }
    }
    if(tk1.first == TK_NUMBER || tk1.first == TK_LABEL)
    {
        stx = I16;
    }


    return mrm + (stx << 4);
}

uint8_t Parser::getDualParameterByte( std::pair<int, std::string> tk1, std::pair<int, std::string> tk2 )
{
    //4-bit syntax
    //first parameter
    uint8_t stx = 0x00, mrm = 0x00;
    if (tk1.first == TK_REGISTER)
    {
        mrm = ParameterParser::toInt(tk1.second)*4;
        stx = REG_REG;
        //printf("mrm=%02x\n", mrm);
    }
    else if (tk1.first == TK_POINTER )
    {
        if (ParameterParser::isRegister( tk1.second ) ) {
            mrm = ParameterParser::toInt(tk1.second)*4;
            stx = PREG_REG;
        } else {
            stx = P16_REG;
        }
    }

    //second parameter | mrm.. (0,3,6)
    if      (tk2.first == TK_REGISTER || stx == P16_REG)stx += 0;
    else if (tk2.first == TK_POINTER )                  stx += 1;
    else if (tk2.first == TK_NUMBER  )                  stx += 2;

    //4-bit MRM
    if (tk2.first == TK_REGISTER)
    {
            mrm += ParameterParser::toInt(tk2.second);
    }    else if (tk2.first == TK_POINTER )
    {
        if (ParameterParser::isRegister( tk2.second ) ) {
            mrm += ParameterParser::toInt(tk2.second);
        }
    }
    //printf("mrm=%02x stx=%02x ", mrm, stx);
    //uint8_t op = mrm + (stx << 4);
    //printf("PRM=%02x\n", op);
    return mrm + (stx << 4);
}


int16_t Parser::getImmediate( std::pair<int, std::string> tk1, unsigned int location )
{
    int16_t imm = 0;
    if (tk1.first == TK_POINTER)
    {
        if (!ParameterParser::isRegister( tk1.second ) ) {
            imm = ParameterParser::toInt(tk1.second);
        }
    } else if (tk1.first == TK_NUMBER)
    {
        imm = ParameterParser::toInt(tk1.second);
        imm = imm-location-4;
        //std::cout << tk1.second << "=" << imm << std::endl;
        //printf("%04x\n", imm-location-4);
    } else if (tk1.first == TK_LABEL)
    {

        imm = this->jumpTable->labels[tk1.second]-location-4;
    }
    return swapBytes(imm);
}


int16_t Parser::getImmediate( std::pair<int, std::string> tk1, std::pair<int, std::string> tk2, unsigned int location)
{
    int16_t imm = 0;
    if (tk1.first == TK_POINTER)
    {
        if (!ParameterParser::isRegister( tk1.second ) ) {
            imm = ParameterParser::toInt(tk1.second);
        }
    } else if (tk1.first == TK_NUMBER)
    {
        imm = ParameterParser::toInt(tk1.second);
    }
    if(tk2.first == TK_POINTER)
    {
        if (!ParameterParser::isRegister( tk2.second ) ) {
            imm = ParameterParser::toInt(tk2.second);
        }
    } else if (tk2.first == TK_NUMBER)
    {
        imm = ParameterParser::toInt(tk2.second);
    }
    return swapBytes(imm);
}

//codegen struct => i32 instruction
std::pair<bool, uint32_t> Parser::parseInstruction( std::vector<std::pair<int, std::string>> tokens, unsigned int location ) {

        std::pair<bool, uint32_t> instruction;
        instruction.first = false;
        instruction.second = 0;

        bool is_instruction = false;
        uint8_t  mrm = 0x00;
        uint16_t imm = 0x0000;

        if(tokens.size( ) > 0)
        {
            if(tokens[0].first == TK_OPCODE)
            {
                is_instruction = true;
                instruction.first = is_instruction;
            } else if(tokens[0].first == TK_LABEL) {
                is_instruction = false;
                instruction.first = is_instruction;
            }
            for (const auto& token : tokens) {
                if(token.first == TK_COMMENT) break;
                //std::cout << "Type: " << token.first << ", Value: " << token.second << std::endl;
            }
        } else { //empty line
            return instruction;
        }

        if(!is_instruction)
        {
            return instruction;
        } else { //instruction
            if( Parser::doesInstructionExist( isa, tokens[0].second ))
            {
                if( isa[tokens[0].second].params_n == 2)
                {
                    //std::cout << "2 params" << std::endl;
                    mrm = this->getDualParameterByte(tokens[1], tokens[3]);
                    imm = this->getImmediate(tokens[1], tokens[3], location);
                    //printf("ins=%02x imm: %x\n",mrm, imm);
                } else if( isa[tokens[0].second].params_n == 1) {
                    //std::cout << "1 param" << std::endl;
                    mrm = this->getSingleParameterByte(tokens[1]);
                    imm = this->getImmediate(tokens[1], location);
                } else if( isa[tokens[0].second].params_n == 0) {
                    //std::cout << "0 param" << std::endl;
                    mrm = 0x00;
                } else {
                    std::cout << "Abort: instruction database corrupted" << std::endl;
                    //TODO: Abort: instruction dtabase corrupted
                }

            } else {
                std::cout << "Abort: unknown instruction" << std::endl;
                //TODO: Abort: unknown instruction
            }

            instruction.second += isa[tokens[0].second].opcode << 24;
            instruction.second += mrm                          << 16;
            instruction.second += imm;
        }
        //printf("mrm=%08x\n",instruction.second);
        //mrm: %08x: instruction.second
        return instruction;
}

bool Parser::doesInstructionExist(const std::map<std::string, instruction_t>& isa, const std::string& keyName) {
    // Use find to check if the key exists
    auto it = isa.find(keyName);
    return it != isa.end( );
}

int Parser::fetchLabels( std::vector<std::pair<int, std::string>> tokens, unsigned int location ) {
        if(tokens.size( ) > 0)
        {
            if(tokens[0].first == TK_OPCODE)
            {
                return 4;
            } else if(tokens[0].first == TK_LABEL) {
                this->jumpTable->labels.insert(std::make_pair(tokens[0].second, location));
                return 0;
            }
        }
        return 0;
}


static uint16_t swapBytes(uint16_t num) {
    return (num >> 8) | (num << 8);
}
