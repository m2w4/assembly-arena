#include "analyze.h"

std::vector<std::pair<int, std::string>> Analyzer::tokenizeLine(const std::string& assembly_code) {
    std::vector<std::pair<int, std::string>> tokens;
    size_t position = 0;

    while (position < assembly_code.length()) {
        std::smatch match;
        bool found = false;

        for (const auto& [token_type, pattern] : token_patterns) {
            std::regex regex(pattern);
            if (std::regex_search(assembly_code.begin() + position, assembly_code.end(), match, regex, std::regex_constants::match_continuous)) {
                std::string value = match.str();
                if (token_type != TK_WHITESPACE) {
                    tokens.emplace_back(token_type, value);
                }
                position += match.length();
                found = true;
                break;
            }
        }

        if (!found) {
            throw std::runtime_error("("+assembly_code.substr(position)+") at position " + std::to_string(position) + " ");
        }
    }

    return tokens;
}

std::vector<std::vector<std::pair<int, std::string>>> Analyzer::tokenize(const std::string& fileContent) {
    std::istringstream buffer(fileContent);
    std::string line;
    std::vector<std::vector<std::pair<int, std::string>>> all_line_tokens;

    int i=1;
    while (std::getline(buffer, line)) {
        try {
            std::vector<std::pair<int, std::string>> line_tokens = Analyzer::tokenizeLine(line);
            all_line_tokens.push_back(line_tokens);
            ++i;
        } catch (const std::exception& e) {
            // Handle the error
            std::cerr << "Syntax Error at line " << i << e.what() << std::endl;
            throw std::runtime_error("Syntax Error");
        }
    }

    return all_line_tokens;
}

bool Analyzer::checkSyntax(std::vector<std::pair<int, std::string>> tokens) {
    std::vector<std::unordered_set<int>> syntaxPatterns = {
        {TK_OPCODE},
        {TK_OPCODE, TK_REGISTER},
        {TK_OPCODE, TK_NUMBER},
        {TK_OPCODE, TK_POINTER},
        {TK_OPCODE, TK_REGISTER, TK_COMMA,TK_REGISTER},
        {TK_OPCODE, TK_REGISTER, TK_COMMA,TK_NUMBER},
        {TK_OPCODE, TK_REGISTER, TK_COMMA,TK_POINTER},
        {TK_OPCODE, TK_POINTER, TK_COMMA,TK_REGISTER},
        {TK_OPCODE, TK_POINTER, TK_COMMA,TK_NUMBER},
        {TK_OPCODE, TK_POINTER, TK_COMMA,TK_POINTER},
        {TK_LABEL},
        {TK_COMMENT}
    };

    for (const auto& token : tokens) {
        for (const auto& pattern : syntaxPatterns) {
            if (pattern.count(token.first)) {
                return true;
            }
        }
    }
    return false; // All tokens match a serial pattern
}


