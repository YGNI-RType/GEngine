/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** net_channel
*/

#pragma once

#include "net_msg.hpp"
#include "net_socket.hpp"

#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

// UN udp message / tcp message ne doit pas être envoyé brut,
// il doit être envoyé à travers une pool, qui en fonction du ratelimit enverront tout ça
// Un classe de message spécialisé, donc avec les headers mêem du udp message qui sotn store dans un gros buffer, dont
// les seuls traces sont des structs, précisant leur segmentation.
// la taille du buffer est réplique pour MAXCLIENTS, dans un convar CMD cappé à 100 jouers

#define MAX_CONCURRENT_FRAGMENTS 3
#define FRAG_SEQUENCE_TABLE_SZ sizeof(uint16_t) * 8

#define UDP_POOL_SZ MAX_UDP_PACKET_LENGTH *FRAG_SEQUENCE_TABLE_SZ *MAX_CONCURRENT_FRAGMENTS
#define TCP_POOL_SZ MAX_TCP_MSGLEN

#define NETCHAN_GENCHECKSUM(challenge, sequence) ((challenge) ^ ((sequence) * (challenge)))

namespace Network {

typedef enum {
    CS_FREE,      // can be reused for a new connection
    CS_ZOMBIE,    // client has been disconnected, but don't reuse
                  // connection for a couple seconds
    CS_CONNECTED, // has been assigned to a client_t, but no gamestate yet
    CS_PRIMED,    // gamestate has been sent, but client hasn't sent a usercmd
    CS_ACTIVE     // client is fully in game
} clientState;

typedef enum {
    CON_UNINITIALIZED,
    CON_DISCONNECTED, // not talking to a server
    CON_AUTHORIZING,  // not used any more, was checking cd key
    CON_CONNECTING,   // sending request packets to the server
    CON_CHALLENGING,  // sending challenge packets to the server
    CON_CONNECTED,    // netchan_t established, getting gamestate
    CON_LOADING,      // only during cgame initialization, never during main loop
    CON_PRIMED,       // got gamestate, waiting for first frame
    CON_ACTIVE,       // game views should be displayed
    CON_CINEMATIC     // playing a cinematic or a static pic, not connected to a server
} connectionState;

/**
 * @class PacketPoolUdp
 * @brief Manages a pool of UDP packets for sending and receiving fragmented messages.
 *
 * This class provides functionality to handle UDP packet fragmentation and reassembly.
 * It maintains a pool of packet sequences and allows adding, retrieving, and reconstructing
 * messages from these sequences.
 *
 * @details
 * The PacketPoolUdp class is responsible for:
 * - Adding messages to the pool.
 * - Retrieving missing fragments of a message.
 * - Constructing and reconstructing messages from fragments.
 * - Managing the sequence information of messages.
 * - Cleaning up old sequences and maintaining the pool size.
 *
 * @note
 * The class uses a fixed chunk size for packet fragments, which is determined by the maximum
 * UDP packet length and the sizes of the fragment and net channel headers.
 */
class PacketPoolUdp {

    static const size_t CHUNK_SIZE =
        MAX_UDP_PACKET_LENGTH - sizeof(UDPG_FragmentHeaderTo) - sizeof(UDPG_NetChannelHeader);

    static constexpr size_t MAX_NB_RECV_FRAG = 5;

public:
    /**
     * @typedef chunk_t
     * @brief Defines a type alias for a fixed-size array of bytes.
     *
     * This type alias represents a chunk of data with a size defined by the constant CHUNK_SIZE.
     * It is used to handle fixed-size blocks of byte data within the network channel.
     */
    typedef std::array<byte_t, CHUNK_SIZE> chunk_t;

private:
    /* type, flag, numbers of chunk, last chunk size, cur mask, pool offset */
    struct PoolSequence {
        PoolSequence() = default;
        PoolSequence(uint8_t type, uint8_t flag, uint8_t size, uint16_t last_size, uint16_t mask, size_t offset)
            : type(type)
            , flag(flag)
            , size(size)
            , last_size(last_size)
            , mask(mask)
            , offset(offset) {
        }
        ~PoolSequence() = default;

        uint8_t type;
        uint8_t flag;
        uint8_t size;
        uint16_t last_size;
        uint16_t mask;
        size_t offset;

        uint64_t receivedLast = 0;
    };

public:
    PacketPoolUdp() = default;
    ~PacketPoolUdp() = default;

    /* send */

    bool addMessage(uint32_t sequence, const UDPMessage &msg);
    std::vector<const chunk_t *> getMissingFragments(uint32_t sequence, uint16_t mask);
    void constructMessage(UDPMessage &msg, const chunk_t *chunk, size_t chunk_size,
                          const UDPG_FragmentHeaderTo &header);

    /* recv */

    bool recvMessage(const UDPMessage &msg, size_t &readOffset, uint32_t &fragSequence, const uint32_t &maxFragSeq);
    std::pair<uint32_t, uint16_t> getCurrentSequence(void);
    uint16_t getMask(uint32_t sequence);
    void reconstructMessage(uint32_t sequence, UDPMessage &msg);

    bool deleteSequence(uint32_t sequence);
    /**
     * @brief Clears the network channel.
     *
     * This function resets the state of the network channel, removing any
     * pending data or state information. It is typically used to prepare
     * the channel for a refresh of the acknowledgment sequence.
     */
    void clear(void);

