/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** GEngine.cpp
*/

#include "GEngine/GEngine.hpp"

// Definition of static members
std::unique_ptr<GEngine> GEngine::instance = nullptr;  ///< Initialization of the static instance pointer.
std::once_flag GEngine::initFlag;                       ///< Initialization of the static initialization flag.

// GEngine::GEngine() {
//     // Constructor implementation (if needed)
// }

// GEngine::~GEngine() {
//     // Destructor implementation (if needed)
// }

// void GEngine::init(void) {
//     std::call_once(initFlag, [](void) {
//         instance.reset(new GEngine());
//     });
//     registerElements();
// }

// GEngine &GEngine::getInstance(void) {
//     if (!instance)
//         init();
//     return *instance;
// }

// GEngine *GEngine::getInstancePointer(void) {
//     if (!instance)
//         init();
//     return instance.get();
// }

// void GEngine::registerElements(void) {
//     if (!instance) {
//         init();
//     }

//     std::vector<std::reference_wrapper<gengine::BaseEngine>> game = {
//         std::ref(instance->m_gameEngine)
//     };
//     std::vector<std::reference_wrapper<gengine::BaseEngine>> driver = {
//         std::ref(instance->m_driverEngine)
//     };
//     std::vector<std::reference_wrapper<gengine::BaseEngine>> shared = {
//         std::ref(instance->m_gameEngine),
//         std::ref(instance->m_driverEngine)
//     };

//     auto gameRegistry = std::make_unique<Registry>(game);
//     GEngineDeclareGame(gameRegistry.get());

//     auto driverRegistry = std::make_unique<Registry>(driver);
//     GEngineDeclareDriver(driverRegistry.get());

//     auto sharedRegistry = std::make_unique<Registry>(shared);
//     GEngineDeclareShared(sharedRegistry.get());
// }
