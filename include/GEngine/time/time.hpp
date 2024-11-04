/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** time
*/

#pragma once

#include <cstdint>

namespace Time {
/**
 * @class Clock
 * @brief Provides static methods to retrieve the current time in milliseconds and seconds. It uses chrono to get the
 * current time.
 */
class Clock {
public:
    static uint64_t milliseconds(void);
    static uint64_t seconds(void);
};
} // namespace Time
