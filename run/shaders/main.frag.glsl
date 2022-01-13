#type fragment
#version 430 core

out vec4 colorOut;

in vec2 texCoords;

uniform vec4 uColor;
uniform uvec2 uTextureSize[32];

uniform sampler2D uTexture[32];

void main() {

    uvec2 currentPixel = uvec2(floor(texCoords.x), floor(texCoords.y));
    vec2 samplingLocation = vec2((float(currentPixel.x) + 0.5) / float(uTextureSize[0].x), (float(currentPixel.y) + 0.5) / float(uTextureSize[0].x));
    
    colorOut = texture(uTexture[0], samplingLocation);
//    colorOut = vec4(samplingLocation, 0.0, 1.0);
}