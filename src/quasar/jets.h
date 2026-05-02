#ifndef JETS_H
#define JETS_H

#include "gfx/shader.h"
#include <glm/glm.hpp>

class Jets {
public:
    Jets(float direction, float baseY, float length, float tubeRadius, unsigned int particleCount);
    void draw(Shader &shader);

private:
    unsigned int VAO;
    unsigned int quadVBO;      
    unsigned int instanceVBO;  
    float direction;          
    float baseY;              
    float length;             
    float tubeRadius;         
    unsigned int particleCount;

    void setupMesh();
};

#endif