#ifndef BLOOM_H
#define BLOOM_H

#include <glm/glm.hpp>
#include "gfx/shader.h"


class Bloom
{
    public:
        Bloom(unsigned int scrWidth, unsigned int scrHeight);
        void bindHDR();        
        void render();
        void draw(Shader &shader);

    private:

        unsigned int hdrFBO;
        unsigned int hdrTexture;
        unsigned int rbo;
        unsigned int quadVAO;
        unsigned int quadVBO;
        unsigned int scrWidth;
        unsigned int scrHeight;

        unsigned int pingpongFBO[2];
        unsigned int pingpongTexture[2];

        void setupFramebuffers();
        void setupQuad();

        Shader brightnessShader;
        Shader blurShader;
        Shader compositeShader;



};

#endif