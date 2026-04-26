#include <glad/glad.h>
#include "quasar/black_hole.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <vector>
#include <cmath>

const double PI = 3.14159265358979323846;

BlackHole::BlackHole(float r)
{

    radius = r;
    generateSphere(64, 32); //kind of high resolution (sectors, stacks) respectively


}


void BlackHole::generateSphere(int sectors, int stacks)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    //vertex generation
    //starts at 90 degrees and ends at -90 degrees as i goes from 0 to # of stacks
    for (int i = 0; i <= stacks; i++)
    {

        //stackAngle starts at PI/2 (+90) and decreases to -PI/X (-90)
        //each step is PI/stacks, so subtracting i moves us downwards
        float stackAngle = PI/2 - i * (PI / stacks);

        //starts at 0 degrees and ends at 360 degrees as j goes from 0 to # of sectors
        //iterates around the equator for each stack ring
        for (int j = 0; j <= sectors; j++) 
        {
            //sectorAngle goes from 0 to 360 evenly across all sectors
            float sectorAngle = j * (2 * PI / sectors);

            //spherical to cartesian conversion
            //cos(stackAngle) shrinks the horizontal circle raidus as we get closer to the poles
            //cos/sin(sectorAngle) distributes the point around the horizontal circle
            float x = radius * cos(stackAngle) * cos(sectorAngle);
            float y = radius * sin(stackAngle); //veritcal position is determined only by stackAngle
            float z = radius * cos(stackAngle) * sin(sectorAngle);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }

    }


    //generating indices
    //connects the adjacent vertices into triangles
    //each quad between two adjacent stack rings is split into two triangles
    for (int i = 0; i < stacks; i++)
    {
        //k1 walks along the current stack ring 
        int k1 = i * (sectors + 1);
        //k2 walks along the ring directly below k1
        int k2 = k1 + sectors + 1;


        //As j increases both k1 and k2 walk across their rows to form a quad
        for (int j = 0; j < sectors; j++, k1++, k2++)
        {
            //if loops make sure theres only 1 triangle at each poles
            //since both rows collapse into a single point
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stacks - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    //stores index count for use in glDrawElements
    indexCount = indices.size();

    //uploading data to the GPU
    //generating buffers ID to store in respective variables
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //bind VAO first then all subsequentt VBO/EBO binds and attribute configs are recorded in this VAO
    glBindVertexArray(VAO);

    //bind VBO to array buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //Function that copies user defined data into the current buffer
    //Arguments:
    //1st: Type of data we want to copy into: the vertex buffer object currently 
    //bound to the GL_ARRAY_BUFFER
    //2nd: Specifies the size of the data we want to pass to the buffer
    //3rd: The actual data we want to send
    //4th: How we want the graphics card to manage the data
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW); //upload vertex positions to VBO

    //binds EBO to an element array buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //upload indices to EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    //tells opengl how to interpret the vertex buffer
    //location 0, 3 floats per vertex, don't normalize, stride of 3 floats, no offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //setting to 0, unbinds the VAO
    glBindVertexArray(0);
}

void BlackHole::draw(Shader &shader) {
    //4x4 identity matrix, black hole is at the world origin (not rotating or scaling)
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

