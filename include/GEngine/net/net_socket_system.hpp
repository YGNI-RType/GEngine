/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** net_system_socket
*/

#pragma once

#include "net_socket.hpp"

#include <string>

namespace Network {

#ifdef _WIN32
#define STD_IN STD_INPUT_HANDLE
#define STD_OUT STD_OUTPUT_HANDLE
#define STD_ERR STD_ERROR_HANDLE
#else
#define STD_IN STDIN_FILENO
#define STD_OUT STDOUT_FILENO
#define STD_ERR STDERR_FILENO
#endif

class SocketSTD : public ASocket {
public:
    SocketSTD() = default;
    SocketSTD(int stdNumber);
    SocketSTD(const SocketSTD &other) = delete;
    SocketSTD &operator=(const SocketSTD &) = delete;
    SocketSTD(SocketSTD &&other);
    SocketSTD &operator=(SocketSTD &&other);
    ~SocketSTD() = default;

    int read(std::string &buffer) const;
    int write(const std::string &buffer) const;

    int socketClose(void) override final {
        return socketCloseAdv(false);
    }

    void setStd(int stdNumber);
};
} // namespace Network
