# Huffman Codec

A lossless compression tool for arbitrary binary files, implemented in C++20. It builds a
Huffman tree from a file's byte-frequency distribution and re-encodes the file as a
variable-length bitstream, storing everything needed to reverse the process — the byte
frequencies — directly in the output file, so decompression never depends on an external
codebook.

## How it works

Huffman coding replaces fixed 8-bit bytes with variable-length codes: bytes that occur
often get short codes, rare bytes get longer ones. On non-uniform data this reduces the
total number of bits needed to represent the file, with no loss of information.

**Compression pipeline** (`Compressor::compress`):

1. **Frequency counting** — `FrequencyTable` reads the input file once and counts how many
   times each of the 256 possible byte values occurs.
2. **Tree construction** — `TreeStructure::build` pushes one leaf node per distinct byte
   into a min-heap (ordered by frequency), then repeatedly pops the two smallest nodes and
   merges them into a new internal node, until a single root remains. This is the standard
   greedy Huffman construction and runs in O(n log n) for n ≤ 256 distinct symbols.
3. **Code table generation** — a depth-first walk of the tree assigns each leaf a bit
   sequence: `0` for every left branch, `1` for every right branch. Because every symbol
   sits at a leaf, no code is a prefix of another, so the resulting bitstream can be decoded
   unambiguously with no separators between codes.
4. **Header + bitstream output** — the byte frequencies (not the tree itself) are written to
   the output file, followed by the packed bitstream. Storing frequencies instead of the
   tree is sufficient: the decoder reruns the identical tree-construction algorithm and
   arrives at the same tree.

**Decompression** (`Decoder::decompress`) reverses this: it reads the frequency table,
rebuilds the identical tree, then walks the tree bit by bit from the root — left on `0`,
right on `1` — emitting a byte each time it reaches a leaf and returning to the root.

### File format

```
──────────────────────────────────────────────────────────────────────────────────────────────
│ usedCount      │ (symbol, frequency) pairs          │ validBits    │ packed bitstream      │ 
│ uint16         │ usedCount × (uint8 + uint64)       │ uint64       │ zero-padded to a      │
│                │                                    │              │ whole number of bytes │
└───────────────┴──────────────────────────────────┴──────────────┴───────────────────────────
```

`validBits` records the exact number of meaningful bits in the stream, so the decoder knows
where real data ends and the zero-padding (added by `BitWriter::flush` to complete the final
byte) begins.

## Class overview

| File | Responsibility |
|---|---|
| `FrequencyTable` | Counts occurrences of each of the 256 possible byte values in a file; also used by the decoder to rebuild the table from the stored header. |
| `TreeStructure` | Builds the Huffman tree from a `FrequencyTable` via a priority queue (min-heap on frequency); generates the leaf → bit-sequence code table via depth-first traversal. |
| `BitWriter` | Buffers individual bits into bytes and writes them to an output stream; `flush()` pads and emits any partial final byte. |
| `BitReader` | The mirror of `BitWriter` — reads a byte at a time from the input stream and serves it back one bit at a time. |
| `Compressor` | Orchestrates compression: builds the frequency table and tree, writes the header, then streams the input file through the code table and `BitWriter`. |
| `Decoder` | Orchestrates decompression: reads the header, rebuilds the tree, then walks it bit by bit via `BitReader` to reconstruct the original bytes. |
| `main.cpp` | Self-contained test harness — generates a small and a large synthetic input file, round-trips each through compression and decompression, and verifies the output is byte-identical to the input. |

## Build & run

Requires a C++20 compiler and CMake ≥ 3.16.

```bash
cmake -B build -S .
cmake --build build
./build/code
```

The build enforces `-Wall -Werror -Wextra -pedantic`.

## Benchmarks

Measured on this repository's test harness (repeating-pattern synthetic data) and on
additional ad-hoc inputs:

| Input | Original size | Compressed size | Ratio | Time |
|---|---|---|---|---|
| Repeating 7-byte pattern | 200 B | 145 B | ~1.4 : 1 | — |
| Repeating 15-byte pattern | 200 KB | 60 KB | ~3.3 : 1 | — |
| English-word text corpus | 20.8 MB | 10.8 MB | ~1.9 : 1 | ~1.5 s |
| Uniformly random bytes | 20 MB | 20.0 MB + 2.3 KB | ~1 : 1 (expected — see below) | ~1.75 s |

Random data doesn't compress under any lossless scheme, by definition — an ideal Huffman
coder on uniformly distributed bytes converges to roughly 8 bits/byte, i.e. no size
reduction, plus a small, fixed header overhead. The measurement above confirms the
implementation behaves as theory predicts, rather than being a shortcoming.


