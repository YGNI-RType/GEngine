/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** net_huffman
*/

#include "GEngine/net/net_huffman.hpp"
#include "GEngine/net/msg.hpp"
#include "GEngine/net/net_exception.hpp"

namespace Network::Compression {

AHC::AHC(bool dpcm) {
    /* Add the frequency table to the huffman table, creating it in the process */
    for (size_t sym = 0; const auto freq : m_symbolFrequencies) {
        for (int32_t i = 0; i < freq; i++) {
            m_compress.addSymbol(sym);
            m_decompress.addSymbol(sym);
        }
        sym++;
    }
}

/* do this if you want to compress one time */

void AHC::compress(AMessage &msg) {
    HuffTable huffCompress;

    size_t size;
    void *data = nullptr;
    const size_t msgSize = msg.getSize();

    if (!msg.getCompressingBuffer(data, size))
        return;
    if (data == nullptr)
        return;

    /* Use huffCompress instead of m_compress */
}

size_t AHC::compressContinuous(AMessage &msg, size_t offset, const byte_t *pushData, size_t size) {
    byte_t *msgData = reinterpret_cast<byte_t *>(msg.getModifyData() + offset);
    auto msgSize = msg.getMaxMsgSize() - offset;
    size_t oldBitBuffer = msg.getBitBuffer();
    auto bitRemaningUntilByte = msg.getBitRemains();

    for (size_t i = 0; i < size; i++)
        if (!m_compress.writeSymbol(pushData[i], msgData, msgSize, msg.getBitBuffer()))
            throw NetException("Message Overflow", EL_ERR_COMPRESSION);

    auto diff = msg.getBitBuffer() - oldBitBuffer;
    return diff / 8 + 1;
}

size_t AHC::decompressContinuous(AMessage &msg, size_t offset, byte_t *pushData, size_t size) {
    auto offsetBits = msg.getBitBuffer();
    const size_t msgSize = msg.getMaxMsgSize() - offset;
    size_t old = msg.getBitBuffer();

    if (!m_decompress.readSymbol(msg.getData() + offset, pushData, size, msgSize, msg.getBitBuffer()))
        throw NetException("Message Overflow", EL_ERR_COMPRESSION);

    auto diff = msg.getBitBuffer() - old;
    return diff;
}

/*************************************************************/

/* swap linked list logic */
void Node::swapLL(Node *rnode) {
    Node *lnodeParent;

    lnodeParent = this->next;
    this->next = rnode->next;
    rnode->next = lnodeParent;

    lnodeParent = this->prev;
    this->prev = rnode->prev;
    rnode->prev = lnodeParent;

    if (this->next == this)
        this->next = rnode;
    if (rnode->next == rnode)
        rnode->next = this;
    if (this->next)
        this->next->prev = this;
    if (rnode->next)
        rnode->next->prev = rnode;

    if (this->prev)
        this->prev->next = this;
    if (rnode->prev)
        rnode->prev->next = rnode;
}

/*************************************************************/

/* Add a bit to the output file (buffered) */
static void add_bit(byte_t bit, byte_t *fout, size_t &bitCount) {
    if ((bitCount & 7) == 0)
        fout[(bitCount / 8)] = 0;
    fout[(bitCount / 8)] |= bit << (bitCount & 7);
    bitCount++;
}

/* Receive one bit from the input file (buffered) */
static byte_t get_bit(const byte_t *inData, size_t &bitCount) {
    byte_t t = (inData[(bitCount / 8)] >> (bitCount & 7)) & 0x1;
    bitCount++;
    return t;
}

HuffTable::HuffTable() {
    m_tree = m_head = m_tail = m_nodeIndexSymbol[NYT] = &(m_nodeList[m_nextBlock++]);

    m_tree->symbol = NYT;
    m_tree->weight = 0;
    m_tree->parent = m_tree->left = m_tree->right = nullptr;

    m_head->next = m_head->prev = nullptr;
}

/* Swap the location of these two nodes in the tree */
void HuffTable::swap(Node *lnode, Node *rnode) {
    Node *lnodeParent, *rnodeParent;

    lnodeParent = lnode->parent;
    rnodeParent = rnode->parent;

    if (lnodeParent)
        if (lnodeParent->left == lnode)
            lnodeParent->left = rnode;
        else
            lnodeParent->right = rnode;
    else
        m_tree = rnode;

    if (rnodeParent)
        if (rnodeParent->left == rnode)
            rnodeParent->left = lnode;
        else
            rnodeParent->right = lnode;
    else
        m_tree = lnode;

    lnode->parent = rnodeParent;
    rnode->parent = lnodeParent;
}

Node **HuffTable::getFreeNode(void) {
    Node **head;

    if (!m_curFreeNode) {
        return &(m_freeNodes[m_nextFreeBlock++]);
    } else {
        head = m_curFreeNode;
        m_curFreeNode = (Node **)*head;
        return head;
    }
}

void HuffTable::setFreeNode(Node **ppNode) {
    *ppNode = (Node *)m_curFreeNode;
    m_curFreeNode = ppNode;
}

void HuffTable::increment(Node *node) {
    if (!node)
        return;

    if (node->next != nullptr && node->next->weight == node->weight) {
        Node *lnode = *node->head;
        if (lnode != node->parent)
            swap(lnode, node);
        lnode->swapLL(node);
    }
    if (node->prev && node->prev->weight == node->weight) {
        *node->head = node->prev;
    } else {
        *node->head = nullptr;
        setFreeNode(node->head);
    }
    node->weight++;
    if (node->next && node->next->weight == node->weight) {
        node->head = node->next->head;
    } else {
        node->head = getFreeNode();
        *node->head = node;
    }

    if (node->parent) {
        increment(node->parent);
        if (node->prev == node->parent) {
            node->swapLL(node->parent);
            if (*node->head == node)
                *node->head = node->parent;
        }
    }
}

void HuffTable::addSymbol(uint8_t sym) {
    Node *tnode, *tnode2;
    if (m_nodeIndexSymbol[sym] == nullptr) { /* if this is the first transmission of this node */
        tnode = &(m_nodeList[m_nextBlock++]);
        tnode2 = &(m_nodeList[m_nextBlock++]);

        tnode2->symbol = INTERNAL_NODE;
        tnode2->weight = 1;
        tnode2->next = m_head->next;
        if (m_head->next) {
            m_head->next->prev = tnode2;
            if (m_head->next->weight == 1) {
                tnode2->head = m_head->next->head;
            } else {
                tnode2->head = getFreeNode();
                *tnode2->head = tnode2;
            }
        } else {
            tnode2->head = getFreeNode();
            *tnode2->head = tnode2;
        }
        m_head->next = tnode2;
        tnode2->prev = m_head;

        tnode->symbol = sym;
        tnode->weight = 1;
        tnode->next = m_head->next;
        if (m_head->next) {
            m_head->next->prev = tnode;
            if (m_head->next->weight == 1) {
                tnode->head = m_head->next->head;
            } else {
                /* impossible */
                tnode->head = getFreeNode();
                *tnode->head = tnode2;
            }
        } else {
            /* impossible */
            tnode->head = getFreeNode();
            *tnode->head = tnode;
        }
        m_head->next = tnode;
        tnode->prev = m_head;
        tnode->left = tnode->right = nullptr;

        if (m_head->parent)
            if (m_head->parent->left == m_head) /* lhead is guaranteed to by the NYT */
                m_head->parent->left = tnode2;
            else
                m_head->parent->right = tnode2;
        else
            m_tree = tnode2;

        tnode2->right = tnode;
        tnode2->left = m_head;

        tnode2->parent = m_head->parent;
        m_head->parent = tnode->parent = tnode2;

        m_nodeIndexSymbol[sym] = tnode;

        increment(tnode2->parent);
    } else {
        increment(m_nodeIndexSymbol[sym]);
    }
}

/* Send the prefix code for this node */
static size_t writeSymbolRec(Node *node, Node *child, byte_t *data, size_t maxoffset, size_t &bitCount) {
    bool shouldContinue = true;
    if (node->parent) {
        shouldContinue = writeSymbolRec(node->parent, node, data, maxoffset, bitCount);
        if (!shouldContinue)
            return false;
    }
    if (child) {
        if (bitCount >= maxoffset) { /* too much info, cancel */
            bitCount = maxoffset + 1;
            return -1;
        }
        add_bit(node->right == child ? 1 : 0, data, bitCount);
    }
    return shouldContinue;
}

bool HuffTable::writeSymbol(uint8_t symbol, byte_t *data, size_t maxDataSizeBytes, size_t &bitBuffer) {
    const size_t maxDataSizeBits = maxDataSizeBytes * 8;
    return writeSymbolRec(m_nodeIndexSymbol[symbol], nullptr, data, maxDataSizeBits, bitBuffer);
}

static bool readSymbolRec(Node *node, const byte_t *inData, byte_t &symbol, const size_t maxDataSizeBits,
                          size_t &readCountBits) {
    while (node && node->symbol == INTERNAL_NODE) {
        if (readCountBits >= maxDataSizeBits)
            return false;
        if (get_bit(inData, readCountBits))
            node = node->right;
        else
            node = node->left;
    }
    if (!node)
        return false;
    symbol = node->symbol;
    return true;
}

bool HuffTable::readSymbol(const byte_t *inData, byte_t *outData, size_t sizeToRead, size_t maxDataSizeBytes,
                           size_t &readCountBits) {
    const size_t maxDataSizeBits = maxDataSizeBytes * 8;
    for (size_t i = 0; i < sizeToRead; i++)
        if (!readSymbolRec(m_tree, inData, outData[i], maxDataSizeBits, readCountBits))
            return false;
    return true;
}

/* TODO : calculate own frequencies */
std::array<uint32_t, HMAX> AHC::m_symbolFrequencies = {
    250315, // 0
    41193,  // 1
    6292,   // 2
    7106,   // 3
    3730,   // 4
    3750,   // 5
    6110,   // 6
    23283,  // 7
    33317,  // 8
    6950,   // 9
    7838,   // 10
    9714,   // 11
    9257,   // 12
    17259,  // 13
    3949,   // 14
    1778,   // 15
    8288,   // 16
    1604,   // 17
    1590,   // 18
    1663,   // 19
    1100,   // 20
    1213,   // 21
    1238,   // 22
    1134,   // 23
    1749,   // 24
    1059,   // 25
    1246,   // 26
    1149,   // 27
    1273,   // 28
    4486,   // 29
    2805,   // 30
    3472,   // 31
    21819,  // 32
    1159,   // 33
    1670,   // 34
    1066,   // 35
    1043,   // 36
    1012,   // 37
    1053,   // 38
    1070,   // 39
    1726,   // 40
    888,    // 41
    1180,   // 42
    850,    // 43
    960,    // 44
    780,    // 45
    1752,   // 46
    3296,   // 47
    10630,  // 48
    4514,   // 49
    5881,   // 50
    2685,   // 51
    4650,   // 52
    3837,   // 53
    2093,   // 54
    1867,   // 55
    2584,   // 56
    1949,   // 57
    1972,   // 58
    940,    // 59
    1134,   // 60
    1788,   // 61
    1670,   // 62
    1206,   // 63
    5719,   // 64
    6128,   // 65
    7222,   // 66
    6654,   // 67
    3710,   // 68
    3795,   // 69
    1492,   // 70
    1524,   // 71
    2215,   // 72
    1140,   // 73
    1355,   // 74
    971,    // 75
    2180,   // 76
    1248,   // 77
    1328,   // 78
    1195,   // 79
    1770,   // 80
    1078,   // 81
    1264,   // 82
    1266,   // 83
    1168,   // 84
    965,    // 85
    1155,   // 86
    1186,   // 87
    1347,   // 88
    1228,   // 89
    1529,   // 90
    1600,   // 91
    2617,   // 92
    2048,   // 93
    2546,   // 94
    3275,   // 95
    2410,   // 96
    3585,   // 97
    2504,   // 98
    2800,   // 99
    2675,   // 100
    6146,   // 101
    3663,   // 102
    2840,   // 103
    14253,  // 104
    3164,   // 105
    2221,   // 106
    1687,   // 107
    3208,   // 108
    2739,   // 109
    3512,   // 110
    4796,   // 111
    4091,   // 112
    3515,   // 113
    5288,   // 114
    4016,   // 115
    7937,   // 116
    6031,   // 117
    5360,   // 118
    3924,   // 119
    4892,   // 120
    3743,   // 121
    4566,   // 122
    4807,   // 123
    5852,   // 124
    6400,   // 125
    6225,   // 126
    8291,   // 127
    23243,  // 128
    7838,   // 129
    7073,   // 130
    8935,   // 131
    5437,   // 132
    4483,   // 133
    3641,   // 134
    5256,   // 135
    5312,   // 136
    5328,   // 137
    5370,   // 138
    3492,   // 139
    2458,   // 140
    1694,   // 141
    1821,   // 142
    2121,   // 143
    1916,   // 144
    1149,   // 145
    1516,   // 146
    1367,   // 147
    1236,   // 148
    1029,   // 149
    1258,   // 150
    1104,   // 151
    1245,   // 152
    1006,   // 153
    1149,   // 154
    1025,   // 155
    1241,   // 156
    952,    // 157
    1287,   // 158
    997,    // 159
    1713,   // 160
    1009,   // 161
    1187,   // 162
    879,    // 163
    1099,   // 164
    929,    // 165
    1078,   // 166
    951,    // 167
    1656,   // 168
    930,    // 169
    1153,   // 170
    1030,   // 171
    1262,   // 172
    1062,   // 173
    1214,   // 174
    1060,   // 175
    1621,   // 176
    930,    // 177
    1106,   // 178
    912,    // 179
    1034,   // 180
    892,    // 181
    1158,   // 182
    990,    // 183
    1175,   // 184
    850,    // 185
    1121,   // 186
    903,    // 187
    1087,   // 188
    920,    // 189
    1144,   // 190
    1056,   // 191
    3462,   // 192
    2240,   // 193
    4397,   // 194
    12136,  // 195
    7758,   // 196
    1345,   // 197
    1307,   // 198
    3278,   // 199
    1950,   // 200
    886,    // 201
    1023,   // 202
    1112,   // 203
    1077,   // 204
    1042,   // 205
    1061,   // 206
    1071,   // 207
    1484,   // 208
    1001,   // 209
    1096,   // 210
    915,    // 211
    1052,   // 212
    995,    // 213
    1070,   // 214
    876,    // 215
    1111,   // 216
    851,    // 217
    1059,   // 218
    805,    // 219
    1112,   // 220
    923,    // 221
    1103,   // 222
    817,    // 223
    1899,   // 224
    1872,   // 225
    976,    // 226
    841,    // 227
    1127,   // 228
    956,    // 229
    1159,   // 230
    950,    // 231
    7791,   // 232
    954,    // 233
    1289,   // 234
    933,    // 235
    1127,   // 236
    3207,   // 237
    1020,   // 238
    927,    // 239
    1355,   // 240
    768,    // 241
    1040,   // 242
    745,    // 243
    952,    // 244
    805,    // 245
    1073,   // 246
    740,    // 247
    1013,   // 248
    805,    // 249
    1008,   // 250
    796,    // 251
    996,    // 252
    1057,   // 253
    11457,  // 254
    13504,  // 255
};
} // namespace Network::Compression
