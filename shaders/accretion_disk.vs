#version 330 core

//per vertex attribute (same  quad for every instance)
layout (location = 0) in vec3 aPos;     

//per instance attribute (unique orbital data for each particle)
//x = orbital radius y = starting angle in radians
layout (location = 1) in vec2 aOrbit;

uniform mat4 view;
uniform mat4 projection;
//time in seconds used too animate orbital motion
uniform float time;

//passed to frag shader for temp based coloring
out float vRadius;   

void main()
{
    //kepler's 3rd formula for orbital mechanics
    //inner particles orbit faster than outer ones
    float angularVelocity = 1.0 / sqrt(pow(aOrbit.x, 3.0));

    //advances angle based on how current run time
    float currentAngle =  aOrbit.y + angularVelocity * time;

    //converts polar coordinates (radius, angle) to cartestian of (x,z)
    float x = aOrbit.x * cos(currentAngle);
    float z = aOrbit.x * sin(currentAngle);
    vec3 worldPos = vec3(x, 0.0, z);

    //apOs is the local quad shape at the origin
    //add worldPos to translate the quad to its orbital position
    vec3 finalPos = worldPos + aPos;

    //passing radius to frag shader to color by temp
    vRadius = aOrbit.x;
    gl_Position = projection * view * vec4(finalPos, 1.0);
}