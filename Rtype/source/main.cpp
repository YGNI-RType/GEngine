// /*
// ** EPITECH PROJECT, 2024
// ** B-CPP-500-LYN-5-1-rtype-basile.fouquet
// ** File description:
// ** main.cpp
// */

// // TODO reorder include

//? ## Engine ##
#include "GEngine/driver/Engine.hpp"
#include "GEngine/game/Engine.hpp"
#include "GEngine/interface/Internal.hpp"

//? ## GEngine Components ##
#include "GEngine/libdev/components/HitBoxs.hpp"
#include "GEngine/libdev/components/Velocities.hpp"
#include "GEngine/libdev/components/driver/output/Animation.hpp"
#include "GEngine/libdev/components/driver/output/Shape.hpp"

//? ## GEngine Systems ##
#include "GEngine/libdev/systems/Collisions.hpp"
#include "GEngine/libdev/systems/MainLoop.hpp"
#include "GEngine/libdev/systems/Motions.hpp"
#include "GEngine/libdev/systems/driver/input/KeyboardCatcher.hpp"
#include "GEngine/libdev/systems/driver/output/Animate.hpp"
#include "GEngine/libdev/systems/driver/output/Draw.hpp"
#include "GEngine/libdev/systems/driver/output/RenderWindow.hpp"
#include "GEngine/libdev/systems/driver/output/TextureManager.hpp"

//? ### R-Type Components ###

#include "components/Background.hpp"
#include "components/Bullet.hpp"
#include "components/Monster.hpp"
#include "components/Player.hpp"
#include "components/PlayerControl.hpp"

//? ### R-Type Systems ###
#include "systems/BackgroundMotion.hpp"
#include "systems/ClearBullets.hpp"
#include "systems/DestroyOnCollision.hpp"
#include "systems/InputsToGameEvents.hpp"
#include "systems/MonstersAutoMotion.hpp"
#include "systems/PlayerAnimation.hpp"
#include "systems/PlayerMotion.hpp"
#include "systems/PlayerShoot.hpp"
#include "systems/Start.hpp"

#include "GEngine/libdev/systems/driver/output/AudioDriver.hpp"
#include "GEngine/libdev/systems/MusicManager.hpp"
#include "GEngine/libdev/components/Music.hpp"

//? ### R-Type Events ###
#include "events/Movement.hpp"
#include "events/Shoot.hpp"

namespace rtype {
void registerComponents(gengine::game::Engine &gameEngine, gengine::driver::Engine &driverEngine) {
    gameEngine.registerComponent<gengine::component::Transform2D>();
    gameEngine.registerComponent<gengine::component::Velocity2D>();
    gameEngine.registerComponent<gengine::component::driver::output::Animation>();
    gameEngine.registerComponent<gengine::component::driver::output::Drawable>();
    gameEngine.registerComponent<gengine::component::driver::output::Sprite>();
    gameEngine.registerComponent<gengine::component::driver::output::Rectangle>();
    gameEngine.registerComponent<gengine::component::HitBoxSquare2D>();
    gameEngine.registerComponent<gengine::component::HitBoxCircle2D>();

    gameEngine.registerComponent<component::Player>();
    gameEngine.registerComponent<component::PlayerControl>();
    gameEngine.registerComponent<component::Monster>();
    gameEngine.registerComponent<component::Background>();
    gameEngine.registerComponent<component::Bullet>();
    gameEngine.registerComponent<gengine::component::Music>();

    driverEngine.registerComponent<gengine::component::Transform2D>();
    driverEngine.registerComponent<gengine::component::Velocity2D>();
    driverEngine.registerComponent<gengine::component::driver::output::Animation>();
    driverEngine.registerComponent<gengine::component::driver::output::Drawable>();
    driverEngine.registerComponent<gengine::component::driver::output::Sprite>();
    driverEngine.registerComponent<gengine::component::driver::output::Rectangle>();
    driverEngine.registerComponent<gengine::component::HitBoxSquare2D>();
    driverEngine.registerComponent<gengine::component::HitBoxCircle2D>();

    driverEngine.registerComponent<component::Player>();
    driverEngine.registerComponent<component::PlayerControl>();
    driverEngine.registerComponent<component::Monster>();
    driverEngine.registerComponent<component::Background>();
    driverEngine.registerComponent<component::Bullet>();
    driverEngine.registerComponent<gengine::component::Music>();
}

void registerSystems(gengine::game::Engine &gameEngine, gengine::driver::Engine &driverEngine) {
    driverEngine.registerSystem<gengine::system::driver::output::RenderWindow>(1280, 720, "R-Type");
    driverEngine.registerSystem<gengine::system::driver::output::Draw2D>();
    driverEngine.registerSystem<gengine::system::driver::output::DrawSprite>();
    driverEngine.registerSystem<gengine::system::driver::output::TextureManager>("../assets/sprites");
    gameEngine.registerSystem<gengine::system::driver::output::AnimationManager>("../assets/animations");
    gameEngine.registerSystem<gengine::system::driver::output::Animate>();
    driverEngine.registerSystem<gengine::system::driver::input::KeyboardCatcher>();
    driverEngine.registerSystem<system::InputsToGameEvents>();
    driverEngine.registerSystem<gengine::system::driver::output::AudioDriver>("../sounds", "../musics");

    gameEngine.registerSystem<gengine::system::Motion2D>();
    gameEngine.registerSystem<gengine::system::Collision2D>();
    gameEngine.registerSystem<system::Start>();
    gameEngine.registerSystem<system::MonstersAutoMotion>();
    gameEngine.registerSystem<system::PlayerMotion>();
    gameEngine.registerSystem<system::PlayerAnimation>();
    gameEngine.registerSystem<system::PlayerShoot>();
    gameEngine.registerSystem<system::BackgroundMotion>();
    gameEngine.registerSystem<system::ClearBullets>();
    gameEngine.registerSystem<system::DestroyOnCollision>();
    gameEngine.registerSystem<gengine::system::MusicManager>("../musics");
}
} // namespace rtype

