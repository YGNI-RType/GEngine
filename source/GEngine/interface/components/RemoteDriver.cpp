/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** RemoteLocal.cpp
*/

#include "GEngine/interface/components/RemoteLocal.hpp"

namespace gengine::interface::component {

RemoteLocal::RemoteLocal() {
    generateUUID(m_uuid);
}

RemoteLocal::RemoteLocal(const uuids::uuid &uuid)
    : m_uuid(uuid) {
}

// Copy constructor
RemoteLocal::RemoteLocal(const RemoteLocal &other)
    : m_uuid(other.m_uuid)
    , m_whoIAm(other.m_whoIAm) {
}

// Assignment operator
RemoteLocal &RemoteLocal::operator=(const RemoteLocal &other) {
    if (this != &other) {
        m_uuid = other.m_uuid;
        m_whoIAm = other.m_whoIAm;
    }
    return *this;
}

// Overloading the == operator to compare based on UUID
bool RemoteLocal::operator==(const RemoteLocal &other) const {
    return m_uuid == other.m_uuid && m_whoIAm == other.m_whoIAm;
}

// Getter for the UUID as a string (hexadecimal format)
std::string RemoteLocal::getUUIDString() const {
    return uuids::to_string(m_uuid);
}

// Getter for the raw UUID bytes (for network transmission)
const uuids::uuid &RemoteLocal::getUUIDBytes() const {
    return m_uuid;
}

void RemoteLocal::generateUUID(uuids::uuid &toGenerate) {
    std::random_device rd;
    auto seed_data = std::array<int, std::mt19937::state_size>{};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    std::mt19937 generator(seq);
    uuids::uuid_random_generator gen{generator};

    uuids::uuid_random_generator uuidG(gen);
    toGenerate = uuidG();
}
} // namespace gengine::interface::component