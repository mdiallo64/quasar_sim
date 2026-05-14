#include <glad/glad.h>
#include "quasar/jets.h"
#include <random>
#include <vector>

Jets::Jets(float dir, float bY, float len, float tRadius, unsigned int pCount)
{
    direction = dir;
    baseY = bY;
    length = len;
    tubeRadius = tRadius;
    particleCount = pCount;

    setupMesh();
}

void Jets::setupMesh()
{
    std::vector<float> instanceData;


    float s = 0.02;
    float quadVertices[] = 
    {
    -s, 0.0f, -s,
    -s, 0.0f,  s,
     s, 0.0f, -s,

     s, 0.0f, -s,
    -s, 0.0f,  s,
     s, 0.0f,  s 

    };
    

    std::random_device r;
    std::mt19937 gen(r());


    //Y start position staggered along the length of jet
    std::uniform_real_distribution<float> yDist(baseY, baseY + length);

    //horizontal scatter within tubeRadius o
    std::uniform_real_distribution<float> xzDist(-tubeRadius, tubeRadius);

    std::uniform_real_distribution<float> timeDist(0.0f, length);

    for (unsigned int i = 0; i < particleCount; i++) 
    {
        // random starting offset instead of evenly spaced
        float y = timeDist(gen);
        
        instanceData.push_back(y);
        instanceData.push_back(xzDist(gen));
        instanceData.push_back(xzDist(gen));
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &instanceVBO);

    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    ////attribute 1, 3 floats per particle
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, instanceData.size() * sizeof(float), instanceData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    glBindVertexArray(0);

}

void Jets::draw(Shader &shader)
{
    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, particleCount);
    glBindVertexArray(0);
}


