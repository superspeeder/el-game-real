#pragma once

#include "fwd.h"
#include "utils.hpp"

#include <unordered_map>

class Shader {
public:
    Shader(std::string file);
    ~Shader();

    inline uint32_t getHandle() { return handle; };  

private:

    uint32_t handle;

};

class ShaderProgram {
public:

    ShaderProgram(std::initializer_list<std::string> files);
    ShaderProgram(std::string json);
    ~ShaderProgram();

    inline static std::shared_ptr<ShaderProgram> create(std::initializer_list<std::string> files) { return std::make_shared<ShaderProgram>(files); };

    inline uint32_t getHandle() { return handle; };  

    void use();

    uint32_t getUniformLocation(const std::string& uname);

    void setInt(const std::string& name, int32_t x);
    void setUInt(const std::string& name, uint32_t x);
    void setFloat(const std::string& name, float x);
    void setDouble(const std::string& name, double x);
    void setBool(const std::string& name, bool x);

    void setInt2(const std::string& name, int32_t x, int32_t y);
    void setUInt2(const std::string& name, uint32_t x, uint32_t y);
    void setFloat2(const std::string& name, float x, float y);
    void setDouble2(const std::string& name, double x, double y);
    void setBool2(const std::string& name, bool x, bool y);

    void setInt2(const std::string& name, glm::ivec2 x);
    void setUInt2(const std::string& name, glm::uvec2 x);
    void setFloat2(const std::string& name, glm::fvec2 x);
    void setDouble2(const std::string& name, glm::dvec2 x);
    void setBool2(const std::string& name, glm::bvec2 x);

    void setInt3(const std::string& name, int32_t x, int32_t y, int32_t z);
    void setUInt3(const std::string& name, uint32_t x, uint32_t y, uint32_t z);
    void setFloat3(const std::string& name, float x, float y, float z);
    void setDouble3(const std::string& name, double x, double y, double z);
    void setBool3(const std::string& name, bool x, bool y, bool z);

    void setInt3(const std::string& name, glm::ivec3 x);
    void setUInt3(const std::string& name, glm::uvec3 x);
    void setFloat3(const std::string& name, glm::fvec3 x);
    void setDouble3(const std::string& name, glm::dvec3 x);
    void setBool3(const std::string& name, glm::bvec3 x);

    void setInt4(const std::string& name, int32_t x, int32_t y, int32_t z, int32_t w);
    void setUInt4(const std::string& name, uint32_t x, uint32_t y, uint32_t z, uint32_t w);
    void setFloat4(const std::string& name, float x, float y, float z, float w);
    void setDouble4(const std::string& name, double x, double y, double z, double w);
    void setBool4(const std::string& name, bool x, bool y, bool z, bool w);

    void setInt4(const std::string& name, glm::ivec4 x);
    void setUInt4(const std::string& name, glm::uvec4 x);
    void setFloat4(const std::string& name, glm::fvec4 x);
    void setDouble4(const std::string& name, glm::dvec4 x);
    void setBool4(const std::string& name, glm::bvec4 x);

    void setMatrix2(const std::string& name, glm::mat2x2 m);
    void setMatrix3(const std::string& name, glm::mat3x3 m);
    void setMatrix4(const std::string& name, glm::mat4x4 m);

    void setMatrix2x3(const std::string& name, glm::mat2x3 m);
    void setMatrix3x2(const std::string& name, glm::mat3x2 m);

    void setMatrix2x4(const std::string& name, glm::mat2x4 m);
    void setMatrix4x2(const std::string& name, glm::mat4x2 m);

    void setMatrix4x3(const std::string& name, glm::mat4x3 m);
    void setMatrix3x4(const std::string& name, glm::mat3x4 m);

private:

    std::unordered_map<std::string, uint32_t> uniforms;
    
    uint32_t handle;
};
