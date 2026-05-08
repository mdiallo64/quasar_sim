#version 330 core

//normalized height along jet received from vs
in float vHeight;

out vec4 color;

void main()
{
    //bright blue white at the base
    vec3 jetBase = vec3(0.6, 0.8, 1.0);

    //cooler white at the tip
    vec3 jetTip  = vec3(0.3, 0.5, 1.0);

    //blends color from base to tip depending on height
    vec3 jetColor = mix(jetBase, jetTip, vHeight);

    //fade alpha to zero at the tip so particles fade off
    float alpha = mix(0.3, 0.0, vHeight * vHeight);

    color = vec4(jetColor * 6.0, alpha);
}