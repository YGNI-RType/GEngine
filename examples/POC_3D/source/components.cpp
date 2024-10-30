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
#include "components/Player.hpp"

#include "GEngine/interface/components/RemoteLocal.hpp"

void GEngineDeclareComponents(Registry *r) {
    r->registerComponent<geg::component::HitBoxSquare2D>();
    r->registerComponent<geg::component::HitBoxCircle2D>();
    r->registerComponent<geg::component::SpanLife>();
    r->registerComponent<geg::component::io::Animation>();
    r->registerComponent<geg::component::Velocity2D>();
    r->registerComponent<geg::component::Velocity3D>();
    r->registerComponent<geg::component::Transform2D>();
    r->registerComponent<geg::component::Transform3D>();
    r->registerComponent<geg::component::Acceleration3D>();
    r->registerComponent<geg::component::io::Drawable>();
    r->registerComponent<geg::component::io::Sprite>();
    r->registerComponent<geg::component::io::Model>();
    r->registerComponent<geg::component::io::Text>();
    r->registerComponent<geg::component::io::Rectangle>();

    r->registerComponent<rtype::component::Player>();
    r->registerComponent<rtype::component::Background>();

    r->registerComponent<gengine::interface::component::RemoteLocal>();
    r->registerComponent<geg::component::network::NetSend>();
}
