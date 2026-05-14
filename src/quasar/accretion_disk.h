#ifndef ACCRETIONDISK_H
#define ACCRETIONDISK_H

#include "gfx/shader.h"
#include <glm/glm.hpp>

class AccretionDisk{

    public:

        //innerRadius: distance from black hole center to inner edge of risk
        //outerRadius: distance from black hole center to outer edge of risk
        //particleCount: number of instanced particles in the ring
        AccretionDisk(float innerRadius, float outerRadius, unsigned int particleCount);

        void draw(Shader &shader);
        
        void regenerate(unsigned int newCount);


    private:

        //quad vertex position which is the same for all instances
        unsigned int quadVBO;
        //per particle orbital data like radius and initial angle
        unsigned int instanceVBO;
        unsigned int VAO;
        float innerRadius;
        float outerRadius;
        unsigned int particleCount;

        void setupMesh();

    
};

#endif