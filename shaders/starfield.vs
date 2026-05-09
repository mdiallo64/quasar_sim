#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in float aBrightness;

uniform mat4 view;
uniform mat4 projection;

out float vBrightness;

void main()
{
    vBrightness = aBrightness;

    // brighter stars appear slightly larger
    gl_PointSize = aBrightness * 0.0f;

    gl_Position = projection * view * vec4(aPos, 1.0);
}