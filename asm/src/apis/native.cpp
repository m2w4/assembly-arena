
#include "native.h"


void ArenaASM::printFormatted(std::string unprocessed_code) {
    //std::string assembly_code = "MOV #a, 42 ; Example x86 assembly code";
    //std::shared_ptr<JumpTable> jumpTable = std::make_shared<JumpTable>();
    std::unique_ptr<Parser> parser = std::make_unique<Parser>();
    std::string assembly_code = preprocess( unprocessed_code);
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
            instruction =  parser->parseInstruction(tokens, location);
            if(instruction.first) {
                Util::formatInstruction( instruction.second );
                location += 4;
            } else {
                printf("-- -- -- --\n");
            }
        } else {
            std::cout << "Syntax error at line " << (i+1) << std::endl;
        }
    }
}

void ArenaASM::assembleToFile(output_t* output, std::string fname)
{
    if (!output) {
        std::cerr << "Error: Invalid output pointer\n";
        return;
    }

    std::ofstream outfile(fname, std::ios::out | std::ios::binary);
    if (!outfile) {
        std::cerr << "Error: Failed to open file " << fname << " for writing\n";
        return;
    }

    outfile.write(reinterpret_cast<const char*>(output->binaryData.data()), output->binaryData.size()* sizeof(uint32_t));

    if (!outfile.good()) {
        std::cerr << "Error: Failed to write data to file " << fname << "\n";
    } else {
        std::cout << "Data written to file " << fname << " successfully\n";
    }
    outfile.close();
}

output_t* ArenaASM::asmBuffer( std::string unprocessed_code, output_t* output)
{
    return assembleBuffer( unprocessed_code, output);
}
