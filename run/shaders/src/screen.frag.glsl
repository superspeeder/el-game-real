#type fragment
#version 430 core

out vec4 colorOut;

in vec2 texCoords;

uniform vec4 uColor;
uniform uvec2 uTextureSize[32];

uniform sampler2D uTexture[32];

void main() {

    colorOut = texture(uTexture[0], texCoords);

}