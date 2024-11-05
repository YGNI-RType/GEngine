/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Draw.cpp
*/

#include "GEngine/libdev/systems/driver/output/Draw.hpp"
#include "GEngine/libdev/tools/Raylib.hpp"
#include <iostream>
#include <raymath.h>
#include <rlgl.h>

namespace gengine::system::driver::output {
struct zIndexComparator {
    bool operator()(const std::pair<Entity, int> &lhs, const std::pair<Entity, int> &rhs) const {
        return lhs.second < rhs.second;
    }
};

Draw::Draw(const Color &clear)
    : m_clear(clear) {
}

void Draw::init(void) {
    subscribeToEvent<gengine::system::event::RenderLoop>(&Draw::onRenderLoop);
    subscribeToEvent<gengine::system::event::WindowResized>(&Draw::onWindowResized);
    subscribeToEvent<gengine::system::event::BeginDraw>(&Draw::onBeginDraw);
    subscribeToEvent<gengine::system::event::EndDraw>(&Draw::onEndDraw);
}

void Draw::onRenderLoop(gengine::system::event::RenderLoop &e) {
    publishEvent(gengine::system::event::BeginDraw(m_clear));
    auto &drawables = getComponents<component::driver::output::Drawable>();

    std::multiset<std::pair<Entity, int>, zIndexComparator> toDraw;
    for (auto &[entity, drawable] : drawables)
        toDraw.emplace(entity, drawable.zIndex);

    for (auto &[entity, zIndex] : toDraw)
        publishEvent(gengine::system::event::Draw(entity, m_ratio));
    publishEvent(gengine::system::event::EndDraw());
}

void Draw::onWindowResized(gengine::system::event::WindowResized &e) {
    m_ratio = e.ratio;
}

void Draw::onBeginDraw(gengine::system::event::BeginDraw &e) {
    // BeginDrawing();
    ClearBackground(e.clear);
}
void Draw::onEndDraw(gengine::system::event::EndDraw &e) {
    rlDrawRenderBatchActive();
    SwapScreenBuffer();
}

void DrawSprite::init(void) {
    subscribeToEvent<gengine::system::event::Draw>(&DrawSprite::onDraw);
}

void DrawSprite::onDraw(gengine::system::event::Draw &e) {
    auto &sprites = getComponents<component::driver::output::Sprite>();
    auto &transforms = getComponents<gengine::component::Transform2D>();

    if (sprites.contains(e.entity) && transforms.contains(e.entity)) {
        auto &txtMan = getSystem<TextureManager>();
        auto &[path, src, tint] = sprites.get(e.entity);
        auto &[pos, scale, rotation] = transforms.get(e.entity);
        auto &txt = txtMan.get(path.c_str());
        if (txt.width < src.width)
            src.width = txt.width;
        if (txt.height < src.height)
            src.height = txt.height;
        Rectangle r = {pos.x * e.ratio.x, pos.y * e.ratio.y, src.width * scale.x * e.ratio.x,
                       src.height * scale.y * e.ratio.y};
        DrawTexturePro(txt, src, r, {0, 0}, rotation, tint); // TODO origin
    }
}

void DrawText::init(void) {
    subscribeToEvent<gengine::system::event::Draw>(&DrawText::onDraw);
}

void DrawText::onDraw(gengine::system::event::Draw &e) {
    auto &texts = getComponents<component::driver::output::Text>();
    auto &transforms = getComponents<gengine::component::Transform2D>();

    if (texts.contains(e.entity) && transforms.contains(e.entity)) {
        auto &fontMan = getSystem<FontManager>();
        auto &[path, str, fontSize, spacing, tint] = texts.get(e.entity);
        auto &[pos, scale, rotation] = transforms.get(e.entity);
        DrawTextPro(fontMan.get(path.c_str()), str.c_str(), Vector2{pos.x * e.ratio.x, pos.y * e.ratio.y}, {0, 0},
                    rotation, fontSize * scale.y * e.ratio.y, spacing, tint); // TODO origin
    }
}

void DrawRectangle::init(void) {
    subscribeToEvent<gengine::system::event::Draw>(&DrawRectangle::onDraw);
}

void DrawRectangle::onDraw(gengine::system::event::Draw &e) {
    auto &rectangles = getComponents<component::driver::output::Rectangle>();
    auto &transforms = getComponents<gengine::component::Transform2D>();

    if (rectangles.contains(e.entity) && transforms.contains(e.entity)) {
        auto &[width, height, color] = rectangles.get(e.entity);
        auto &[pos, scale, rotation] = transforms.get(e.entity);
        Rectangle r = {pos.x * e.ratio.x, pos.y * e.ratio.y, width * scale.x * e.ratio.x, height * scale.y * e.ratio.y};
        DrawRectanglePro(r, {0, 0}, rotation, color); // TODO origin
    }
}

void DrawCircle::init(void) {
    subscribeToEvent<gengine::system::event::Draw>(&DrawCircle::onDraw);
}

void DrawCircle::onDraw(gengine::system::event::Draw &e) {
    auto &circles = getComponents<component::driver::output::Circle>();
    auto &transforms = getComponents<gengine::component::Transform2D>();

    if (circles.contains(e.entity) && transforms.contains(e.entity)) {
        auto &[r, color] = circles.get(e.entity);
        auto &[pos, scale, rotation] = transforms.get(e.entity);
        ::DrawCircle(pos.x * e.ratio.x, pos.y * e.ratio.y, r * scale.x * e.ratio.x, color);
    }
}

// 3D Rendering
void DrawModel::init(void) {
    subscribeToEvent<gengine::system::event::Draw>(&DrawModel::onDraw);
}

void DrawModel::onDraw(gengine::system::event::Draw &e) {
    auto &models = getComponents<component::driver::output::Model>();
    auto &transforms = getComponents<gengine::component::Transform3D>();

    if (models.contains(e.entity) && transforms.contains(e.entity)) {
        auto &modelMan = getSystem<ModelManager>();
        auto &[path, color, isAnimated] = models.get(e.entity);
        auto &transform = transforms.get(e.entity);
        auto &anims = getComponents<component::driver::output::Animation>();

        BeginMode3D(camera);
        std::string fullPath = path;
        if (isAnimated && anims.contains(e.entity)) {
            auto &anim = anims.get(e.entity);
            std::string animName = anim.trackName;
            size_t frameIndex = anim.currentFrameIndex;

            fullPath += animName.substr(animName.find_last_of("/") + 1) + "/";
            if (frameIndex + 1 < 10)
                fullPath += "0";
            fullPath += std::to_string(frameIndex + 1) + ".obj";
        }
        Model model = modelMan.get(fullPath.c_str());
        setModelTransform(model, transform);

        ::DrawModel(model, {0, 0, 0}, 1, color);
        BoundingBox box = TransformBoundingBox(GetMeshBoundingBox(model.meshes[0]), model.transform);

        ::DrawBoundingBox(box, RED);
        EndMode3D();
    }
}
} // namespace gengine::system::driver::output
