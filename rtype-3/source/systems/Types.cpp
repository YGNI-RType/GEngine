/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Types
*/

#include "Types.hpp"

namespace rtype {
void to_json(nlohmann::json &j, const Background &b) {
    j = nlohmann::json{{"sprite", b.sprite}, {"transform", b.transform}, {"velocity", b.velocity}};
}

void from_json(const nlohmann::json &j, Background &b) {
    j.at("sprite").get_to(b.sprite);
    j.contains("transform") ? j.at("transform").get_to(b.transform) : b.transform = geg::component::Transform2D();
    j.contains("velocity") ? j.at("velocity").get_to(b.velocity) : b.velocity = geg::component::Velocity2D();
}

void to_json(nlohmann::json &j, const Bullet &b) {
    j = nlohmann::json{{"sprite", b.sprite},
                       {"animation", b.animation},
                       {"transform", b.transform},
                       {"velocity", b.velocity},
                       {"hitbox", b.hitbox},
                       {"followPlayer", b.followPlayer},
                       {"isDestroyable", b.isDestroyable}};
}

void from_json(const nlohmann::json &j, Bullet &b) {
    j.at("sprite").get_to(b.sprite);
    j.contains("animation") ? j.at("animation").get_to(b.animation) : b.animation = geg::component::io::Animation();
    j.contains("transform") ? j.at("transform").get_to(b.transform) : b.transform = geg::component::Transform2D();
    j.contains("velocity") ? j.at("velocity").get_to(b.velocity) : b.velocity = geg::component::Velocity2D();
    j.contains("hitbox") ? j.at("hitbox").get_to(b.hitbox)
                         : b.hitbox = geg::component::HitBoxSquare2D(b.sprite.src.width, b.sprite.src.height);
    j.contains("followPlayer") ? j.at("followPlayer").get_to(b.followPlayer) : b.followPlayer = false;
    j.contains("isDestroyable") ? j.at("isDestroyable").get_to(b.isDestroyable) : b.isDestroyable = false;
}

void to_json(nlohmann::json &j, const Ammo &a) {
    j = nlohmann::json{{"bulletName", a.bulletName},
                       {"spawnDelay", a.spawnDelay},
                       {"scaleFactor", a.scaleFactor},
                       {"speedFactor", a.speedFactor}};
}

void from_json(const nlohmann::json &j, Ammo &a) {
    j.at("bulletName").get_to(a.bulletName);
    j.at("spawnDelay").get_to(a.spawnDelay);
    j.contains("scaleFactor") ? j.at("scaleFactor").get_to(a.scaleFactor) : a.scaleFactor = 1;
    j.contains("speedFactor") ? j.at("speedFactor").get_to(a.speedFactor) : a.speedFactor = 1;
}

void to_json(nlohmann::json &j, const Mob &m) {
    j = nlohmann::json{{"sprite", m.sprite},     {"animation", m.animation}, {"transform", m.transform},
                       {"velocity", m.velocity}, {"hitbox", m.hitbox},       {"typeOfMotion", m.typeOfMotion},
                       {"ammo", m.ammo}};
}

void from_json(const nlohmann::json &j, Mob &m) {
    j.at("sprite").get_to(m.sprite);
    j.contains("animation") ? j.at("animation").get_to(m.animation) : m.animation = geg::component::io::Animation();
    j.contains("transform") ? j.at("transform").get_to(m.transform) : m.transform = geg::component::Transform2D();
    j.contains("velocity") ? j.at("velocity").get_to(m.velocity) : m.velocity = geg::component::Velocity2D();
    j.contains("hitbox") ? j.at("hitbox").get_to(m.hitbox)
                         : m.hitbox = geg::component::HitBoxSquare2D(m.sprite.src.width, m.sprite.src.height);
    j.contains("typeOfMotion") ? j.at("typeOfMotion").get_to(m.typeOfMotion) : m.typeOfMotion = TypeOfMotion::LINEAR;
    j.contains("ammo") ? j.at("ammo").get_to(m.ammo) : m.ammo = std::vector<Ammo>();
}

void to_json(nlohmann::json &j, const Boss &b) {
    j = nlohmann::json{{"sprite", b.sprite},
                       {"animation", b.animation},
                       {"transform", b.transform},
                       {"velocity", b.velocity},
                       {"hitbox", b.hitbox},
                       {"wavesName", b.wavesName},
                       {"waveCooldown", b.waveCooldown},
                       {"ammoName", b.ammoName},
                       {"bulletSpeed", b.bulletSpeed},
                       {"minVelocity", b.minVelocity},
                       {"maxVelocity", b.maxVelocity},
                       {"borderMargin", b.borderMargin}};
}

void from_json(const nlohmann::json &j, Boss &b) {
    j.at("sprite").get_to(b.sprite);
    j.contains("animation") ? j.at("animation").get_to(b.animation) : b.animation = geg::component::io::Animation();
    j.contains("transform") ? j.at("transform").get_to(b.transform) : b.transform = geg::component::Transform2D();
    j.contains("velocity") ? j.at("velocity").get_to(b.velocity) : b.velocity = geg::component::Velocity2D();
    j.contains("hitbox") ? j.at("hitbox").get_to(b.hitbox)
                         : b.hitbox = geg::component::HitBoxSquare2D(b.sprite.src.width, b.sprite.src.height);
    j.contains("wavesName") ? j.at("wavesName").get_to(b.wavesName) : b.wavesName = std::vector<std::string>();
    j.contains("waveCooldown") ? j.at("waveCooldown").get_to(b.waveCooldown) : b.waveCooldown = 10000;
    j.contains("ammoName") ? j.at("ammoName").get_to(b.ammoName) : b.ammoName = std::vector<std::string>();
    j.contains("bulletSpeed") ? j.at("bulletSpeed").get_to(b.bulletSpeed) : b.bulletSpeed = 2;
    j.contains("minVelocity") ? j.at("minVelocity").get_to(b.minVelocity) : b.minVelocity = 2;
    j.contains("maxVelocity") ? j.at("maxVelocity").get_to(b.maxVelocity) : b.maxVelocity = 6;
    j.contains("borderMargin") ? j.at("borderMargin").get_to(b.borderMargin) : b.borderMargin = 0.2;
}

void to_json(nlohmann::json &j, const Monster &m) {
    j = nlohmann::json{{"mobName", m.mobName},
                       {"spawnFactor", m.spawnFactor},
                       {"spawnDelay", m.spawnDelay},
                       {"scaleFactor", m.scaleFactor},
                       {"speedFactor", m.speedFactor},
                       {"scoreGain", m.scoreGain},
                       {"numberOfLifes", m.numberOfLifes}};
}

void from_json(const nlohmann::json &j, Monster &m) {
    j.at("mobName").get_to(m.mobName);
    j.at("spawnFactor").get_to(m.spawnFactor);
    j.at("spawnDelay").get_to(m.spawnDelay);
    j.contains("scaleFactor") ? j.at("scaleFactor").get_to(m.scaleFactor) : m.scaleFactor = 1;
    j.contains("speedFactor") ? j.at("speedFactor").get_to(m.speedFactor) : m.speedFactor = 1;
    j.contains("scoreGain") ? j.at("scoreGain").get_to(m.scoreGain) : m.scoreGain = 0;
    j.contains("numberOfLifes") ? j.at("numberOfLifes").get_to(m.numberOfLifes) : m.numberOfLifes = 1;
}

void to_json(nlohmann::json &j, const Stage &s) {
    j = nlohmann::json{{"background", s.background}, {"monsters", s.monsters}, {"boss", s.boss}};
}

void from_json(const nlohmann::json &j, Stage &s) {
    j.at("background").get_to(s.background);
    j.contains("monsters") ? j.at("monsters").get_to(s.monsters) : s.monsters = std::vector<Monster>();
    j.at("boss").get_to(s.boss);
}
} // namespace rtype

