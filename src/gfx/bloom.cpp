#include <glad/glad.h>
#include "bloom.h"
#include <iostream>

Bloom::Bloom(unsigned int scrWid, unsigned int scrHght)

    : brightnessShader("shaders/screen.vs", "shaders/brightness.fs")
    , blurShader("shaders/screen.vs", "shaders/blur.fs")
    , compositeShader("shaders/screen.vs", "shaders/composite.fs")
{


    scrWidth = scrWid;
    scrHeight = scrHght;

    setupQuad();
    setupFramebuffers();
}


void Bloom::setupQuad()
{
    float quadVertices[] =
    {
        -1.0f,   1.0f,    0.0f, 1.0f,  
        -1.0f,  -1.0f,    0.0f, 0.0f,
        1.0f,  -1.0f,    1.0f, 0.0f,

        -1.0f,   1.0f,    0.0f, 1.0f,
        1.0f,  -1.0f,    1.0f, 0.0f,
        1.0f,   1.0f,    1.0f, 1.0f,
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
        
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

}

void Bloom::setupFramebuffers()
{

    //creates frame buffer object
    glGenFramebuffers(1, &hdrFBO);
    //makes it the currently bound frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);    

    //creates new texture
    glGenTextures(1, &hdrTexture);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);

    //GL_RGBA16F: each pixel stores 4 16 bit floating point numbers (RGBA)
    //Floating point means numbers aren't clamped 0-1
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, scrWidth, scrHeight, 0, GL_RGBA, GL_FLOAT, NULL);  //null: means allocate memory but don't fill

    //GL_LINEAR: when textures are sampled between exact pixel positions, opengl averages
    //the surrounding pixels smoothly instead of picking the closest one
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    //creates render buffer for depth and stencil
    //GL_DEPTH24_STENCIL8: 24 bits for depth and 8 for stencil
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, scrWidth, scrHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hdrTexture, 0);  

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongTexture);

    for (int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongTexture[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, scrWidth, scrHeight, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongTexture[i], 0);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Bloom::bindHDR()
{
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Bloom::render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[0]);
    glClear(GL_COLOR_BUFFER_BIT);

    brightnessShader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);
    brightnessShader.setInt("hdrScene", 0);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    bool horizontal = true;
    bool firstIteration = true;
    int blurPasses = 10.0;

    blurShader.use();

    for (int i = 0; i < blurPasses; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal ? 1 : 0]);
        glClear(GL_COLOR_BUFFER_BIT);

        blurShader.setBool("horizontal", horizontal);

        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, firstIteration ? pingpongTexture[0] : pingpongTexture[horizontal ? 0 : 1]);
        blurShader.setInt("image", 0);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        horizontal = !horizontal;
        firstIteration = false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    compositeShader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);
    compositeShader.setInt("scene", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pingpongTexture[horizontal ? 1 : 0]);
    compositeShader.setInt("bloomBlur", 1);

    compositeShader.setFloat("exposure", 1.0f);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}