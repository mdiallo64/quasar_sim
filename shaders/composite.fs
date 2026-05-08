#version 330 core

in vec2 texCoords;
out vec4 color;
uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float exposure;

void main() {
    vec3 hdrColor = texture(scene, texCoords).rgb;
    vec3 bloomColor = texture(bloomBlur, texCoords).rgb;

    hdrColor += bloomColor;

    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

    color = vec4(result, 1.0);
}