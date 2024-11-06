/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Start.hpp
*/

#include "Constants.hpp"

#include "systems/Lobby.hpp"

#include "components/Background.hpp"
#include "components/Boss.hpp"
#include "components/Invincible.hpp"
#include "components/Life.hpp"
#include "components/Monster.hpp"
#include "components/Score.hpp"
#include "components/ScoreText.hpp"

#include <random>

namespace rtype::system {
void Lobby::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&Lobby::initGame);
    subscribeToEvent<gengine::system::event::GameLoop>(&Lobby::checkPlayersReady);
    subscribeToEvent<gengine::interface::event::NewRemoteLocal>(&Lobby::onNewRemoteLocal);
    subscribeToEvent<gengine::interface::event::DeleteRemoteLocal>(&Lobby::onDeleteRemoteLocal);
    subscribeToEvent<gengine::interface::event::SharedEvent<event::IAmReady>>(&Lobby::setPlayersReady);
}

void Lobby::initGame(gengine::system::event::StartEngine &e) {
    spawnEntity(component::ScoreText(), geg::component::Transform2D({1280 - 315, 726}, {2, 2}),
                geg::component::io::Drawable(2), geg::component::io::Text("arcade.ttf", "Score: 0", WHITE),
                geg::component::network::NetSend());
}

void Lobby::setPlayersReady(gengine::interface::event::SharedEvent<event::IAmReady> &e) {
    if (m_started)
        return;
    auto &players = getComponents<component::Player>();
    auto &netsends = getComponents<gengine::interface::network::component::NetSend>();
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();

    for (auto [entity, player, remote, netsend] : gengine::Zip(players, remotes, netsends))
        if (e.remoteUUID == remote.getUUIDBytes())
            player.ready = e->ready;
}

void Lobby::checkPlayersReady(gengine::system::event::GameLoop &e) {
    if (m_started || !m_nbPlayer)
        return;

    auto &players = getComponents<component::Player>();

    for (auto &[entity, player] : players)
        if (!player.ready)
            return;
    publishEvent(event::StartGame());
    m_started = true;
}

void Lobby::onNewRemoteLocal(gengine::interface::event::NewRemoteLocal &e) {
    if (m_started)
        return;
    spawnEntity(
        component::Player(), geg::component::Transform2D({0, WINDOW_HEIGHT / 2 - (17 * 3) / 2.0f}, {3, 3}, 0),
        geg::component::Velocity2D(0, 0), geg::component::io::Drawable(1),
        geg::component::io::Sprite("spaceships.gif", Rectangle{66, 17.f * (m_nbPlayer % 5), 33, 17}),
        geg::component::HitBoxSquare2D(33, 17), gengine::interface::component::RemoteLocal(e.uuid),
        geg::component::io::Animation("spaceships.json/fly", 0.1f, geg::component::io::AnimationTrack::Idling, 2),
        component::Score(0), geg::component::network::NetSend());
    m_nbPlayer++;
}

void Lobby::onDeleteRemoteLocal(gengine::interface::event::DeleteRemoteLocal &e) {
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();
    for (auto &[entity, remote] : remotes) {
        if (remote.getUUIDBytes() == e.uuid) {
            killEntity(entity);
            m_nbPlayer--;
            return;
        }
    }
}
} // namespace rtype::system
