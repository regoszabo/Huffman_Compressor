#ifndef FREQUENCY_TABLE_HPP
#define FREQUENCY_TABLE_HPP

#include <array>
#include <cstdint>
#include <string>
using namespace std;

class FrequencyTable { //gyakoriság tábla
private:
    array<uint64_t, 256> freq; // 256 elem, minden bájtnak 1 hely

public:
    FrequencyTable();                       // konstruktor
    void countFromFile(const string& filename); // fájl beolvasás
    uint64_t get(unsigned char ch) const;// adott bájt gyakorisága
    void set(unsigned char ch, uint64_t f);// bájt gyakoriságának beállítása
};

#endif



