#include <glad/glad.h>
#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    //enables exceeotions so failed files opens throw instead of fialing quietly
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    int success;
    char infoLog[512];

    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        //reads file contents into string streams
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();	


        vShaderFile.close();
        fShaderFile.close();

        //converts streams to strings
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();		
    
    }

        catch(std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        //convers to C strings since opengl expects const char*
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        //compiling vertex shader
        unsigned int vertex;
        vertex =  glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

        //error if compilation fails
        if(!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        };
        unsigned int fragment;

        //compiles fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        
        if(!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        };


        //linking both shaders into a program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        //free up resources since they've been linked together
        glDeleteShader(vertex);
        glDeleteShader(fragment);

    
}

        //bind this program so incoming drawcalls use it
        void Shader::use() 
        {
        glUseProgram(ID);
        }

        //sets a single float uniform
        void Shader::setFloat(const std::string &name, float value) const
        {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }

        //bool is stored as an int in glsl so have to use glUniform1i
        void Shader::setBool(const std::string &name, bool value) const
        {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }

        //sets a single int uniform
        void Shader::setInt(const std::string &name, int value) const
        {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }

        //sets a vec3, takes pointer to first float
        void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
            glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }

        //sets mat4, the GL_FALSE is to not transpose the matrix
        void Shader::setMat4(const std::string &name, const glm::mat4 &value) const {
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
        }


        void Shader::checkCompileErrors(unsigned int shader, std::string type) 
        {
            //TODO
        }



