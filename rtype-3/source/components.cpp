/*
** EPITECH PROJECT, 2024
** r->
** File description:
** game.cpp
*/

#include "GEngine/libdev/Components.hpp"
#include "GEngine/GEngine.hpp"
#include "GEngine/libdev/Systems.hpp"

#include "components/Background.hpp"
#include "components/Boss.hpp"
#include "components/Bounding.hpp"
#include "components/Bullet.hpp"
#include "components/BulletEnemy.hpp"
#include "components/Flapping.hpp"
#include "components/Invincible.hpp"
#include "components/Life.hpp"
#include "components/Monster.hpp"
#include "components/Player.hpp"
#include "components/Score.hpp"
#include "components/ScoreText.hpp"

#include "GEngine/interface/components/RemoteLocal.hpp"

void GEngineDeclareComponents(Registry *r) {
    r->registerComponent<geg::component::HitBoxSquare2D>();
    r->registerComponent<geg::component::HitBoxCircle2D>();
    r->registerComponent<geg::component::SpanLife>();
    r->registerComponent<geg::component::io::Animation>();
    r->registerComponent<geg::component::Velocity2D>();
    r->registerComponent<geg::component::Transform2D>();
    r->registerComponent<geg::component::Box<1>>();
    r->registerComponent<geg::component::io::Drawable>();
    r->registerComponent<geg::component::io::Sprite>();
    r->registerComponent<geg::component::io::Text>();
    r->registerComponent<geg::component::io::Rectangle>();

    r->registerComponent<gengine::component::gui::SceneMember>();
    r->registerComponent<gengine::component::gui::Button>();
    r->registerComponent<gengine::component::gui::ToggleButton>();
    r->registerComponent<gengine::component::gui::SelectButton>();
    r->registerComponent<gengine::component::gui::InputBox>();
    r->registerComponent<gengine::component::gui::ButtonSpriteTint>();
    r->registerComponent<gengine::component::gui::onClick>();

    r->registerComponent<rtype::component::Player>();
    r->registerComponent<rtype::component::Monster>();
    r->registerComponent<rtype::component::Background>();
    r->registerComponent<rtype::component::Bullet>();
    r->registerComponent<rtype::component::BulletEnemy>();
    r->registerComponent<rtype::component::Flapping>();
    r->registerComponent<rtype::component::Bounding>();
    r->registerComponent<rtype::component::Score>();
    r->registerComponent<rtype::component::ScoreText>();
    r->registerComponent<rtype::component::Life>();
    r->registerComponent<rtype::component::Invincible>();
    r->registerComponent<rtype::component::Boss>();

    r->registerComponent<gengine::component::driver::output::Music>();

    r->registerComponent<gengine::component::driver::output::Sound>();

    r->registerComponent<gengine::interface::component::RemoteLocal>();
    r->registerComponent<geg::component::network::NetSend>();
}
