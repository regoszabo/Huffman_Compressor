cmake_minimum_required(VERSION 4.0)
project(Huffman_Compressor)

set(CMAKE_CXX_STANDARD 20)

add_executable(Huffman_Compressor
        src/main.cpp
        src/TreeStructure.cpp
        src/FrequencyTable.cpp
        src/Decompress.cpp
        src/Compressor.cpp
        src/BitWriter.cpp
        src/BitReader.cpp
        include/TreeStructure.hpp
        include/FrequencyTable.hpp
        include/Decompress.hpp
        include/Compressor.hpp
        include/BitWriter.hpp
        include/BitReader.hpp
)
target_include_directories(Huffman_Compressor PRIVATE include)

target_compile_options(Huffman_Compressor PRIVATE -Wall -Wextra -Werror -pedantic)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
