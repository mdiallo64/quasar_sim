#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <GLFW/glfw3.h>

Camera::Camera(glm::vec3 pos)
{
    cameraPos = pos;
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    //cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); //default to look towards -z
    cameraFront = glm::vec3(0.0f, 2.0f, 12.0f); 
    yaw = -90.0;    //set at this value so camera faces -z initially (not +x)
    pitch = 0.0f;   //initally looking at level view, no tilt, x = 0
    sensitivity = 0.02f; //can be tuned to adjust camera rotation speed
    fov = 45.0f;    //starting field of view
}

glm::mat4 Camera::getViewMatrix()
{   //takes eye pos, target point, up direction
    //cameraPos + cameraFront gives unit vector 1 point ahead of camera
    //makes it enough to define direction without needing a fixed target
    return glm::lookAt(cameraPos, cameraPos + cameraFront, worldUp);
}

void Camera::processMouseDrag(float xoffset, float yoffset)
{
    //scales pixel delta by sensitivity to get rotation in degrees
    yaw += xoffset * sensitivity;
    pitch += yoffset * sensitivity;

    //makes sure camera wont flip straight up or down
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    //makes use of spherical coords to convert yaw and pitch angles to direction vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch)); //horizontal component along x
    front.y = sin(glm::radians(pitch)); //vertical component
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch)); //horizontal component along z
    cameraFront = glm::normalize(front); //normalize to keep as unit vector
}

  
void Camera::processScroll(float yoffset)
{
    fov -= yoffset;
    if (fov < 1.0f)  fov = 1.0f;
    if (fov > 45.0f) fov = 45.0f;
}

float Camera::getFov() const
{
    return fov;
}

void Camera::processKeyboard(GLFWwindow* window, float deltaTime)
{
    float speed = 5.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += speed * cameraFront;
        
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= speed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)

        cameraPos -= glm::normalize(glm::cross(cameraFront, worldUp)) * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, worldUp)) * speed;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    cameraPos += speed * worldUp;

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    cameraPos -= speed * worldUp;
}
    


