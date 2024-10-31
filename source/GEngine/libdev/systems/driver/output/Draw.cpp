/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Draw.cpp
*/

#include "GEngine/libdev/systems/driver/output/Draw.hpp"
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
        publishEvent(gengine::system::event::Draw(entity));
    publishEvent(gengine::system::event::EndDraw());
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
        Rectangle r = {pos.x, pos.y, src.width * scale.x, src.height * scale.y};
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
        DrawTextPro(fontMan.get(path.c_str()), str.c_str(), Vector2{pos.x, pos.y}, {0, 0}, rotation, fontSize * scale.y,
                    spacing, tint); // TODO origin
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
        Rectangle r = {pos.x, pos.y, width * scale.x, height * scale.y};
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
        ::DrawCircle(pos.x, pos.y, r * scale.x, color);
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
        auto &[path, color] = models.get(e.entity);
        auto &[pos, scale, rotation] = transforms.get(e.entity);
        // LoadMaterials();
        // camera.position = (Vector3){0.0f, 0.0f, 0.0f}; // Camera position
        // camera.target = (Vector3){0.0f, 0.0f, 1.0f};   // Camera looking at point
        // camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
        // camera.fovy = 60.0f;                                // Camera field-of-view Y
        // camera.projection = CAMERA_PERSPECTIVE;
        // ::Matrix

        //* Camera Movement
        // UpdateCamera(&camera, CAMERA_FIRST_PERSON); // Camera projection type
        // float moveSpeedVertical = 0.05f;
        // if (IsKeyDown(KEY_SPACE)) {
        //     camera.position.y += moveSpeedVertical;
        //     camera.target.y += moveSpeedVertical;
        // }
        // if (IsKeyDown(KEY_LEFT_SHIFT)) {
        //     camera.position.y -= moveSpeedVertical;
        //     camera.target.y -= moveSpeedVertical;
        // }

        ::DrawText(std::string("Camera position: " + std::to_string(camera.position.x) + " " +
                               std::to_string(camera.position.y) + " " + std::to_string(camera.position.z))
                       .c_str(),
                   10, 10, 20, WHITE);
        ::DrawText(std::string("Camera target: " + std::to_string(camera.target.x) + " " +
                               std::to_string(camera.target.y) + " " + std::to_string(camera.target.z))
                       .c_str(),
                   10, 30, 20, WHITE);
        ::DrawText(std::string("Camera up: " + std::to_string(camera.up.x) + " " + std::to_string(camera.up.y) + " " +
                               std::to_string(camera.up.z))
                       .c_str(),
                   10, 50, 20, WHITE);
        ::DrawText(std::string("Camera fovy: " + std::to_string(camera.fovy)).c_str(), 10, 70, 20, WHITE);
        ::DrawText(std::string("Camera projection: " + std::to_string(camera.projection)).c_str(), 10, 90, 20, WHITE);

        BeginMode3D(camera);

        Model model = modelMan.get(path.c_str());
        model.transform = MatrixIdentity();
        model.transform = MatrixMultiply(model.transform, MatrixScale(scale.x, scale.y, scale.z));
        model.transform = MatrixMultiply(
            model.transform, MatrixRotateXYZ({rotation.x * DEG2RAD, rotation.y * DEG2RAD, rotation.z * DEG2RAD}));
        model.transform = MatrixMultiply(model.transform, MatrixTranslate(pos.x, pos.y, pos.z));

        ::DrawModel(model, {0, 0, 0}, 1, color);
        EndMode3D();
    }
}
} // namespace gengine::system::driver::output
