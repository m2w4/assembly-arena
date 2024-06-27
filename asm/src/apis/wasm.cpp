//webasm api

#include "wasm.h"
#include "../util.h"
#include <cstring> // for std::memcpy

static void web_debugBinaryPrint(const uint8_t *data, size_t len);

extern "C" {

    EMSCRIPTEN_KEEPALIVE
    char* web_getReadableData( output_t* output )
    {
        std::string str = output->readableData.str();
        char* c_str = new char[str.length( )+1];
        std::memcpy(c_str, str.c_str( ), str.length( ));
        c_str[str.length( )] = 0x00;
        return c_str;
    }

    EMSCRIPTEN_KEEPALIVE
    unsigned char* web_getBinaryData( output_t* output )
    {
        // Calculate the total size required
        size_t dataSize = output->binaryData.size() * sizeof(uint32_t);
        unsigned char* charData = new uint8_t[dataSize];
        // Copy binaryData into charData
        std::memcpy(charData, output->binaryData.data(), dataSize);
        return charData;
    }
    EMSCRIPTEN_KEEPALIVE
    unsigned int web_getBinaryData_n( output_t* output )
    {
        // Calculate the total size required
        return output->binaryData.size() * sizeof(uint32_t);
    }

    EMSCRIPTEN_KEEPALIVE
    output_t* web_assembleBuffer(const char* unprocessed_code) {
        std::cout << "web_assembleBuffer: Processing code.." << std::endl;
        output_t* p = new output_t;
        p = assembleBuffer(unprocessed_code, p);
        return p;
    }

    EMSCRIPTEN_KEEPALIVE
    void web_freeOutput( output_t* output) {
        delete output;
    }

}

static void web_debugBinaryPrint(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02x ", data[i]);
        if ((i + 1) % 4 == 0) // Newline after every 4 bytes
            printf("\n");
    }
    printf("\n");
}


int web_debugPrint( std::string unprocessed_code ) {
    output_t* output = new output_t;

    output = assembleBuffer( unprocessed_code, output );
    std::cout << output->readableData.str() << std::endl;

    // Calculate the total size required
    size_t dataSize = output->binaryData.size() * sizeof(uint32_t);
    unsigned char* charData = new uint8_t[dataSize];

    // Copy binaryData into charData
    std::memcpy(charData, output->binaryData.data(), dataSize);
    web_debugBinaryPrint(charData, dataSize);
    return 0;
}
