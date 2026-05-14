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
    //keep mod input always positive by using abs(direction)
    //then apply direction after to flip for lower jet
    float offset = mod(posData.x + time * speed, length);
    float currentY = baseY + offset * direction;


    vec3 worldPos = vec3(posData.y, currentY, posData.z);
    vec3 finalPos = vec3(worldPos + aPos);
    gl_Position = projection * view * vec4(finalPos, 1.0);

    vHeight = clamp(offset / length, 0.0, 1.0);
}
