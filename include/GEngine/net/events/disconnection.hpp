/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** disconnection
*/

#pragma once

namespace Network {

class NetClient;

namespace Event {

enum DisonnectType { DT_WANTED, DT_TIMEOUT, DT_RESET };

struct DisconnectInfo {
    NetClient *client;
    DisonnectType type;

    DisconnectInfo(NetClient *client, DisonnectType type)
        : client(client)
        , type(type) {};
};
} // namespace Event

} // namespace Network
