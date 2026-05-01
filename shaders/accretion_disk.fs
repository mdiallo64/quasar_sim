#version 330 core

//orbital radius of particle received from vertex shader
in float vRadius;

out vec4 color;

//disk boundaries passed from code in main
//used to normalize radius to 0-1 range
uniform float innerRadius;
uniform float outerRadius;


void main()
{
    //hot inner color
    vec3 inner = vec3(0.9, 0.95, 1.0);
    //cool outer color
    vec3 outer = vec3(1.0, 0.4, 0.1);

    //normalize radius to 0-1
    //0 = inner edge (hottest) while 1 = outer edge (coolest)
    float tNorm = (vRadius - innerRadius) / (outerRadius - innerRadius);

    //blend function to blend between hot and cool based on position
    vec3 mixedColor = mix(inner, outer, tNorm);
    float alpha = mix(0.8, 0.3, tNorm);
    color = vec4(mixedColor, alpha);
}
