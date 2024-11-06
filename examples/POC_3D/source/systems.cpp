/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** shared.cpp
*/

#include "GEngine/libdev/Systems.hpp"
#include "GEngine/GEngine.hpp"

#include "GEngine/libdev/systems/driver/input/KeyboardCatcher.hpp"
#include "GEngine/libdev/systems/driver/input/MouseCatcher.hpp"
#include "GEngine/libdev/systems/driver/output/Animate.hpp"
#include "GEngine/libdev/systems/driver/output/Draw.hpp"
#include "GEngine/libdev/systems/driver/output/FontManager.hpp"
#include "GEngine/libdev/systems/driver/output/RenderWindow.hpp"
#include "GEngine/libdev/systems/driver/output/TextureManager.hpp"

#include "Constants.hpp"
#include "systems/BackgroundMotion.hpp"
#include "systems/CameraRotation.hpp"
#include "systems/ChangePlayerModel.hpp"
#include "systems/Guess.hpp"
#include "systems/HUD.hpp"
#include "systems/InputsToGameEvents.hpp"
#include "systems/MorphToProp.hpp"
#include "systems/PlayerMotion.hpp"
#include "systems/ResetPlayerRotation.hpp"
#include "systems/Start.hpp"

#include "GEngine/interface/network/systems/ClientEventPublisher.hpp"
#include "GEngine/interface/network/systems/ClientServer.hpp"
#include "GEngine/interface/network/systems/ServerEventReceiver.hpp"

#include "GEngine/interface/events/RemoteLocal.hpp"
#include "GEngine/interface/systems/RemoteLocal.hpp"

// VOIP
#include "GEngine/interface/network/systems/VoIPManager.hpp"
#include "GEngine/libdev/systems/driver/input/VoIPAudioCatcher.hpp"
#include "GEngine/libdev/systems/driver/output/VoIPAudio.hpp"

#include "GEngine/libdev/System.hpp"

struct V : gengine::System<V> {
    void init(void) override {
        publishEvent(gengine::system::event::driver::input::StartVoIP());
    }
};

void GEngineDeclareSystems(Registry *r) {
    r->registerSystem<gengine::system::driver::output::RenderWindow>(WINDOW_WIDTH, WINDOW_TOTAL_HEIGHT, "R-Type");
    r->registerSystem<gengine::system::driver::output::Draw>(BLACK);
    r->registerSystem<gengine::system::driver::output::DrawSprite>();
    r->registerSystem<gengine::system::driver::output::DrawModel>();
    r->registerSystem<gengine::system::driver::output::DrawText>();
    r->registerSystem<gengine::system::driver::output::DrawRectangle>();
    r->registerSystem<gengine::system::driver::output::DrawCircle>();

    r->registerSystem<gengine::system::driver::output::TextureManager>("../assets/sprites");
    // r->registerSystem<gengine::system::driver::output::FontManager>("../assets/fonts");
    r->registerSystem<gengine::system::driver::input::KeyboardCatcher>();
    r->registerSystem<gengine::system::driver::input::MouseCatcher>();
    r->registerSystem<gengine::system::driver::output::ModelManager>("../assets/models");
    r->registerSystem<geg::system::io::AnimationManager>("../assets/animations");
    r->registerSystem<gengine::system::driver::output::Animate>();
    r->registerSystem<gengine::system::driver::output::AnimateModel>();

    r->registerSystem<gengine::system::Motion2D>();
    r->registerSystem<gengine::system::Motion3D>();
    r->registerSystem<gengine::system::MotionAcceleration3D>();
    r->registerSystem<gengine::system::Collision2D>();
    r->registerSystem<gengine::system::AutoKiller>();

    r->registerSystem<poc3d::system::Crossair>();

    r->registerSystem<poc3d::system::InputsToGameEvents>();
    r->registerSystem<poc3d::system::Start>();
    r->registerSystem<poc3d::system::BackgroundMotion>();
    r->registerSystem<poc3d::system::PlayerMotion>();
    r->registerSystem<poc3d::system::CameraRotation>();
    r->registerSystem<poc3d::system::ResetPlayerRotation>();
    r->registerSystem<poc3d::system::ChangePlayerModel>();
    r->registerSystem<poc3d::system::Guess>();
    r->registerSystem<poc3d::system::MorphToProp>();

    r->registerSystem<gengine::interface::system::HandleRemoteLocal>();
    r->registerSystem<gengine::interface::system::HandleLocal>();

    r->registerSystem<gengine::interface::network::system::ClientEventPublisher<
        poc3d::event::Movement, poc3d::event::Rotation, poc3d::event::Jump, poc3d::event::ChangeCameraMode,
        poc3d::event::ChangePlayerModelEvent, poc3d::event::ResetPlayerRotationEvent, poc3d::event::LockPlayerEvent,
        poc3d::event::Sprint, gengine::interface::event::GetRemoteLocalWhoIAm>>();
    r->registerSystem<gengine::interface::network::system::ServerEventReceiver<
        poc3d::event::Movement, poc3d::event::Rotation, poc3d::event::Jump, poc3d::event::ChangeCameraMode,
        poc3d::event::ChangePlayerModelEvent, poc3d::event::ResetPlayerRotationEvent, poc3d::event::LockPlayerEvent,
        poc3d::event::Sprint, gengine::interface::event::GetRemoteLocalWhoIAm>>();

    r->registerSystem<gengine::interface::network::system::ConnectAtStart>("127.0.0.1", 4242);

    r->registerSystem<V>();

    // VOIP
    r->registerSystem<gengine::interface::network::system::VoIPManager>(7.f);
    r->registerSystem<gengine::system::driver::output::VoIPAudio>();
    r->registerSystem<gengine::system::driver::input::VoIPAudioCatcher>();
    // TODO auto register ↓
}
