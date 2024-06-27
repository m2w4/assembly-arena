#include "util.h"

namespace Util
{
    unsigned int swapEndianness(unsigned int value) {
        return ((value >> 24) & 0x000000FF) |       // Move byte 3 to byte 0
            ((value << 8) & 0x00FF0000) |        // Move byte 1 to byte 2
            ((value >> 8) & 0x0000FF00) |        // Move byte 2 to byte 1
            ((value << 24) & 0xFF000000);        // Move byte 0 to byte 3
    }

    void formatInstruction(uint32_t i32) {
        // Print each byte of the integer with a space after each byte
        std::cout << std::hex << std::setfill('0')
                << std::setw(2) << ((i32 >> 24) & 0xFF) << " "
                << std::setw(2) << ((i32 >> 16) & 0xFF) << " "
                << std::setw(2) << ((i32 >> 8) & 0xFF) << " "
                << std::setw(2) << (i32 & 0xFF) << " " << std::endl;
    }

    std::string sformatInstruction(uint32_t i32) {
        std::stringstream ss;

        // Print each byte of the integer with a space after each byte
        ss << std::hex << std::setfill('0')
        << std::setw(2) << ((i32 >> 24) & 0xFF) << " "
        << std::setw(2) << ((i32 >> 16) & 0xFF) << " "
        << std::setw(2) << ((i32 >> 8) & 0xFF) << " "
        << std::setw(2) << (i32 & 0xFF) << " " << std::endl;

        return ss.str();
    }

}