namespace gengine {
void to_json(nlohmann::json &j, const Vect2 &v) {
    j = nlohmann::json{{"x", v.x}, {"y", v.y}};
}

void from_json(const nlohmann::json &j, Vect2 &v) {
    j.contains("x") ? j.at("x").get_to(v.x) : v.x = 0;
    j.contains("y") ? j.at("y").get_to(v.y) : v.y = 0;
}
} // namespace gengine

namespace gengine::component {
void to_json(nlohmann::json &j, const HitBoxSquare2D &h) {
    j = nlohmann::json{{"width", h.width}, {"height", h.height}};
}

void from_json(const nlohmann::json &j, HitBoxSquare2D &h) {
    j.at("width").get_to(h.width);
    j.at("height").get_to(h.height);
}

void to_json(nlohmann::json &j, const Transform2D &t) {
    j = nlohmann::json{{"position", t.pos}, {"scale", t.scale}, {"rotation", t.rotation}};
}

void from_json(const nlohmann::json &j, Transform2D &t) {
    j.contains("position") ? j.at("position").get_to(t.pos) : t.pos = {0, 0};
    j.contains("scale") ? j.at("scale").get_to(t.scale) : t.scale = {1, 1};
    j.contains("rotation") ? j.at("rotation").get_to(t.rotation) : t.rotation = 0;
}

void to_json(nlohmann::json &j, const Velocity2D &v) {
    j = nlohmann::json{{"x", v.x}, {"y", v.y}};
}

void from_json(const nlohmann::json &j, Velocity2D &v) {
    j.contains("x") ? j.at("x").get_to(v.x) : v.x = 0;
    j.contains("y") ? j.at("y").get_to(v.y) : v.y = 0;
}
} // namespace gengine::component

