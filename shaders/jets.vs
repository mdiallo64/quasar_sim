#version 330 core

layout (location = 0) in vec3 aPos;

//x = starting Y position along the jet
//y = X horizontal scatter offset
//z = Z horizontal scatter offset
layout (location = 1) in vec3 posData;

uniform mat4 view;
uniform mat4 projection;

uniform float direction; //+1 for upper jet and -1 for lower jet
uniform float time; //time in seconds, for particle animation
uniform float speed; //how fast particles travel along the jet
uniform float length; //total length of the jet used for looping
uniform float baseY; //Y position where the jet originates (top/bottom)

//passed to fragment shader to fade alpha toward the tip
out float vHeight;


void main()
{
    //move particle along Y axis over time then loop back
    //direction flips the motion for upper (Y+) vs lower (-Y) jet
    float currentY = mod(posData.x + time * speed * direction, length) + baseY;

    //horizontal scatter stays the same, only Y animates
    vec3 worldPos = vec3(posData.y, currentY, posData.z);

    //offset the quad vertex by the particle's world pos
    vec3 finalPos = vec3(worldPos + aPos);

    gl_Position = projection * view * vec4(finalPos, 1.0);

    //normalizes height to 0-1 range
    //clamped to handle negative values from the lower jet direction
    vHeight = clamp((currentY - baseY) / length, 0.0, 1.0);
}
