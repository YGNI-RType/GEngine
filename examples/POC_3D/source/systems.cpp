/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** shared.cpp
*/

#include "GEngine/libdev/Systems.hpp"
#include "GEngine/GEngine.hpp"

#include "GEngine/libdev/systems/driver/input/KeyboardCatcher.hpp"
#include "GEngine/libdev/systems/driver/output/Animate.hpp"
#include "GEngine/libdev/systems/driver/output/Draw.hpp"
#include "GEngine/libdev/systems/driver/output/FontManager.hpp"
#include "GEngine/libdev/systems/driver/output/RenderWindow.hpp"
#include "GEngine/libdev/systems/driver/output/SoundManager.hpp"
#include "GEngine/libdev/systems/driver/output/TextureManager.hpp"

#include "Constants.hpp"
#include "systems/BackgroundMotion.hpp"
#include "systems/InputsToGameEvents.hpp"
#include "systems/PlayerMotion.hpp"
#include "systems/Start.hpp"

#include "GEngine/interface/network/systems/ClientEventPublisher.hpp"
#include "GEngine/interface/network/systems/ServerEventReceiver.hpp"

#include "GEngine/interface/events/RemoteLocal.hpp"
#include "GEngine/interface/systems/RemoteLocal.hpp"

void GEngineDeclareSystems(Registry *r) {
    r->registerSystem<gengine::system::driver::output::RenderWindow>(WINDOW_WIDTH, WINDOW_TOTAL_HEIGHT, "R-Type");
    r->registerSystem<gengine::system::driver::output::Draw>(BLACK);
    r->registerSystem<gengine::system::driver::output::DrawSprite>();
    r->registerSystem<gengine::system::driver::output::DrawModel>();
    r->registerSystem<gengine::system::driver::output::DrawText>();
    r->registerSystem<gengine::system::driver::output::DrawRectangle>();

    r->registerSystem<gengine::system::driver::output::TextureManager>("../assets/sprites");
    r->registerSystem<gengine::system::driver::output::FontManager>("../assets/fonts");
    r->registerSystem<gengine::system::driver::input::KeyboardCatcher>();
    r->registerSystem<gengine::system::driver::output::SoundManager>("../assets/sounds");
    r->registerSystem<gengine::system::driver::output::ModelManager>("../assets/models");
    r->registerSystem<geg::system::io::AnimationManager>("../assets/animations");
    r->registerSystem<gengine::system::driver::output::Animate>();

    r->registerSystem<gengine::system::Motion2D>();
    r->registerSystem<gengine::system::Motion3D>();
    r->registerSystem<gengine::system::MotionAcceleration3D>();
    r->registerSystem<gengine::system::Collision2D>();
    r->registerSystem<gengine::system::AutoKiller>();

    r->registerSystem<rtype::system::InputsToGameEvents>();
    r->registerSystem<rtype::system::Start>();
    r->registerSystem<rtype::system::BackgroundMotion>();
    r->registerSystem<rtype::system::PlayerMotion>();

    r->registerSystem<gengine::interface::system::HandleRemoteLocal>();
    r->registerSystem<gengine::interface::system::HandleLocal>();

    r->registerSystem<gengine::interface::network::system::ClientEventPublisher<
        rtype::event::Movement, rtype::event::Rotation, rtype::event::Jump,
        gengine::interface::event::GetRemoteLocalWhoIAm>>();
    r->registerSystem<gengine::interface::network::system::ServerEventReceiver<
        rtype::event::Movement, rtype::event::Rotation, rtype::event::Jump,
        gengine::interface::event::GetRemoteLocalWhoIAm>>();

    // TODO auto register ↓
}
