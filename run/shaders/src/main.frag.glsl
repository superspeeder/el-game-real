#type fragment
#version 430 core

out vec4 colorOut;

in vec2 texCoords;

uniform vec4 uColor;
uniform uvec2 uTextureSize[32];

uniform sampler2D uTexture[32];

void main() {

    colorOut = texture(uTexture[0], vec2(texCoords.x / float(uTextureSize[0].x), texCoords.y / float(uTextureSize[0].y)));

}