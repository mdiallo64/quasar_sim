#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

//first person camera style class
//processes mouses input to rotate view
//initial possition set at construction and updated through proceessMouseDrag
class Camera
 {
    public:
        //Constructs camera at the given world position. Looking towards -z
        Camera(glm::vec3 pos);

        //Returns view matrix to use in  vertex shader
        //calculated every frame from current position and front vector
        glm::mat4 getViewMatrix();

        //updates the yaw and pitch from mouse delta values, then recomputes cameraFront
        //xoffset: horizontall mouse movement, yoffset: vertical mouse movement
        void processMouseDrag(float xoffset, float yoffset);
    
        //modifies fov
        void processScroll(float yoffset);  
        //returns current fov
        float getFov() const;   

        void processKeyboard(GLFWwindow* window, float deltaTime);       


        
    private:
        glm::vec3 cameraPos; //world position of the camera
        glm::vec3 worldUp;  //global up direction (0, 1,0). Used by lookAt
        glm::vec3 cameraFront; //unit vector pointing wherever the camera faces
        float yaw; //horizontal rotation angle in degrees
        float pitch; //vertical rotation angle in degrees (only from -89 to 89 degrees)
        float sensitivity; //scales mouse delta to rotation speed
        float fov; //field of view in degrees, used for zoom

};



#endif

