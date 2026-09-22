#ifndef BITREADER_HPP
#define BITREADER_HPP

#include <fstream>
using namespace std;

class BitReader {
private:
    ifstream& in;
    unsigned char buffer;
    int bitCount;

public:
    BitReader(ifstream& input) : in(input), buffer(0), bitCount(0) {}

    // Egyetlen bit olvasasa
    bool readBit();
    int getBitCount();

    // Ellenorzes: van-e meg mit olvasni
    bool eof() const { return in.eof(); }
};

#endif
