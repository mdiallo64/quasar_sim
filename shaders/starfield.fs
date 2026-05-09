#version 330 core

in float vBrightness;
out vec4 color;

void main()
{
    // white point with varying brightness
    color = vec4(vBrightness, vBrightness, vBrightness, 1.0);
}