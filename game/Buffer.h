#pragma once

#include "fwd.h"
#include "utils.hpp"

#include <vector>

#include <array>



class VertexBuffer : public DirtyableType {
public:

    VertexBuffer(uint32_t size, BufferMode mode = BufferMode::StaticDraw);
    VertexBuffer(float* data, uint32_t size, BufferMode mode = BufferMode::StaticDraw);
    VertexBuffer(std::vector<float> data, BufferMode mode = BufferMode::StaticDraw);

    template<typename T>
    inline VertexBuffer(T* data, uint32_t size, BufferMode mode = BufferMode::StaticDraw) {
        glCreateBuffers(1, &handle);
        bind();
        set_forcepush<T>(data,size,mode);
    };

    ~VertexBuffer();


    template<typename T>
    inline void set(T* data, uint32_t size, BufferMode mode = BufferMode::StaticDraw) {
        set(reinterpret_cast<float*>(data), (sizeof(T) / sizeof(float)) * size, mode);
    };

    template<typename T>
    inline void set_forcepush(T* data, uint32_t size, BufferMode mode = BufferMode::StaticDraw) {
        set_forcepush(reinterpret_cast<float*>(data), (sizeof(T) / sizeof(float)) * size, mode);
    };

    void set(uint32_t size, BufferMode mode = BufferMode::StaticDraw);
    void set(float* data, uint32_t size, BufferMode mode = BufferMode::StaticDraw);
    void set(std::vector<float> data, BufferMode mode = BufferMode::StaticDraw);

    void set_forcepush(uint32_t size, BufferMode mode = BufferMode::StaticDraw);
    void set_forcepush(float* data, uint32_t size, BufferMode mode = BufferMode::StaticDraw);
    void set_forcepush(std::vector<float> data, BufferMode mode = BufferMode::StaticDraw);

    void conservativeFlush();
    void flush();

    void push();

    void resize(size_t newSize);

    inline uint32_t getSize() const noexcept { return vertices.size(); };
    const std::vector<float>& getVertices();

    dirty_this<float> operator[](size_t idx);

    float& getUnsafe(size_t idx);

    inline const uint32_t& getHandle() { return handle; };


    inline void bind() const { glBindBuffer(GL_ARRAY_BUFFER, handle); };

    template<size_t C>
    inline void setRegion(size_t idx, std::array<float, C> data) {
        memcpy(&vertices[idx], data.data(), C * sizeof(float));
        markDirty();
    };

private:
    uint32_t handle;
    BufferMode currentMode;
    size_t lastSize = 0;
    
    std::vector<float> vertices;
};

class IndexBuffer  : public DirtyableType {
public:

    IndexBuffer(uint32_t size, BufferMode mode = BufferMode::StaticDraw);
    IndexBuffer(uint32_t* data, uint32_t size, BufferMode mode = BufferMode::StaticDraw);
    IndexBuffer(std::vector<uint32_t> data, BufferMode mode = BufferMode::StaticDraw);

    template<typename T>
    inline IndexBuffer(T* data, uint32_t size, BufferMode mode = BufferMode::StaticDraw) {
        glCreateBuffers(1, &handle);
        bind();
        set_forcepush<T>(data,size,mode);
    };

    ~IndexBuffer();


    template<typename T>
    inline void set(T* data, uint32_t size, BufferMode mode = BufferMode::StaticDraw) {
        set(reinterpret_cast<uint32_t*>(data), (sizeof(T) / sizeof(uint32_t)) * size, mode);
    };

    template<typename T>
    inline void set_forcepush(T* data, uint32_t size, BufferMode mode = BufferMode::StaticDraw) {
        set_forcepush(reinterpret_cast<uint32_t*>(data), (sizeof(T) / sizeof(uint32_t)) * size, mode);
    };

    void set(uint32_t size, BufferMode mode = BufferMode::StaticDraw);
    void set(uint32_t* data, uint32_t size, BufferMode mode = BufferMode::StaticDraw);
    void set(std::vector<uint32_t> data, BufferMode mode = BufferMode::StaticDraw);

    void set_forcepush(uint32_t size, BufferMode mode = BufferMode::StaticDraw);
    void set_forcepush(uint32_t* data, uint32_t size, BufferMode mode = BufferMode::StaticDraw);
    void set_forcepush(std::vector<uint32_t> data, BufferMode mode = BufferMode::StaticDraw);

    void conservativeFlush();
    void flush();

    void push();

    void resize(size_t newSize);

    inline uint32_t getSize() const noexcept { return indices.size(); };
    const std::vector<uint32_t>& getIndices();

    dirty_this<uint32_t> operator[](size_t idx);

    uint32_t& getUnsafe(size_t idx);

    inline const uint32_t& getHandle() const noexcept { return handle; };

    inline void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle); };


private:
    uint32_t handle;
    BufferMode currentMode;
    size_t lastSize = 0;

    std::vector<uint32_t> indices;
};

class VertexArray {
public:

    VertexArray();
    ~VertexArray();

    void vertexBuffer(std::shared_ptr<VertexBuffer>& vbo, std::initializer_list<uint32_t> attributes);
    void vertexBuffer(std::shared_ptr<VertexBuffer>& vbo, std::vector<uint32_t> attributes);

    void elementBuffer(std::shared_ptr<IndexBuffer>& ibo);
    void elementBuffer(uint32_t iboHandle);

    inline const uint32_t& getHandle() { return handle; };

    inline void bind() const { glBindVertexArray(handle); };


private:
    uint32_t handle;

    uint32_t nextAttrib = 0;
    uint32_t nextBinding = 0;
};
