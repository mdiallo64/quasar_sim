#ifndef BLACKHOLE_H
#define BLACKHOLE_H

#include "gfx/shader.h"
#include <glm/glm.hpp>

//represents the central black hole
//makes a UV sphere procedurally and renders it as a pure black object
class BlackHole{
    public:

        //constructor for the black hole with given radius
        //immediately generates sphere geometry and uploads it to the GPU
        BlackHole(float radius);

        //Draws the black hole sphere using the provided shader
        //Shader needs to have a "model" mat4 uniformm
        void draw(Shader &shader);

    private:

        unsigned int VAO; //vertex array object to store attribute configs
        unsigned int VBO; //vertex buffer object to store vertex indices on GPU
        unsigned int EBO; //element buffer objeect to store triangle indices on GPU
        unsigned int indexCount; //number of indices passed to glDrawElements
        float radius; //radiues of the sphere in world units

        //generates UV shere vertices and indices and uploads them to VAO/VBO/EBO
        //sectors are the vertical slices around the equator
        //stacks are horizontal rings from top to bottom
        //the more of each the smoother the sphere
        void generateSphere(int sectors, int stacks);


};



#endif