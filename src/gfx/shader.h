#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp>

//Loads,compiles, and links the vertex and fragment shader into a opengl program
//makes it easier to set uniform values using methods
class Shader
{
    public:

        //after successful linking, an ID is assigned
        //used by all uniform setters internally
        unsigned int ID;

        //reads shader source from the given file paths, compiles both shaders
        //lilnks them into a program which is stored in ID
        Shader(const char* vertexPath, const char* fragmentPath);

        //Binds the shader program for draw calls
        void use();

        //uniform setters, each one finds the uniform by name and sets its value
        //you need to call use() before setting uniforms
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setVec3(const std::string &name, const glm::vec3 &value) const;
        void setMat4(const std::string &name, const glm::mat4 &mat) const;

    private:
        //checks shader/program compilation and linking  status
        //prints error long to stdout if the compilation wasnt successful
        void checkCompileErrors(unsigned int shader, std::string type);
};



#endif