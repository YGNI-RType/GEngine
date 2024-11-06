/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** shared.cpp
*/

#include "GEngine/libdev/Systems.hpp"
#include "GEngine/GEngine.hpp"

#include "GEngine/interface/network/systems/ClientServer.hpp"
#include "GEngine/libdev/systems/driver/input/KeyboardCatcher.hpp"
#include "GEngine/libdev/systems/driver/output/Animate.hpp"
#include "GEngine/libdev/systems/driver/output/AudioManager.hpp"
#include "GEngine/libdev/systems/driver/output/Draw.hpp"
#include "GEngine/libdev/systems/driver/output/FontManager.hpp"
#include "GEngine/libdev/systems/driver/output/RenderWindow.hpp"
#include "GEngine/libdev/systems/driver/output/TextureManager.hpp"
#include "GEngine/libdev/systems/gui/SceneManager.hpp"
#include "GEngine/libdev/systems/gui/Widgets.hpp"

#include "Constants.hpp"
#include "RTypeManager.hpp"
#include "systems/BackgroundMotion.hpp"
#include "systems/BossManager.hpp"
#include "systems/BoundingMotion.hpp"
#include "systems/BulletManager.hpp"
#include "systems/ClearEntities.hpp"
#include "systems/DestroyOnCollision.hpp"
#include "systems/FlappingMotion.hpp"
#include "systems/InputsToGameEvents.hpp"
#include "systems/Invincibility.hpp"
#include "systems/Lobby.hpp"
#include "systems/MobManager.hpp"
#include "systems/PlayerAnimation.hpp"
#include "systems/PlayerGui.hpp"
#include "systems/PlayerMotion.hpp"
#include "systems/PlayerShoot.hpp"
#include "systems/Scenes.hpp"
#include "systems/StageManager.hpp"
#include "systems/UpdateScoreText.hpp"

#include "GEngine/interface/network/systems/ClientEventPublisher.hpp"
#include "GEngine/interface/network/systems/ServerEventReceiver.hpp"

#include "GEngine/interface/events/RemoteLocal.hpp"
#include "GEngine/interface/systems/RemoteLocal.hpp"
#include "events/BecomeInvincible.hpp"

#include "GEngine/libdev/systems/CLI.hpp"

void GEngineDeclareSystems(Registry *r) {
    rtype::RTypeManager rm("../r-type.json"); // TODO enlever ..

    r->registerSystem<gengine::system::driver::output::RenderWindow>(WINDOW_WIDTH, WINDOW_TOTAL_HEIGHT, "R-Type");
    r->registerSystem<gengine::system::driver::output::Draw>(BLACK);
    r->registerSystem<gengine::system::driver::output::DrawSprite>();
    r->registerSystem<gengine::system::driver::output::DrawText>();
    r->registerSystem<gengine::system::driver::output::DrawRectangle>();

    r->registerSystem<gengine::system::driver::output::TextureManager>(rm.getManagerPath("textureManager"));
    r->registerSystem<gengine::system::driver::output::FontManager>(rm.getManagerPath("fontManager"));
    r->registerSystem<gengine::system::driver::input::MouseCatcher>();
    r->registerSystem<gengine::system::driver::input::KeyboardCatcher>();
    r->registerSystem<gengine::system::driver::output::AudioManagerLocal>(rm.getManagerPath("sounds"),
                                                                          rm.getManagerPath("musics"));
    r->registerSystem<gengine::system ::driver::output::AudioManagerRemote>(rm.getManagerPath("sounds"),
                                                                            rm.getManagerPath("musics"));
    // r->registerSystem<gengine::system ::driver::output::AudioManagerLocal>(rm.getManagerPath("AudioManager"));
    r->registerSystem<geg::system::io::AnimationManager>(rm.getManagerPath("animationManager"));
    r->registerSystem<rtype::system::MobManager>(rm.getManagerPath("mobManager"));
    r->registerSystem<rtype::system::BossManager>(rm.getManagerPath("bossManager"));
    r->registerSystem<rtype::system::BulletManager>(rm.getManagerPath("bulletManager"));
    r->registerSystem<rtype::system::StageManager>(rm.getManagerPath("stageManager"));
    r->registerSystem<gengine::system::driver::output::Animate>();

    r->registerSystem<gengine::system::gui::SceneManager>(rtype::system::gui::MAINMENU);
    r->registerSystem<gengine::system::gui::ButtonHandler>();
    r->registerSystem<gengine::system::gui::ToggleButtonHandler>();
    r->registerSystem<gengine::system::gui::SelectButtonHandler>();
    r->registerSystem<gengine::system::gui::InputBoxHandler>();
    r->registerSystem<rtype::system::gui::MainMenu>();
    r->registerSystem<rtype::system::gui::Servers>();
    r->registerSystem<rtype::system::gui::Settings>();
    r->registerSystem<rtype::system::gui::GameLobby>();

    r->registerSystem<gengine::system::Motion2D>();
    r->registerSystem<gengine::system::Collision2D>();
    r->registerSystem<gengine::system::AutoKiller>();

    r->registerSystem<rtype::system::InputsToGameEvents>();
    r->registerSystem<rtype::system::Lobby>();
    r->registerSystem<rtype::system::BoundingMotion>();
    r->registerSystem<rtype::system::FlappingMotion>();
    r->registerSystem<rtype::system::PlayerMotion>();
    r->registerSystem<rtype::system::PlayerAnimation>();
    r->registerSystem<rtype::system::PlayerShoot>();
    r->registerSystem<rtype::system::PlayerGui>();
    r->registerSystem<rtype::system::BackgroundMotion>();
    r->registerSystem<rtype::system::ClearEntities>();
    r->registerSystem<rtype::system::DestroyOnCollision>();
    r->registerSystem<rtype::system::UpdateScoreText>();
    r->registerSystem<rtype::system::Invincibility>();

    r->registerSystem<gengine::interface::system::HandleRemoteLocal>();
    r->registerSystem<gengine::interface::system::HandleLocal>();

    r->registerSystem<gengine::system::CLI>();

    r->registerSystem<gengine::interface::network::system::ClientEventPublisher<
        rtype::event::IAmReady, rtype::event::Movement, rtype::event::Shoot,
        gengine::interface::event::GetRemoteLocalWhoIAm, rtype::event::BecomeInvincible,
        gengine::system::event::driver::output::SoundPlayed>>();
    r->registerSystem<gengine::interface::network::system::ServerEventReceiver<
        rtype::event::IAmReady, rtype::event::Movement, rtype::event::Shoot,
        gengine::interface::event::GetRemoteLocalWhoIAm, rtype::event::BecomeInvincible,
        gengine::system::event::driver::output::SoundPlayed>>();

    // TODO auto register ↓
}
