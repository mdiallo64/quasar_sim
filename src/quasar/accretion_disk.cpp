#include <glad/glad.h>
#include "quasar/accretion_disk.h"
#include <random>

const double PI = 3.14159265358979323846;


AccretionDisk::AccretionDisk(float iR, float oR, unsigned int pC)
{


    innerRadius = iR;
    outerRadius = oR ;
    particleCount = pC;

    //helper function
    setupMesh();
}

void AccretionDisk::setupMesh()
{

    std::vector<float> instanceData;


    float s = 0.05;
    float quadVertices[] = 
    {
    -s, 0.0f, -s,
    -s, 0.0f,  s,
     s, 0.0f, -s,

     s, 0.0f, -s,
    -s, 0.0f,  s,
     s, 0.0f,  s 

    };
    

    //generates random number between given ranges and adds them to instanceData vector
    std::random_device r;
    std::mt19937 gen(r());
    std::uniform_real_distribution<float> rad(innerRadius, outerRadius);
    std::uniform_real_distribution<float> ang(0.0, 2.0 * PI);

    for (int i = 0; i < particleCount; i++ )
    {
        instanceData.push_back(rad(gen));
        instanceData.push_back(ang(gen));
    }
 

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &instanceVBO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(VAO);

    
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, instanceData.size() * sizeof(float), instanceData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glVertexAttribDivisor(1, 1);

    glBindVertexArray(0);   

}

void AccretionDisk::draw(Shader &shader) 
{

    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, particleCount);
    glBindVertexArray(0);
}
    