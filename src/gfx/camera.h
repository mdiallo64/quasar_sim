#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
 {
    public:
        Camera(glm::vec3 pos);
        glm::mat4 getViewMatrix();
        void processMouseDrag(float xoffset, float yoffset);
        float getZoom(float yoffset);
        
    private:
        glm::vec3 cameraPos;
        glm::vec3 worldUp;
        glm::vec3 cameraFront;
        float yaw;
        float pitch;
        float sensitivity;
        float fov;
        bool firstMouse;       
};


#endif

