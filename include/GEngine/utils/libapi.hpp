/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** libapi.hpp
*/

#pragma once

#ifdef _WIN32
#ifdef GENGINE_BUILD
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API
#endif
