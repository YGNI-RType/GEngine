/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** net_huffman
*/

#pragma once

#include "net_common.hpp"

#include <array>
#include <cstdint>
#include <memory>

namespace Network {
class AMessage;
}

namespace Network::Compression {

/*
We use the Adaptive Huffman Compression algorithm to compress the data using predefined table for the udp/tcp packets
using segments of compressed data. We get an average 20% compression rate on the data.
https://en.wikipedia.org/wiki/Adaptive_Huffman_coding
*/

#define HMAX 256 /* Maximum symbol */

#define NYT HMAX /* Not Yet Transmitted */
#define INTERNAL_NODE (HMAX + 1)

class Node {
public:
    Node *left, *right, *parent;
    Node *next, *prev; /* doubly-linked list */
    Node **head;       /* highest ranked node in block */

    uint32_t weight;
    uint32_t symbol;

public:
    void swapLL(Node *other);
};

/**
 * @class HuffTable
 * @brief A class representing a Huffman coding table.
 *
 * This class provides functionalities to add symbols to the Huffman table,
 * write symbols to a data buffer, and read symbols from a data buffer.
 *
 * @warning The table should be complete with all the symbols, otherwise we get undefined behavior (UB).
 */
class HuffTable {
public:
    HuffTable();
    ~HuffTable() = default;

    void addSymbol(uint8_t symbol);

    /* Warning: The table should be complete with all the symbols, otherwise we get UB */
    bool writeSymbol(uint8_t symbol, byte_t *data, size_t maxDataSizeBytes, size_t &bitBuffer);
    bool readSymbol(const byte_t *inData, byte_t *outData, size_t sizeToRead, size_t maxDataSizeBytes,
                    size_t &bitBuffer);

private:
    void swap(Node *lnode, Node *lright);
    void increment(Node *node);

    Node **getFreeNode(void);
    void setFreeNode(Node **ppNode);

private:
    size_t m_nextBlock = 0;
    std::array<Node, 0x300> m_nodeList = {};
    std::array<Node *, HMAX + 1> m_nodeIndexSymbol = {}; // loc

    Node **m_curFreeNode = nullptr;
    std::array<Node *, 0x300> m_freeNodes = {};
    size_t m_nextFreeBlock = 0;

    Node *m_tree = nullptr;
    Node *m_tail = nullptr;
    Node *m_head = nullptr;
};

/**
 * @class AHC
 * @brief Adaptive Huffman Coding class for compressing and decompressing messages.
 *
 * This class provides methods to compress and decompress messages using Adaptive Huffman Coding.
 * It supports both segment-based and continuous compression and decompression.
 */
class AHC {
public:
    AHC(bool dpcm = false);
    ~AHC() = default;

    /* compress the segment, shift everthing and update the size */
    void compress(AMessage &msg);
    size_t compressContinuous(AMessage &msg, size_t offset, const byte_t *data, size_t size);

    /* decompress the segment, shift everyhting and update the actual size */
    void decompress(AMessage &msg);
    size_t decompressContinuous(AMessage &msg, size_t offset, byte_t *data, size_t size);

private:
    static std::array<uint32_t, HMAX> m_symbolFrequencies;

private:
    HuffTable m_compress;
    HuffTable m_decompress;
};
} // namespace Network::Compression
