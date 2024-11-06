/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Types
*/

#pragma once

#include "GEngine/libdev/Components.hpp"
#include "GEngine/libdev/Entity.hpp"

#include <nlohmann/json.hpp>

namespace rtype {
struct Background {
    geg::component::io::Sprite sprite;
    geg::component::Transform2D transform;
    geg::component::Velocity2D velocity;
};

enum TypeOfMotion { FLAPPING, BOUNDING, LINEAR };

struct Bullet {
    geg::component::io::Sprite sprite;
    geg::component::io::Animation animation;
    geg::component::Transform2D transform;
    geg::component::Velocity2D velocity;
    geg::component::HitBoxSquare2D hitbox;
    bool followPlayer;
    bool isDestroyable;
};

struct Ammo {
    std::string bulletName;
    unsigned int spawnDelay;
    float scaleFactor;
    float speedFactor;
    gengine::Entity mobId;
};

struct Mob {
    geg::component::io::Sprite sprite;
    geg::component::io::Animation animation;
    geg::component::Transform2D transform;
    geg::component::Velocity2D velocity;
    geg::component::HitBoxSquare2D hitbox;
    TypeOfMotion typeOfMotion;
    std::vector<Ammo> ammo;
};

struct Boss {
    geg::component::io::Sprite sprite;
    geg::component::io::Animation animation;
    geg::component::Transform2D transform;
    geg::component::Velocity2D velocity;
    geg::component::HitBoxSquare2D hitbox;
    std::vector<std::string> wavesName;
    int waveCooldown;
    std::vector<std::string> ammoName;
    int bulletSpeed;
    int minVelocity;
    int maxVelocity;
    float borderMargin;
};

struct Monster {
    std::string mobName;
    gengine::Vect2 spawnFactor;
    unsigned int spawnDelay;
    float scaleFactor;
    float speedFactor;
    int scoreGain;
    unsigned int numberOfLifes;
};

struct Stage {
    Background background;
    std::vector<Monster> monsters;
    Monster boss;
    std::vector<Ammo> ammo;
};

void to_json(nlohmann::json &j, const Background &b);
void from_json(const nlohmann::json &j, Background &b);

void to_json(nlohmann::json &j, const Bullet &b);
void from_json(const nlohmann::json &j, Bullet &b);

void to_json(nlohmann::json &j, const Ammo &a);
void from_json(const nlohmann::json &j, Ammo &a);

void to_json(nlohmann::json &j, const Mob &m);
void from_json(const nlohmann::json &j, Mob &m);

void to_json(nlohmann::json &j, const Boss &b);
void from_json(const nlohmann::json &j, Boss &b);

void to_json(nlohmann::json &j, const Monster &m);
void from_json(const nlohmann::json &j, Monster &m);

void to_json(nlohmann::json &j, const Stage &s);
void from_json(const nlohmann::json &j, Stage &s);
} // namespace rtype

namespace Network {
template <size_t Size>
void to_json(nlohmann::json &j, const NetString<Size> &s) {
    std::string str = s.c_str();
    j = nlohmann::json{str};
}

template <size_t Size>
void from_json(const nlohmann::json &j, NetString<Size> &s) {
    std::string str;
    j.get_to(str);
    s = str;
}
} // namespace Network

namespace gengine {
void to_json(nlohmann::json &j, const Vect2 &v);
void from_json(const nlohmann::json &j, Vect2 &v);
} // namespace gengine

namespace gengine::component {
void to_json(nlohmann::json &j, const HitBoxSquare2D &h);
void from_json(const nlohmann::json &j, HitBoxSquare2D &h);

void to_json(nlohmann::json &j, const Transform2D &t);
void from_json(const nlohmann::json &j, Transform2D &t);

void to_json(nlohmann::json &j, const Velocity2D &v);
void from_json(const nlohmann::json &j, Velocity2D &v);
} // namespace gengine::component

namespace gengine::component::driver::output {
void to_json(nlohmann::json &j, const Rect &r);
void from_json(const nlohmann::json &j, Rect &r);

void to_json(nlohmann::json &j, const Clr &c);
void from_json(const nlohmann::json &j, Clr &c);

void to_json(nlohmann::json &j, const Sprite &s);
void from_json(const nlohmann::json &j, Sprite &s);

void to_json(nlohmann::json &j, const Animation &a);
void from_json(const nlohmann::json &j, Animation &a);
} // namespace gengine::component::driver::output
