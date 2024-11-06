/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Constants.hpp
*/

#pragma once

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define HUD_HEIGHT 30
#define WINDOW_TOTAL_HEIGHT WINDOW_HEIGHT + HUD_HEIGHT
#define MOUSE_SENSITIVITY 0.6f

const std::vector<std::pair<std::string, float>> propsList = {
    {"beer_bottle.glb", 0.008f},
    {"plastic_water_bottle.glb", 0.01f},
    {"watermelon.glb", 0.02f},
    {"bottle_of_champagne.glb", 1.1f},
    {"shampoo_bottle.glb", 1.0f},
    {"heiniken_bottle.glb", 0.1f},
};

#define PROPS_COUNT 10
