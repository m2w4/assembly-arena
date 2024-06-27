#include "asm.h"
#include "util.h"
#include "preproc.h"
#include "parser.h"
#include "apis/native.h"

std::string loadFileToBuffer(const std::string& fname);
void help();

int main(int argc, char* argv[]) {
    std::vector<std::string> args(argv, argv + argc);
    output_t* output = new output_t( );
    std::string outputFilename;
    std::string unprocessed_code;

    if (args.size() < 2) {
        std::cout << "ArenaASM Assembler v0.1" << std::endl;
        help( );
        delete output;
        return -1;
    }

    unprocessed_code = loadFileToBuffer(args[1]);
    output = ArenaASM::asmBuffer(unprocessed_code, output);

    if (args.size() == 4 && args[2] == "-o") {
        outputFilename = args[3];
        ArenaASM::assembleToFile(output, outputFilename);
    } else if (args.size() == 2)
    {
        ArenaASM::printFormatted(unprocessed_code);
    } else {
        help( );
    }
    delete output;
    return 0;
}

//file
std::string loadFileToBuffer(const std::string& fname) {
    // Open the file
    std::ifstream inputFile(fname);

    // Check if the file is open
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << fname << std::endl;
        return ""; // return an empty string to indicate an error
    }

    // Read the file content into a string
    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string fileContent = buffer.str();

    // Close the file
    inputFile.close();

    // Return the file content
    return fileContent;
}



void help( ) {
    #ifdef __EMSCRIPTEN__
        std::cerr << "ArenaASM Assembler v0.1" << std::endl;
    #else
        std::cerr << "Usage: asm input.asm [-o output.raw]" << std::endl;
    #endif
}


