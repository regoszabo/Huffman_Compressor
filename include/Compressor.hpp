#ifndef COMPRESSOR_HPP
#define COMPRESSOR_HPP

#include <string>
#include "FrequencyTable.hpp"
#include "TreeStructure.hpp"
#include "BitWriter.hpp"
using namespace std;

class Compressor {
public:
    static void compress(const string& inputFile, const string& outputFile);
};

#endif
