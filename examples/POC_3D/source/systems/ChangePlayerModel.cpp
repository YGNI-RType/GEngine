/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** ChangePlayerModel.cpp
*/

#include "systems/ChangePlayerModel.hpp"

#include "GEngine/libdev/Component.hpp" // gengine::Zip

namespace poc3d::system {
void ChangePlayerModel::init(void) {
    subscribeToEvent<gengine::interface::event::SharedEvent<event::ChangePlayerModelEvent>>(
        &ChangePlayerModel::changePlayerModel);
}

void ChangePlayerModel::changePlayerModel(gengine::interface::event::SharedEvent<event::ChangePlayerModelEvent> &e) {
    auto &models = getComponents<gengine::component::driver::output::Model>();
    auto &players = getComponents<component::Player>();
    auto &remotes = getComponents<gengine::interface::component::RemoteLocal>();
    auto &transforms = getComponents<geg::component::Transform3D>();

    for (auto [entity, model, player, remote] : gengine::Zip(models, players, remotes)) {
        if (remote.getUUIDBytes() != e.remoteUUID) // check if its the same remote (zip)
            continue;
        model.txtPath = e->modelPath;
        if (transforms.contains(entity)) {
            auto &transform = transforms.get(entity);
            transform.scale = e->transform.scale;
            transform.rotation = e->transform.rotation;
        }
        if (e->isAnimated) {
            setComponent(entity, geg::component::io::Animation(e->animPath, e->frameDuration));
            model.isAnimated = true;
        } else {
            unsetComponent<geg::component::io::Animation>(entity);
            model.isAnimated = false;
        }
    }
}
} // namespace poc3d::system
