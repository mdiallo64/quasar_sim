#ifndef STARFIELD_H
#define STARFIELD_H

#include "gfx/shader.h"

class Starfield {
public:
    //starcount:number of stars to generate
    //radius:how far out stars are put
    Starfield(unsigned int starCount, float radius);
    void draw(Shader &shader);

private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int starCount;

    void setupMesh(float radius);
};

#endif