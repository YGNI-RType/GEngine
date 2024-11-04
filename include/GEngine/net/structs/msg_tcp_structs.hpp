/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** msg_tcp_structs
*/

#pragma once

#include "GEngine/net/utils/pack.hpp"

namespace Network {
static constexpr uint8_t MAX_CONCOMMAND_NAME_LEN = 128;
static constexpr uint8_t MAX_CONCOMMAND_VAL_LEN = 128;
static constexpr uint16_t MAX_CONCOMMAND_OUTPUT_LEN = 0x1000;

enum {
    SV_INIT_CONNECTON,
    SV_YOU_ARE_READY,
    SV_DOWNLOAD,
    SV_UPLOAD, // TODO : add resriction on files (size, type, etc)
    SV_CVAR
};

enum {
    CL_CONNECT_INFORMATION,
    CL_DOWNLOAD,
    CL_UPLOAD,
    CL_CVAR,
};

PACK(struct TCPSV_ClientInit {
    uint32_t challenge;
    uint16_t udpPort;
    // todo : add more information, that client needs to know that the server won't change
});

PACK(struct TCPCL_ConnectInformation {
    uint16_t udpPort;

    /* todo: add the name, client information */
});

PACK(struct TCPCL_CVar {
    char name[MAX_CONCOMMAND_NAME_LEN];
    char value[MAX_CONCOMMAND_VAL_LEN];
});

PACK(struct TCPSV_CVar {
    uint8_t result;
    char output[MAX_CONCOMMAND_OUTPUT_LEN];
});

} // namespace Network
