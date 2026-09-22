#include "BitReader.hpp"

bool BitReader::readBit() {
    if (bitCount == 0) {
        in.read(reinterpret_cast<char*>(&buffer), 1);
        bitCount = 8;
    }

    bool bit = (buffer & 0b10000000); // legfelso bit
    buffer <<= 1;
    bitCount--;
    return bit;
}
int BitReader::getBitCount() {{
    return bitCount;
}}
