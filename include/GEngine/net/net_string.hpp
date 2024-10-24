/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** net_string
*/

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

namespace Network {

template <size_t Size>
class NetString {
public:
    NetString() {
        m_array.fill(0);
    }

    NetString(const char *str) {
        size_t len = std::strlen(str);
        if (len > Size)
            throw std::out_of_range("String too long");
        std::memcpy(m_array.data(), str, len);
        m_array[len] = 0; // Null-terminate the string
    }
    NetString(const std::string &str) {
        size_t len = str.size();
        if (len > Size)
            throw std::out_of_range("String too long");
        std::memcpy(m_array.data(), str.c_str(), len);
        m_array[len] = 0; // Null-terminate the string
    }

    NetString(const NetString &other) {
        std::memcpy(m_array.data(), other.m_array.data(), Size);
    }
    ~NetString() = default;

    NetString &&operator=(const NetString &other) {
        if (this != &other)
            std::memcpy(m_array.data(), other.m_array.data(), Size);
        return std::move(*this);
    }

    size_t size() const {
        return std::strlen(reinterpret_cast<const char *>(m_array.data()));
    }

    size_t length() const {
        return size();
    }

    bool empty() const {
        return m_array[0] == 0;
    }

    const char *c_str() const {
        return reinterpret_cast<const char *>(m_array.data());
    }

    char operator[](size_t index) {
        if (index >= Size)
            throw std::out_of_range("Index out of range");
        return m_array[index];
    }

    char operator[](size_t index) const {
        if (index >= Size)
            throw std::out_of_range("Index out of range");
        return m_array[index];
    }

    NetString &operator+=(const NetString &other) {
        size_t this_len = size();
        size_t other_len = other.size();
        if (this_len + other_len >= Size)
            throw std::out_of_range("Resulting string too long");
        std::memcpy(m_array.data() + this_len, other.m_array.data(), other_len + 1);
        return *this;
    }

    NetString operator+(const NetString &other) const {
        NetString result = *this;
        result += other;
        return result;
    }

    bool operator==(const NetString &other) const {
        return std::strcmp(c_str(), other.c_str()) == 0;
    }

    bool operator!=(const NetString &other) const {
        return !(*this == other);
    }

private:
    std::array<char, Size> m_array;
};
} // namespace Network
