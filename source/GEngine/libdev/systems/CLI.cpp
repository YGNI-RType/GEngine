/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** CLI.cpp
*/

#include "GEngine/libdev/systems/CLI.hpp"

namespace gengine::system {
CLI::CLI()
    : m_stopReading(false) {
}

void CLI::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&CLI::onStartEngine);
    subscribeToEvent<gengine::system::event::StopEngine>(&CLI::onStopEngine);
    subscribeToEvent<gengine::system::event::MainLoop>(&CLI::onMainLoop);
}

void CLI::onStartEngine(gengine::system::event::StartEngine &e [[maybe_unused]]) {
    m_inputThread = std::thread(&CLI::getInputs, this);
    m_inputThread.detach();
}

void CLI::onStopEngine(gengine::system::event::StopEngine &e [[maybe_unused]]) {
    m_stopReading = true;
}

void CLI::onMainLoop(gengine::system::event::MainLoop &e [[maybe_unused]]) {
    std::lock_guard<std::mutex> lock(m_historyMutex);
    for (const auto &entry : m_userInputHistory)
        publishEvent(gengine::system::event::CLINewInput(splitInput(entry)));
    m_userInputHistory.clear();
}

void CLI::getInputs(void) {
    std::string input;

    while (!m_stopReading) {
        std::cout << "> " << std::flush;
        if (std::getline(std::cin, input)) {
            std::lock_guard<std::mutex> lock(m_historyMutex);
            m_userInputHistory.push_back(input);
        } else
            break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

std::vector<std::string> CLI::splitInput(const std::string &input) {
    std::vector<std::string> words;
    std::istringstream stream(input);
    std::string word;

    while (stream >> word)
        words.push_back(word);
    return words;
}
} // namespace gengine::system