#include "GEngine/interface/events/RemoteEvent.hpp"
#include "GEngine/interface/network/Networked.hpp"
#include "GEngine/interface/network/systems/ClientEventPublisher.hpp"
#include "GEngine/interface/network/systems/ServerEventReceiver.hpp"
#include "GEngine/libdev/systems/events/game/Music.hpp"

struct Test
    : public gengine::OnEventSystem<Test, gengine::interface::network::event::RemoteEvent<rtype::event::Movement>> {
    void onEvent(gengine::interface::network::event::RemoteEvent<rtype::event::Movement> &e) {
        // std::cout << "go: " << e->state << std::endl;
    }
};

struct TestDriver : public gengine::OnEventSystem<TestDriver, rtype::event::Movement> {
    void onEvent(rtype::event::Movement &e) {
        publishEvent(gengine::system::event::game::PlayMusic("cto.mp3"));
    }
};

struct TMusic: public gengine::System<TMusic> {
    void init(void) override {
        subscribeToEvent<gengine::system::event::MainLoop>(&TMusic::onS);
        subscribeToEvent<gengine::interface::network::event::RemoteEvent<gengine::system::event::driver::input::Key_A>>(&TMusic::onA);
        subscribeToEvent<gengine::interface::network::event::RemoteEvent<gengine::system::event::driver::input::Key_B>>(&TMusic::onB);
    }

    void onS(gengine::system::event::MainLoop &e) {
        publishEvent(gengine::system::event::game::PlayMusic("cto.mp3"));
    }

    void onA(gengine::interface::network::event::RemoteEvent<gengine::system::event::driver::input::Key_A> &e) {
        publishEvent(gengine::system::event::game::PlayMusic("cto.mp3"));
    }

    void onB(gengine::interface::network::event::RemoteEvent<gengine::system::event::driver::input::Key_B> &e) {
        publishEvent(gengine::system::event::game::StopMusic());
    }
};

int main(int argc, char **argv) {
    std::string ip;
    if (argc > 1)
        ip = std::string(argv[1]);
    else
        ip = "127.0.0.1";

    gengine::driver::Engine driverEngine;
    gengine::game::Engine gameEngine;

    driverEngine.registerSystem<
        gengine::interface::network::system::ClientEventPublisher<
            rtype::event::Movement,
            rtype::event::Shoot,
            gengine::system::event::driver::input::Key_A,
            gengine::system::event::driver::input::Key_B
    >>();

    gameEngine.registerSystem<
        gengine::interface::network::system::ServerEventReceiver<
            rtype::event::Movement,
            rtype::event::Shoot,
            gengine::system::event::driver::input::Key_A,
            gengine::system::event::driver::input::Key_B
    >>();

    rtype::registerComponents(gameEngine, driverEngine);
    rtype::registerSystems(gameEngine, driverEngine);

    gameEngine.registerSystem<Test>();
    driverEngine.registerSystem<TestDriver>();
    gameEngine.registerSystem<TMusic>();
    gengine::interface::network::Networked interface(driverEngine, gameEngine, ip, 4243, true);

    interface.run();
}
