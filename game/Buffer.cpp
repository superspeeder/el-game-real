#include "Buffer.h"

VertexBuffer::VertexBuffer(uint32_t size, BufferMode mode) {
    glCreateBuffers(1, &handle);
    set(size, mode);
}

VertexBuffer::VertexBuffer(float* data, uint32_t size, BufferMode mode) {
    glCreateBuffers(1, &handle);
    set(data, size, mode);
}

VertexBuffer::VertexBuffer(std::vector<float> data, BufferMode mode) {
    glCreateBuffers(1, &handle);
    set(data, mode);
}

void VertexBuffer::set(uint32_t size, BufferMode mode) {
    bool size_same = vertices.size() == size;
    vertices.resize(size);
    memset(vertices.data(), 0, size * sizeof(float));
    if (size_same && mode == currentMode) {
        glNamedBufferSubData(handle, 0, size * sizeof(float), nullptr);
    } else {
        glNamedBufferData(handle, size * sizeof(float), nullptr, static_cast<GLenum>(mode));
        lastSize = size;
    }
}

void VertexBuffer::set(float* data, uint32_t size, BufferMode mode) {
    bool size_same = vertices.size() == size;
    vertices.resize(size);
    memcpy(vertices.data(), data, size * sizeof(float));
    if (size_same && mode == currentMode) {
        glNamedBufferSubData(handle, 0, size * sizeof(float), data);
    } else {
        glNamedBufferData(handle, size * sizeof(float), data, static_cast<GLenum>(mode));
        lastSize = size;
    }
}

void VertexBuffer::set(std::vector<float> data, BufferMode mode) {
    bool size_same = vertices.size() == data.size();
    vertices = data;
    if (size_same && mode == currentMode) {
        glNamedBufferSubData(handle, 0, data.size() * sizeof(float), data.data());
    } else {
        glNamedBufferData(handle, data.size() * sizeof(float), data.data(), static_cast<GLenum>(mode));
        lastSize = vertices.size();
    }    
}

void VertexBuffer::set_forcepush(uint32_t size, BufferMode mode) {
    vertices.resize(size);
    memset(vertices.data(), 0, size * sizeof(float));
    glNamedBufferData(handle, size * sizeof(float), nullptr, static_cast<GLenum>(mode));
    lastSize = size;
}

void VertexBuffer::set_forcepush(float* data, uint32_t size, BufferMode mode) {
    vertices.resize(size);
    memcpy(vertices.data(), data, size * sizeof(float));
    glNamedBufferData(handle, size * sizeof(float), data, static_cast<GLenum>(mode));
    lastSize = size;
}

void VertexBuffer::set_forcepush(std::vector<float> data, BufferMode mode) {
    vertices = data;
    glNamedBufferData(handle, data.size() * sizeof(float), data.data(), static_cast<GLenum>(mode));
    lastSize = vertices.size();
}

dirty_this<float> VertexBuffer::operator[](size_t idx) {
    return dirty_this<float>(getUnsafe(idx), this);
}

float& VertexBuffer::getUnsafe(size_t idx) {
    return vertices[idx];
}

void VertexBuffer::conservativeFlush() {
    if (isDirty()) {
        flush();
    }
}

void VertexBuffer::resize(size_t newSize) {
    if (vertices.size() != newSize) {
        vertices.resize(newSize);
        push();
    }
}

void VertexBuffer::flush() {
    glNamedBufferSubData(handle, 0, std::min(lastSize, vertices.size()) * sizeof(float), vertices.data());
}

void VertexBuffer::push() {
    glNamedBufferData(handle, vertices.size() * sizeof(float), vertices.data(), static_cast<GLenum>(currentMode));
    lastSize = vertices.size();
}

const std::vector<float>& VertexBuffer::getVertices() {
    return vertices;
}

IndexBuffer::IndexBuffer(uint32_t size, BufferMode mode) {
    glCreateBuffers(1, &handle);
    set(size, mode);
}

IndexBuffer::IndexBuffer(uint32_t* data, uint32_t size, BufferMode mode) {
    glCreateBuffers(1, &handle);
    set(data, size, mode);
}

IndexBuffer::IndexBuffer(std::vector<uint32_t> data, BufferMode mode) {
    glCreateBuffers(1, &handle);
    set(data, mode);
}

