/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** msg_tcp_structs
*/

#pragma once

namespace Network {
enum {
    SV_INIT_CONNECTON,
    SV_DOWNLOAD,
    SV_UPLOAD, // TODO : add resriction on files (size, type, etc)
};

enum {
    CL_CONNECT_INFORMATION,
    CL_DOWNLOAD,
    CL_UPLOAD,
};

struct TCPSV_ClientInit {
    int challenge;
    uint16_t udpPort;
    // todo : add more information, that client needs to know that the server won't change
};

struct TCPCL_ConnectInformation {
    uint16_t udpPort;

    /* todo: add the name, client information */
};

} // namespace Network
