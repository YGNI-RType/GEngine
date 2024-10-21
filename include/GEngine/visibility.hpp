/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** visibility.hpp
*/


#pragma once

#if defined _WIN32 || defined __CYGWIN__
  #ifdef GENGINE_BUILD
    #ifdef __GNUC__
      #define GENGINE_PUBLIC __attribute__ ((dllexport))
    #else
      #define GENGINE_PUBLIC __declspec(dllexport)
    #endif
  #else
    #ifdef __GNUC__
      #define GENGINE_PUBLIC __attribute__ ((dllimport))
    #else
      #define GENGINE_PUBLIC __declspec(dllimport)
    #endif
  #endif
  #define GENGINE_PRIVATE
#else
  #if __GNUC__ >= 4
    #define GENGINE_PUBLIC __attribute__ ((visibility ("default")))
    #define GENGINE_PRIVATE  __attribute__ ((visibility ("hidden")))
  #else
    #define GENGINE_PUBLIC
    #define GENGINE_PRIVATE
  #endif
#endif
