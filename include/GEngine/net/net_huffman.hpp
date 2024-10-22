/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** net_huffman
*/

#pragma once

#include <array>
#include <cstdint>
#include <memory>

namespace Network::Compression {

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

class HuffTable {
public:
    HuffTable();
    ~HuffTable() = default;

    void addSymbol(uint8_t symbol);

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

/* Adaptative Huffman Compression */
class AHC {
public:
    AHC(bool dpcm = false);
    ~AHC() = default;

private:
    static std::array<uint32_t, HMAX> m_symbolFrequencies;

private:
    HuffTable compress;
    HuffTable decompress;
};
} // namespace Network::Compression
