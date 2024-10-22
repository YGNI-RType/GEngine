/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** shared.cpp
*/

#include "GEngine/GEngine.hpp"
#include "GEngine/libdev/Components.hpp"

#include "systems/BackgroundMotion.hpp"
#include "systems/CaterpillarsBound.hpp"
#include "systems/CaterpillarsWave.hpp"
#include "systems/ClearEntities.hpp"
#include "systems/DestroyOnCollision.hpp"
#include "systems/InputsToGameEvents.hpp"
#include "systems/PlanesAutoMotion.hpp"
#include "systems/PlanesWave.hpp"
#include "systems/PlayerAnimation.hpp"
#include "systems/PlayerMotion.hpp"
#include "systems/PlayerShoot.hpp"
#include "systems/Start.hpp"
#include "systems/UpdateScoreText.hpp"

#include "components/Player.hpp"
#include "components/Monster.hpp"

void GEngineDeclareShared(Registry *r) {
    r->registerComponent<geg::component::HitBoxSquare2D>();
    r->registerComponent<geg::component::HitBoxCircle2D>();
    r->registerComponent<geg::component::SpanLife>();
    r->registerComponent<geg::component::io::Animation>();
    r->registerComponent<geg::component::Velocity2D>();
    r->registerComponent<geg::component::Transform2D>();
    r->registerComponent<geg::component::io::Drawable>();
    r->registerComponent<geg::component::io::Sprite>();
    r->registerComponent<geg::component::io::Text>();
    r->registerComponent<geg::component::io::Rectangle>();

    r->registerComponent<rtype::component::Player>();
    r->registerComponent<rtype::component::Monster>();
    r->registerComponent<rtype::component::Background>();
    r->registerComponent<rtype::component::Bullet>();
    r->registerComponent<rtype::component::Plane>();
    r->registerComponent<rtype::component::Caterpillar>();
    r->registerComponent<rtype::component::Score>();
    r->registerComponent<rtype::component::ScoreText>();
    r->registerComponent<rtype::component::Life>();

    r->registerSystem<rtype::system::Start>();
    r->registerSystem<rtype::system::CaterpillarsBound>();
    r->registerSystem<rtype::system::CaterpillarsWave>();
    r->registerSystem<rtype::system::PlanesAutoMotion>();
    r->registerSystem<rtype::system::PlanesWave>();
    r->registerSystem<rtype::system::PlayerMotion>();
    r->registerSystem<rtype::system::PlayerAnimation>();
    r->registerSystem<rtype::system::PlayerShoot>();
    r->registerSystem<rtype::system::BackgroundMotion>();
    r->registerSystem<rtype::system::ClearEntities>();
    r->registerSystem<rtype::system::DestroyOnCollision>();
    r->registerSystem<rtype::system::UpdateScoreText>();

    r->registerSystem<gengine::system::driver::output::AnimationManager>("../assets/animations");
    r->registerSystem<gengine::system::driver::output::Animate>();
    r->registerSystem<gengine::system::Motion2D>();
    r->registerSystem<gengine::system::Collision2D>();
    r->registerSystem<gengine::system::AutoKiller>();
}