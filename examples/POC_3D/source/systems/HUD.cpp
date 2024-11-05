/*
** EPITECH PROJECT, 2024
** GEngine
** File description:
** HUD.cpp
*/

#include "systems/HUD.hpp"
#include "Constants.hpp"

namespace poc3d::system {
void Crossair::onEvent(gengine::system::event::StartEngine &e) {
    DisableCursor();
    float crosshairSize = 3;
    float crosshairBorder = 0.7;
    Color whiteTransp = {255, 255, 255, 100};
    spawnEntity(geg::component::Transform2D(
                    {WINDOW_WIDTH / 2 - crosshairSize / 2, (WINDOW_TOTAL_HEIGHT) / 2 - crosshairSize / 2},
                    {crosshairSize, crosshairSize}),
                gengine::component::driver::output::Drawable(5),
                gengine::component::driver::output::Circle(crosshairSize / 2 + crosshairBorder, BLACK));
    spawnEntity(geg::component::Transform2D(
                    {WINDOW_WIDTH / 2 - crosshairSize / 2, (WINDOW_TOTAL_HEIGHT) / 2 - crosshairSize / 2},
                    {crosshairSize, crosshairSize}),
                gengine::component::driver::output::Drawable(5),
                gengine::component::driver::output::Circle(crosshairSize / 2, whiteTransp));
}

// void DebugMenu::onEvent(gengine::system::event::StartEngine &e) {
// F3 Debug Menu

// DrawText(std::string("Camera position: " + std::to_string(camera.position.x) + " " +
//                        std::to_string(camera.position.y) + " " + std::to_string(camera.position.z))
//                .c_str(),
//            10, 10, 20, WHITE);
// DrawText(std::string("Camera target: " + std::to_string(camera.target.x) + " " +
//                        std::to_string(camera.target.y) + " " + std::to_string(camera.target.z))
//                .c_str(),
//            10, 30, 20, WHITE);
// DrawText(std::string("Camera up: " + std::to_string(camera.up.x) + " " + std::to_string(camera.up.y) + " " +
//                        std::to_string(camera.up.z))
//                .c_str(),
//            10, 50, 20, WHITE);
// DrawText(std::string("Camera fovy: " + std::to_string(camera.fovy)).c_str(), 10, 70, 20, WHITE);
// DrawText(std::string("Camera projection: " + std::to_string(camera.projection)).c_str(), 10, 90, 20, WHITE);
// spawnEntity(gengine::component::driver::output::Text(""))
// );
// }
} // namespace poc3d::system