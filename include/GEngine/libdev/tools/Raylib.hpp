/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Raylib.hpp
*/

#pragma once

#include "GEngine/libdev/Components.hpp"
#include <raylib.h>
#include <raymath.h>

namespace gengine {

void setModelTransform(Model &model, component::Transform3D &transform);
BoundingBox TransformBoundingBox(BoundingBox box, const Matrix &transform);
RayCollision GetRayCollisionModel(Ray ray, Model model, component::Transform3D &modelTransform);
void CameraPitch(Camera *camera, float angle, bool lockView, bool rotateAroundTarget, bool rotateUp);
} // namespace gengine
