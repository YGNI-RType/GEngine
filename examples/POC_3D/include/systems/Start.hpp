/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Start.hpp
*/

#pragma once

#include "GEngine/interface/components/RemoteLocal.hpp"
#include "GEngine/interface/events/RemoteLocal.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/Systems.hpp"

namespace rtype::system {
class Start : public gengine::System<Start, gengine::system::driver::output::TextureManager,
                                     gengine::interface::component::RemoteLocal>, public gengine::RemoteSystem {
public:
    void init(void) override;
    void onStartEngine(gengine::system::event::StartEngine &);
    void onNewRemoteLocal(gengine::interface::event::NewRemoteLocal &);
    void onDeleteRemoteLocal(gengine::interface::event::DeleteRemoteLocal &e);

private:
    size_t m_nbPlayer = 0;
};
} // namespace rtype::system
