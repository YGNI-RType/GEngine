/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Raylib.cpp
*/

#include "GEngine/libdev/tools/Raylib.hpp"

namespace gengine {

void setModelTransform(Model &model, component::Transform3D &transform) {
    model.transform = MatrixIdentity();
    model.transform =
        MatrixMultiply(model.transform, MatrixScale(transform.scale.x, transform.scale.y, transform.scale.z));
    model.transform =
        MatrixMultiply(model.transform, MatrixRotateXYZ({transform.rotation.x * DEG2RAD, transform.rotation.y * DEG2RAD,
                                                         transform.rotation.z * DEG2RAD}));
    model.transform =
        MatrixMultiply(model.transform, MatrixTranslate(transform.pos.x, transform.pos.y, transform.pos.z));
}

BoundingBox TransformBoundingBox(BoundingBox box, const Matrix &transform) {
    // Get all 8 corners of the bounding box
    Vector3 corners[8] = {
        {box.min.x, box.min.y, box.min.z}, {box.max.x, box.min.y, box.min.z}, {box.min.x, box.max.y, box.min.z},
        {box.max.x, box.max.y, box.min.z}, {box.min.x, box.min.y, box.max.z}, {box.max.x, box.min.y, box.max.z},
        {box.min.x, box.max.y, box.max.z}, {box.max.x, box.max.y, box.max.z},
    };

    // Transform all corners
    for (int i = 0; i < 8; i++)
        corners[i] = Vector3Transform(corners[i], transform);

    // Find new min/max bounds
    BoundingBox transformedBox = {corners[0], corners[0]};
    for (int i = 1; i < 8; i++) {
        transformedBox.min.x = fmin(transformedBox.min.x, corners[i].x);
        transformedBox.min.y = fmin(transformedBox.min.y, corners[i].y);
        transformedBox.min.z = fmin(transformedBox.min.z, corners[i].z);
        transformedBox.max.x = fmax(transformedBox.max.x, corners[i].x);
        transformedBox.max.y = fmax(transformedBox.max.y, corners[i].y);
        transformedBox.max.z = fmax(transformedBox.max.z, corners[i].z);
    }

    return transformedBox;
}

RayCollision GetMouseRayCollisionModel(Model model, component::Transform3D &modelTransform, Camera &camera) {
    setModelTransform(model, modelTransform);

    // BoundingBox box = GetMeshBoundingBox(model.meshes[0]);
    // box.min.x += modelTransform.pos.x;
    // box.min.y += modelTransform.pos.y;
    // box.min.z += modelTransform.pos.z;
    // box.max.x += modelTransform.pos.x;
    // box.max.y += modelTransform.pos.y;
    // box.max.z += modelTransform.pos.z;
    BoundingBox box = GetMeshBoundingBox(model.meshes[0]);
    BoundingBox transformedBox = TransformBoundingBox(box, model.transform);

    Ray ray = GetMouseRay(GetMousePosition(), camera);
    RayCollision collision = GetRayCollisionBox(ray, transformedBox);
    if (collision.hit) {
        RayCollision meshHitInfo = {0};
        for (int m = 0; m < model.meshCount; m++) {
            meshHitInfo = GetRayCollisionMesh(ray, model.meshes[m], model.transform);
            if (meshHitInfo.hit) {
                // if ((!collision.hit) || (collision.distance > meshHitInfo.distance))
                //     collision = meshHitInfo;

                break;
            }
        }

        return meshHitInfo;
    }
    return collision;
}
void CameraPitch(Camera *camera, float angle, bool lockView, bool rotateAroundTarget, bool rotateUp) {
    // Up direction
    Vector3 targetPosition = Vector3Subtract(camera->target, camera->position);
    Vector3 up = Vector3Normalize(camera->up);
    Vector3 forward = Vector3Normalize(targetPosition);
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, up));

    if (lockView) {
        // In these camera modes we clamp the Pitch angle
        // to allow only viewing straight up or down.

        // Clamp view up
        float maxAngleUp = Vector3Angle(up, targetPosition);
        maxAngleUp -= 0.001f; // avoid numerical errors
        if (angle > maxAngleUp)
            angle = maxAngleUp;

        // Clamp view down
        float maxAngleDown = Vector3Angle(Vector3Negate(up), targetPosition);
        maxAngleDown *= -1.0f;  // downwards angle is negative
        maxAngleDown += 0.001f; // avoid numerical errors
        if (angle < maxAngleDown)
            angle = maxAngleDown;
    }

    // Rotate view vector around right axis
    targetPosition = Vector3RotateByAxisAngle(targetPosition, right, angle);

    if (rotateAroundTarget) {
        // Move position relative to target
        camera->position = Vector3Subtract(camera->target, targetPosition);
    } else // rotate around camera.position
    {
        // Move target relative to position
        camera->target = Vector3Add(camera->position, targetPosition);
    }

    if (rotateUp) {
        // Rotate up direction around right axis
        camera->up = Vector3RotateByAxisAngle(camera->up, right, angle);
    }
}

} // namespace gengine