void IndexBuffer::set(uint32_t size, BufferMode mode) {
    bool size_same = indices.size() == size;
    indices.resize(size);
    memset(indices.data(), 0, size * sizeof(uint32_t));
    if (size_same && mode == currentMode) {
        glNamedBufferSubData(handle, 0, size * sizeof(uint32_t), nullptr);
    } else {
        glNamedBufferData(handle, size * sizeof(uint32_t), nullptr, static_cast<GLenum>(mode));
        lastSize = size;
    }
}

void IndexBuffer::set(uint32_t* data, uint32_t size, BufferMode mode) {
    bool size_same = indices.size() == size;
    indices.resize(size);
    memcpy(indices.data(), data, size * sizeof(uint32_t));
    if (size_same && mode == currentMode) {
        glNamedBufferSubData(handle, 0, size * sizeof(uint32_t), data);
    } else {
        glNamedBufferData(handle, size * sizeof(uint32_t), data, static_cast<GLenum>(mode));
        lastSize = size;
    }
}

void IndexBuffer::set(std::vector<uint32_t> data, BufferMode mode) {
    bool size_same = indices.size() == data.size();
    indices = data;
    if (size_same && mode == currentMode) {
        glNamedBufferSubData(handle, 0, data.size() * sizeof(uint32_t), data.data());
    } else {
        glNamedBufferData(handle, data.size() * sizeof(uint32_t), data.data(), static_cast<GLenum>(mode));
        lastSize = indices.size();
    }    
}

void IndexBuffer::set_forcepush(uint32_t size, BufferMode mode) {
    indices.resize(size);
    memset(indices.data(), 0, size * sizeof(uint32_t));
    glNamedBufferData(handle, size * sizeof(uint32_t), nullptr, static_cast<GLenum>(mode));
    lastSize = size;
}

void IndexBuffer::set_forcepush(uint32_t* data, uint32_t size, BufferMode mode) {
    indices.resize(size);
    memcpy(indices.data(), data, size * sizeof(uint32_t));
    glNamedBufferData(handle, size * sizeof(uint32_t), data, static_cast<GLenum>(mode));
    lastSize = size;
}

void IndexBuffer::set_forcepush(std::vector<uint32_t> data, BufferMode mode) {
    indices = data;
    glNamedBufferData(handle, data.size() * sizeof(uint32_t), data.data(), static_cast<GLenum>(mode));
    lastSize = indices.size();
}

dirty_this<uint32_t> IndexBuffer::operator[](size_t idx) {
    return dirty_this<uint32_t>(getUnsafe(idx), this);
}

uint32_t& IndexBuffer::getUnsafe(size_t idx) {
    return indices[idx];
}

void IndexBuffer::conservativeFlush() {
    if (isDirty()) {
        flush();
    }
}

void IndexBuffer::resize(size_t newSize) {
    if (indices.size() != newSize) {
        indices.resize(newSize);
        push();
    }
}

void IndexBuffer::flush() {
    glNamedBufferSubData(handle, 0, std::min(lastSize, indices.size()) * sizeof(uint32_t), indices.data());
}

void IndexBuffer::push() {
    glNamedBufferData(handle, indices.size() * sizeof(uint32_t), indices.data(), static_cast<GLenum>(currentMode));
    lastSize = indices.size();
}

const std::vector<uint32_t>& IndexBuffer::getIndices() {
    return indices;
}


IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &handle);
}


VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &handle);
}


VertexArray::VertexArray() {
    glGenVertexArrays(1, &handle);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &handle);
}


void VertexArray::vertexBuffer(VertexBuffer* vbo, std::initializer_list<uint32_t> attributes) {
    uint32_t bid = nextBinding++;
    uint32_t stride = 0;
    for (uint32_t i : attributes) {
        uint32_t aid = nextAttrib++;
        glVertexArrayAttribBinding(handle, aid, bid);
        glVertexArrayAttribFormat(handle, aid, i, GL_FLOAT, false, stride * sizeof(float));
        stride += i;
        glEnableVertexArrayAttrib(handle, aid);
    }

    glVertexArrayVertexBuffer(handle, bid, vbo->getHandle(), 0, stride * sizeof(float));
}

void VertexArray::elementBuffer(IndexBuffer* ibo) {
    glVertexArrayElementBuffer(handle, ibo->getHandle());
}
