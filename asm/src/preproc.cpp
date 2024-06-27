#include "preproc.h"

// Function to perform preprocessing
std::string preprocess(const std::string& input) {
    std::unordered_map<std::string, std::string> symbolTable;
    std::stringstream output;
    std::istringstream iss(input);
    std::string line;

    // Iterate through each line
    while (std::getline(iss, line)) {
        // Regular expression to match #define statements
        std::regex defineRegex("^\\s*#define\\s+(\\w+)\\s+([^\\n\\r]*)\\s*$");
        std::smatch match;

        // If the line matches the #define pattern
        if (std::regex_match(line, match, defineRegex)) {
            symbolTable[match[1].str()] = match[2].str(); // Add to symbol table
            output << ";" << std::endl;
        } else {
            // Replace symbols in the line
            for (const auto& pair : symbolTable) {
                std::regex symbolRegex("\\b" + pair.first + "\\b");
                line = std::regex_replace(line, symbolRegex, pair.second);
            }
            output << line << std::endl; // Output processed line
        }
    }
    return output.str();
}
