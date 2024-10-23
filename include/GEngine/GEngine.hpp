/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** GEngine.hpp
*/

#pragma once

#include "GEngine/BaseEngine.hpp"
#include "GEngine/driver/Engine.hpp"
#include "GEngine/game/Engine.hpp"
#include "GEngine/libdev/Component.hpp"
#include <memory>
#include <mutex>
#include <utility> // For std::forward
#include <vector>

/**
 * @brief Registry class for managing components and systems registration.
 */
struct Registry {
    /**
     * @brief Constructor for the Registry class.
     *
     * @param engines A vector of reference wrappers to engines for component and system registration.
     */
    Registry(std::vector<std::reference_wrapper<gengine::BaseEngine>> &engines)
        : m_engines(engines) {
    }

    /**
     * @brief Register a component type with the engines.
     *
     * This method iterates through the engines and registers the specified component type.
     *
     * @tparam ComponentType The type of component to register.
     */
    template <typename ComponentType>
    void registerComponent(void) {
        for (auto &engine : m_engines)
            engine.get().registerComponent<ComponentType>();
    }

    /**
     * @brief Register a system type with the engines.
     *
     * This method iterates through the engines and registers the specified system type
     * with the provided parameters.
     *
     * @tparam SystemType The type of system to register.
     * @tparam Args Variadic template parameters for system construction arguments.
     * @param params Parameters for constructing the system.
     */
    template <typename SystemType, class... Args>
    void registerSystem(Args &&...params) {
        for (auto &engine : m_engines)
            engine.get().registerSystem<SystemType>(std::forward<Args>(params)...);
    }

private:
    std::vector<std::reference_wrapper<gengine::BaseEngine>> m_engines; ///< Engines registered with this registry.
};

extern "C" {
/**
 * @brief Declare game components and systems for the engine.
 *
 * This function declares all game components and systems needed for the engine.
 * It is intended to be called during the initialization of the GEngine instance.
 *
 * @param r Pointer to the Registry instance for registering components and systems.
 */
void GEngineDeclareGame(Registry *r);

/**
 * @brief Declare driver components and systems for the engine.
 *
 * This function declares all driver components and systems needed for the engine.
 * It is intended to be called during the initialization of the GEngine instance.
 *
 * @param r Pointer to the Registry instance for registering components and systems.
 */
void GEngineDeclareDriver(Registry *r);

/**
 * @brief Declare shared components and events for the engine.
 *
 * This function declares shared components and events that can be used across different systems in the engine.
 * It is intended to be called during the initialization of the GEngine instance.
 *
 * @param r Pointer to the Registry instance for registering components and systems.
 */
void GEngineDeclareShared(Registry *r);
}

/**
 * @brief Main class for the Game Engine singleton.
 *
 * This class manages the initialization and access to the game engine components and systems.
 * It ensures that the engine is instantiated only once and provides methods to access its features.
 */
class GEngine {
public:
    /**
     * @brief Initialize the GEngine singleton instance.
     *
     * This function ensures that the GEngine instance is initialized only once using a thread-safe mechanism.
     * It also registers the necessary game engine elements by calling the registerElements method.
     */
    static void init(void) {
        std::call_once(initFlag, []() {
            instance.reset(new GEngine());
            instance->registerElements();
        });
    }

    /**
     * @brief Get the singleton instance of GEngine.
     *
     * This method returns a reference to the GEngine instance. If the instance is not yet created,
     * it calls the init method to create it.
     *
     * @return Reference to the GEngine instance.
     */
    static GEngine &getInstance(void) {
        if (!instance)
            init();
        return *instance;
    }

    /**
     * @brief Get the pointer to the singleton instance of GEngine.
     *
     * This method returns a pointer to the GEngine instance. If the instance is not yet created,
     * it calls the init method to create it.
     *
     * @return Pointer to the GEngine instance.
     */
    static GEngine *getInstancePointer(void) {
        if (!instance)
            init();
        return instance.get();
    }

    /**
     * @brief Destructor.
     *
     * This destructor is private to prevent deletion of the singleton instance through
     * deletion of pointers to GEngine.
     */
    ~GEngine() = default;

    static gengine::game::Engine &getGame(void) {
        if (!instance)
            init();
        return instance->m_gameEngine;
    }

    static gengine::driver::Engine &getDriver(void) {
        if (!instance)
            init();
        return instance->m_driverEngine;
    }

private:
    /**
     * @brief Private constructor to prevent direct instantiation.
     *
     * This constructor is private to enforce the singleton pattern, preventing the creation
     * of multiple instances of GEngine.
     */
    GEngine() = default;

    /**
     * @brief Register all game engine components and systems.
     *
     * This function calls the necessary functions to declare the various components and systems of the game engine.
     * It ensures that all necessary elements are properly registered during initialization.
     */
    static void registerElements(void) {
        if (!instance)
            init();

        std::vector<std::reference_wrapper<gengine::BaseEngine>> shared = {std::ref(instance->m_gameEngine),
                                                                           std::ref(instance->m_driverEngine)};
        std::vector<std::reference_wrapper<gengine::BaseEngine>> game = {std::ref(instance->m_gameEngine)};
        std::vector<std::reference_wrapper<gengine::BaseEngine>> driver = {std::ref(instance->m_driverEngine)};

        auto sharedRegistry = std::make_unique<Registry>(shared);
        GEngineDeclareShared(sharedRegistry.get());

        auto gameRegistry = std::make_unique<Registry>(game);
        GEngineDeclareGame(gameRegistry.get());

        auto driverRegistry = std::make_unique<Registry>(driver);
        GEngineDeclareDriver(driverRegistry.get());
    }

    // Delete copy constructor and assignment operator to enforce singleton pattern
    GEngine(const GEngine &) = delete;
    GEngine &operator=(const GEngine &) = delete;

    // Singleton instance and initialization flag
    static std::unique_ptr<GEngine> instance;
    static std::once_flag initFlag;

    gengine::game::Engine m_gameEngine;     ///< Instance of the game engine.
    gengine::driver::Engine m_driverEngine; ///< Instance of the driver engine.
};

// Initialize static members
// std::unique_ptr<GEngine> GEngine::instance = nullptr;
// std::once_flag GEngine::initFlag;
