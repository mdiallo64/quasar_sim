#include <glad/glad.h>
#include "quasar/starfield.h"
#include <vector>
#include <random>
#include <cmath>

const float STAR_PI = 3.14159265358979323846f;

Starfield::Starfield(unsigned int starCount, float radius)
{
    this->starCount = starCount;
    setupMesh(radius);
}

void Starfield::setupMesh(float radius)
{
    std::vector<float> vertices;

    std::mt19937 gen(std::random_device{}());

    //uniform distribution for spherical coordinates
    std::uniform_real_distribution<float> thetaDist(0.0f, 2.0f * STAR_PI);
    std::uniform_real_distribution<float> cosDist(-1.0f, 1.0f);
    std::uniform_real_distribution<float> brightDist(0.5f, 1.0f);

    for (unsigned int i = 0; i < starCount; i++)
    {
        //uniform distribution on sphere surface
        //using acos of uniform random avoids clustering at poles
        float theta = thetaDist(gen);
        float phi   = acos(cosDist(gen));

        float x = radius * sin(phi) * cos(theta);
        float y = radius * sin(phi) * sin(theta);
        float z = radius * cos(phi);

        float brightness = brightDist(gen);

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
        vertices.push_back(brightness);
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Starfield::draw(Shader &shader)
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, starCount);
    glBindVertexArray(0);
}