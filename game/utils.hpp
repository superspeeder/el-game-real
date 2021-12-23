#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

template<typename T>
struct dirtyable {
    T value;
    bool dirty = true;

    inline dirtyable(T val) : value{val} {};
    inline void operator=(const T& val) { value = val; dirty = true; };
    inline bool isDirty() const noexcept { return dirty; };
    inline void clean() { dirty = false; };
    inline void markDirty() { dirty = true; };
    inline const T* operator->() { return &value; };
};