namespace gengine::component::driver::output {
void to_json(nlohmann::json &j, const Rect &r) {
    j = nlohmann::json{{"x", r.x}, {"y", r.y}, {"width", r.width}, {"height", r.height}};
}

void from_json(const nlohmann::json &j, Rect &r) {
    j.contains("x") ? j.at("x").get_to(r.x) : r.x = 0;
    j.contains("y") ? j.at("y").get_to(r.y) : r.y = 0;
    j.at("width").get_to(r.width);
    j.at("height").get_to(r.height);
}

void to_json(nlohmann::json &j, const Clr &c) {
    j = nlohmann::json{{"r", c.r}, {"g", c.g}, {"b", c.b}, {"a", c.a}};
}

void from_json(const nlohmann::json &j, Clr &c) {
    j.contains("r") ? j.at("r").get_to(c.r) : c.r = 255;
    j.contains("g") ? j.at("g").get_to(c.g) : c.g = 255;
    j.contains("b") ? j.at("b").get_to(c.b) : c.b = 255;
    j.contains("a") ? j.at("a").get_to(c.a) : c.a = 255;
}

void to_json(nlohmann::json &j, const Sprite &s) {
    j = nlohmann::json{{"txtPath", s.txtPath}, {"src", s.src}, {"tint", s.tint}};
}

void from_json(const nlohmann::json &j, Sprite &s) {
    j.at("txtPath").get_to(s.txtPath);
    j.at("src").get_to(s.src);
    j.contains("tint") ? j.at("tint").get_to(s.tint) : s.tint = WHITE;
}

void to_json(nlohmann::json &j, const Animation &a) {
    j = nlohmann::json{{"trackName", a.trackName},
                       {"frameDuration", a.frameDuration},
                       {"mode", a.m_mode},
                       {"startingFrame", a.currentFrameIndex}};
}

void from_json(const nlohmann::json &j, Animation &a) {
    j.at("trackName").get_to(a.trackName);
    j.at("frameDuration").get_to(a.frameDuration);
    j.contains("mode") ? j.at("mode").get_to(a.m_mode) : a.m_mode = AnimationTrack::PlaybackMode::Forward;
    j.contains("startingFrame") ? j.at("startingFrame").get_to(a.currentFrameIndex) : a.currentFrameIndex = 0;
}
} // namespace gengine::component::driver::output