    PoolSequence getMsgSequenceInfo(uint32_t sequence) const {
        return m_poolSequences.at(sequence);
    }
    bool receivedFullSequence(uint32_t sequence);
    void cleanOldSequences(void);
    size_t getPoolSize(void) const {
        return m_poolSequences.size();
    }

private:
    std::unordered_map<uint32_t, PoolSequence> m_poolSequences;
    std::vector<chunk_t> m_pool; /* pool of packet, just send it straight away, no modifications (header are btw)*/
    size_t m_poolSize = 0;
};

/**
 * @class NetChannel
 * @brief Manages network communication channels for both TCP and UDP protocols.
 *
 * The NetChannel class provides functionalities to handle network communication
 * over TCP and UDP. It supports operations such as reading and sending datagrams
 * and streams, managing connection states, and handling packet sequences.
 *
 * @note This class is not copyable but is movable.
 */
class NetChannel {
public:
    NetChannel(bool isServer, std::unique_ptr<Address> addr, SocketTCP &&socket);
    NetChannel(const NetChannel &other) = delete;
    NetChannel &operator=(const NetChannel &other) = delete;
    NetChannel(NetChannel &&other) = default;
    NetChannel &operator=(NetChannel &&other) = default;
    ~NetChannel() = default;

    const Address &getAddressTCP(void) const {
        return *m_toTCPAddress;
    }
    const Address &getAddressUDP(void) const {
        return *m_toUDPAddress;
    }

    bool isEnabled(void) const {
        return m_enabled;
    }
    bool isUDPEnabled(void) const {
        return m_toUDPAddress != nullptr;
    }
    bool isTCPEnabled(void) const {
        return m_toTCPAddress != nullptr;
    }
    bool isDisconnected(void) const {
        return m_disconnect;
    }

    uint64_t getLastACKPacketId(void) const {
        return m_udpACKFullInSequence;
    }

    const SocketTCP &getTcpSocket(void) const {
        return m_tcpSocket;
    }
    void setTcpSocket(SocketTCP &&socket) {
        m_tcpSocket = std::move(socket);
    }

    uint32_t getChallenge(void) const {
        return m_challenge;
    }
    void setChallenge(int challenge) {
        m_challenge = challenge;
    }
    bool canCommunicate(void) {
        return m_enabled && m_tcpSocket.getSocket() != -1;
    }

    void createUdpAddress(uint16_t udpport);

public:
    void reloadAck(void);

private:
    bool m_reloadingAck = false;

public:
    bool readDatagram(SocketUDP &socket, UDPMessage &msg, size_t &readOffset);
    bool readStream(TCPMessage &msg);

    bool sendDatagram(SocketUDP &socket, UDPMessage &msg);
    bool sendStream(const TCPMessage &msg);

private:
    bool sendDatagrams(SocketUDP &socket, uint32_t sequence,
                       const std::vector<const Network::PacketPoolUdp::chunk_t *> &vec);

public:
    bool isTimeout(void) const;

public: /* THREAD SAFE */
    /**
     * @brief Retrieves the current ping time stamp.
     *
     * This function returns the current ping time stamp, which is typically used
     * to measure the round-trip time for network communication.
     *
     * @return uint16_t The current ping time stamp.
     */
    uint16_t getPing_TS(void) const;
    /**
     * @brief Retrieves the address in a thread-safe manner.
     *
     * This function returns the address as a string. It ensures that the
     * operation is performed in a thread-safe manner, preventing data races
     * and ensuring consistency.
     *
     * @return std::string The address as a string.
     */
    std::string getAddress_TS(void) const;

    /**
     * @brief Retrieves the port number in a thread-safe manner.
     *
     * This function returns the port number associated with the network channel.
     * It ensures that the access to the port number is thread-safe.
     *
     * @return uint16_t The port number.
     */
    uint16_t getPort_TS(void) const;

private:
    static constexpr size_t PING_POOL_SIZE = 30;
    std::array<uint16_t, PING_POOL_SIZE> m_pingPool;
    size_t m_pingPoolSize = 0;

private:
    bool m_enabled = false;
    bool m_disconnect = false;

    std::unique_ptr<Address> m_toTCPAddress; /* the recast to v6 or v4 is done later */
    std::unique_ptr<Address> m_toUDPAddress; /* the recast to v6 or v4 is done later */

    /* UDP */

    uint32_t m_challenge = -1;

    /* most likely fragments, since the packets may be too big (mostly (always) for client from server) */
    PacketPoolUdp m_udpPoolSend;
    PacketPoolUdp m_udpPoolRecv;
    std::unordered_map<uint32_t, std::pair<uint64_t, uint8_t>> m_udpFragmentsOgSequences;

    uint32_t m_droppedPackets = 0;

    /* don't give a shit if the packet went through : voip / camera */
    uint64_t m_udpInSequence = 0;
    uint64_t m_udpOutSequence = 1;

    /* snapshot, needs client acknowledge to do some things */
    uint64_t m_udpACKInSequence = 0;
    uint64_t m_udpACKFullInSequence = 0;
    uint64_t m_udpACKOutSequence = 1;
    uint64_t m_udpACKClientLastACK = 0;

    uint64_t m_udplastsent = 0;
    uint64_t m_udplastrecv = 0;
    size_t m_udplastsentsize = 0;

    uint32_t m_udpMyFragSequence = 1;
    uint32_t m_udpFromFragSequence = 0;
    uint8_t m_udpNbFragSequence = 0; /* number of existing frag sequences */

    /*******/

    /* TCP */

    SocketTCP m_tcpSocket;
    TCPSerializedMessage m_tcpBuffer;
    size_t m_sizeBuffer = 0;

    /**
     * @brief A static mutex used for synchronizing access to shared resources
     *        in the GEngine network channel.
     *
     * This mutex ensures that multiple threads can safely communicate with the
     * GEngine without causing data races or inconsistencies. It is particularly
     * useful for protecting critical sections of code that involve network
     * communication under the _TS methods.
     */
    static std::mutex mg_mutex;

    /*******/
};
} // namespace Network
