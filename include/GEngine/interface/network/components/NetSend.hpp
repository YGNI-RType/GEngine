/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) System
** ════════════════════════════════════════════════════════════════════════════
**  File        : NetSend.hpp
**  Create at   : 2024-10-15 04:49
**  Author      : AUTHOR
**  Description : DESCRIPTION
** ═══════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include "GEngine/libdev/Component.hpp"

namespace gengine::interface::network::component {
class NetSend : public Component<NetSend> {
public:
    uint8_t version;

    NetSend()
        : version(0) {
    }

    bool operator==(const NetSend &) const = default;
    void update() {
        version++;
    }
};
} // namespace gengine::interface::network::component
