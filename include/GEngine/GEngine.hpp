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
#include "GEngine/utils/libapi.hpp"
#include <memory>
#include <mutex>
#include <utility> // For std::forward
#include <vector>

#include "GEngine/libdev/System.hpp"

/**
 * @brief Registry class for managing components and systems registration.
 */
struct Registry {
    /**
     * @brief Constructor for the Registry class.
     *
     * @param engines A vector of reference wrappers to engines for component and system registration.
     */
    Registry(gengine::BaseEngine &local, gengine::BaseEngine &remote)
        : m_local(local)
        , m_remote(remote) {
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
        m_local.registerComponent<ComponentType>();
        m_remote.registerComponent<ComponentType>();
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
        if constexpr (!std::is_base_of<gengine::LocalSystem, SystemType>::value)
            m_remote.registerSystem<SystemType>(std::forward<Args>(params)...);
        if constexpr (!std::is_base_of<gengine::RemoteSystem, SystemType>::value)
            m_local.registerSystem<SystemType>(std::forward<Args>(params)...);
    }

private:
    gengine::BaseEngine &m_local;
    gengine::BaseEngine &m_remote;
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
void GEngineDeclareComponents(Registry *r);

/**
 * @brief Declare driver components and systems for the engine.
 *
 * This function declares all driver components and systems needed for the engine.
 * It is intended to be called during the initialization of the GEngine instance.
 *
 * @param r Pointer to the Registry instance for registering components and systems.
 */
void GEngineDeclareSystems(Registry *r);

/**
 * @brief Declare shared components and events for the engine.
 *
 * This function declares shared components and events that can be used across different systems in the engine.
 * It is intended to be called during the initialization of the GEngine instance.
 *
 * @param r Pointer to the Registry instance for registering components and systems.
 */
void GEngineDeclareEvents(Registry *r);
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
    static void init(int size, const char **argv) {
        std::call_once(initFlag, [size, argv]() {
            instance.reset(new GEngine());
            instance->getLocal().setParams(argv, size);
            instance->getRemote().setParams(argv, size);
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
            init(0, NULL);
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
            init(0, NULL);
        return instance.get();
    }

    /**
     * @brief Destructor.
     *
     * This destructor is private to prevent deletion of the singleton instance through
     * deletion of pointers to GEngine.
     */
    ~GEngine() = default;

    static gengine::BaseEngine &getLocal(void) {
        if (!instance)
            init(0, NULL);
        return instance->m_local;
    }

    static gengine::BaseEngine &getRemote(void) {
        if (!instance)
            init(0, NULL);
        return instance->m_remote;
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
            init(0, NULL);

        auto sharedRegistry = std::make_unique<Registry>(instance->m_local, instance->m_remote);

        GEngineDeclareComponents(sharedRegistry.get());
        GEngineDeclareSystems(sharedRegistry.get());
        GEngineDeclareEvents(sharedRegistry.get());
    }

    // Delete copy constructor and assignment operator to enforce singleton pattern
    GEngine(const GEngine &) = delete;
    GEngine &operator=(const GEngine &) = delete;

    // Singleton instance and initialization flag
    API static std::unique_ptr<GEngine> instance;
    API static std::once_flag initFlag;

    gengine::BaseEngine m_local;  ///< Instance of the game engine.
    gengine::BaseEngine m_remote; ///< Instance of the driver engine.
};

// Initialize static members
// std::unique_ptr<GEngine> GEngine::instance = nullptr;
// std::once_flag GEngine::initFlag;
