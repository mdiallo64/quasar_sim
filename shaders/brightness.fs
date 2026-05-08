#version 330 core

in vec2 texCoords;

out vec4 color;

uniform sampler2D hdrScene;

void main()
{
    vec4 sampledColor = texture(hdrScene, texCoords);
    float luminance = dot(sampledColor.rgb, vec3(0.2126, 0.7152, 0.0722));

    if (luminance > 1.0)
    {
        color = sampledColor;  
    }
    else
    {
        color = vec4(0.0, 0.0, 0.0, 1.0); 
    }
}