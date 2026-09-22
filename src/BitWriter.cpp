#include "BitWriter.hpp"

void BitWriter::writeBit(bool bit) {
    // balról jobbra építjük a bájtot: 7..0 bit-ig
    buffer = static_cast<unsigned char>((buffer << 1) | (bit ? 1 : 0));
    bitCount++;

    // Ha 8 bit összegyűlt -> írjuk ki a fájlba
    if (bitCount == 8) {
        out.put(buffer);
        buffer = 0;
        bitCount = 0;
    }
}

void BitWriter::writeBits(const std::vector<bool>& bits) {
    for (bool bit : bits) {
        writeBit(bit);
    }
}

void BitWriter::flush() {
    if (bitCount > 0) {
        buffer <<= 8 - bitCount;
        out.put(buffer);
        buffer = 0;
        bitCount = 0;
    }
}
