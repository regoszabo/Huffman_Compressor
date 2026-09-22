#include "FrequencyTable.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace std;

FrequencyTable::FrequencyTable() {
    freq.fill(0); // minden elem 0-ról indul
}

void FrequencyTable::countFromFile(const std::string& filename) {
    ifstream in(filename, std::ios::binary);
    if (!in) {
        throw runtime_error("Cannot open file: " + filename);
    }

    unsigned char byte;
    while (in.read(reinterpret_cast<char*>(&byte), 1)) {// addig olvasunk, amíg van bájt a fájlban
        freq[byte]++; //gyakoriság növelése
    }
    in.close();
}

uint64_t FrequencyTable::get(unsigned char ch) const {
        return freq[ch]; //adott szimbólum gyakorisága
}
void FrequencyTable::set(unsigned char ch, uint64_t f){
    freq[ch] = f;//adott szimbólum gyakoriságának beállítása
}

