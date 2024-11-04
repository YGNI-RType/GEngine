/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** libapi.hpp
*/

#pragma once

/*
Since Windows uses __declspec(dllexport) and __declspec(dllimport) to export and import symbols, we need to define the
API macro to force Windows to export the symbols from the DLL. This is necessary to ensure that the symbols are
available to the application and it compiles with the game using the engine.
*/

#ifdef _WIN32
#ifdef GENGINE_BUILD
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API
#endif
