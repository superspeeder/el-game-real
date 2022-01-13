#type vertex
#version 430 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTexCoords;

out vec2 texCoords;

uniform mat4 uViewProjection;

void main() {
    gl_Position = uViewProjection * vec4(inPos.xy, -inPos.z, 1.0);
    texCoords = inTexCoords;
}