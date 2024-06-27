#include "asm.h"
#include "util.h"
#include "preproc.h"
#include "parser.h"


output_t* assembleBuffer( std::string unprocessed_code, output_t* output) {
    output->ok = true;
    std::unique_ptr<Parser> parser = std::make_unique<Parser>();
    std::string assembly_code = preprocess( unprocessed_code );
    std::vector<std::vector<std::pair<int, std::string>>> line_tokens = Analyzer::tokenize(assembly_code);

    std::pair<bool, uint32_t> instruction;
    unsigned int location = 0;
    for (const auto& tokens : line_tokens) {
        location += parser->fetchLabels(tokens, location);
    }

    for (size_t i = location = 0; i < line_tokens.size(); ++i) {
        const auto& tokens = line_tokens[i];
        if(Analyzer::checkSyntax(tokens))
        {
            instruction = parser->parseInstruction(tokens, location);
            if(instruction.first) {
                output->binaryData.push_back(Util::swapEndianness(instruction.second));
                output->readableData << Util::sformatInstruction( instruction.second );
                location += 4;
            } else {
                if(tokens.size() > 0)
                {
                        if(tokens.front().first != TK_COMMENT && tokens.front().first != TK_WHITESPACE && tokens.front().first != TK_LABEL) {
                        output->ok = true;
                        std::cerr << "Syntax error at line " << (i+1) << std::endl;
                        throw std::runtime_error("Syntax Error");
                    }
                }
                output->readableData << "-- -- -- -- ";
            }
        } else {
            output->ok = true;
            std::cerr << "Syntax error at line " << (i+1) << std::endl;
            throw std::runtime_error("Syntax Error");

        }
    }
    return output;
}
