
#include "parameterParser.h"

//  Parameter
//    vv
// 00 XX 0000
    int ParameterParser::toInt( const std::string& input ) {
        return parseInteger( ParameterParser::removeBrackets(input) );
    }

    bool ParameterParser::isRegister( const std::string& input_raw ) {
        std::string input =  removeBrackets(input_raw);
        if      (input == "#a") return true;
        else if (input == "#b") return true;
        else if (input == "#c") return true;
        else if (input == "#d") return true;
        return false;
    }

    int ParameterParser::parseInteger(const std::string& input) {
        // Check for special strings
        if (input == "#a") return 0;
        if (input == "#b") return 1;
        if (input == "#c") return 2;
        if (input == "#d") return 3;

        // Check if it's a hexadecimal representation
        if (input.substr(0, 2) == "0x" || input.substr(0, 3) == "+0x" || input.substr(0, 3) == "-0x") {
            try {
                return std::stoi(input, nullptr, 16);
            } catch (const std::invalid_argument&) {
                std::cerr << "Invalid input" << std::endl;
                throw std::invalid_argument("Invalid input");
            } catch (const std::out_of_range&) {
                std::cerr << "Integer out of range error" << std::endl;
                throw std::out_of_range("Integer out of range");
            }
        }

        // Check if it's a binary representation
        if (input.substr(0, 2) == "0b" || input.substr(0, 3) == "+0b" || input.substr(0, 3) == "-0b") {
            try {
                if ( input.substr(0, 3) == "+0b" || input.substr(0, 3) == "-0b" )
                    return std::stoi(input.substr(3), nullptr, 2);
                else
                    return std::stoi(input.substr(2), nullptr, 2);
            } catch (const std::invalid_argument&) {
                throw std::invalid_argument("Invalid input");
            } catch (const std::out_of_range&) {
                throw std::out_of_range("Integer out of range");
            }
        }

        // Assume it's a decimal representation
        try {
            return std::stoi(input);
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Invalid input");
        } catch (const std::out_of_range&) {
            throw std::out_of_range("Integer out of range");
        }
    }

    std::string ParameterParser::removeBrackets(const std::string& str) {
        std::string result = str;

        if (!result.empty() && result.front() == '[' && result.back() == ']') {
            result.erase(result.begin()); // Remove first character
            result.pop_back(); // Remove last character
        }

        return result;
    }
