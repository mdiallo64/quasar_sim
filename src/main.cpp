#include <glad/glad.h> //loads opengl functions, needs to be 1st
#include <GLFW/glfw3.h> //window creation and input handling
#include <glm/glm.hpp>
#include "gfx/camera.h"
#include <iostream>
#include <string>
#include "gfx/shader.h"
#include "quasar/black_hole.h"
#include "quasar/accretion_disk.h"
#include <glm/gtc/matrix_transform.hpp>
#include "quasar/jets.h"
#include "gfx/bloom.h"


const unsigned int  SCR_WIDTH = 800;
const unsigned int  SCR_HEIGHT = 600;

//global state that is accessible by callbacks
//Camera is glbal so mouse/scroll callbacks can update it wihout any extra prameters
Camera camera (glm::vec3(0.0f,4.0f, 14.0f));

//tracks the previouse mouse position to calculate compute per frame deltas
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

//prevents big jump on the first mouse callback when lastX/lastY
bool firstMouse = true;


//fps counter state
double crntTime = 0.0;
double prevTime = 0.0;
double timeDiff = 0.0;
unsigned int counter = 0;


//called by GLFW whenever the window is resized
//keeps the opengl viewport matched to the new window dimensions
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}


//called by GLFW whenever the mouse moves
//converts absolute cursorposition into a per frame delta and forwards to camera
void mouse_callback(GLFWwindow  *window, double xposIn, double yposIn)
{
    float xpos = (float) xposIn;
    float ypos = (float) yposIn;

    //on the first callback, snap lastX/lastY to the real cursor position
    //makes sure first delta is 0 instead of a jump from the center of the window
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;   
    }

    //computes how far the cursor moved since the last callback
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; //reversed since screen Y goes top to bottom and camera Y goes up

    //update last position for next frame
    lastX = xpos;
    lastY = ypos;

    camera.processMouseDrag(xoffset, yoffset);
}

//called by glfw whenever the scroll wheel moves
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.processScroll((float)yoffset);
}

//polled every frame, if escape is presses, then window is closed
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}



int main()
{
    //Initializing GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Quasar Simulation", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << '\n';
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    //register the callbacks. Will be called by glfw when events occur during glfwPollEvents()
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //hides the OS cursor and locks it to the window to get raw mouse delta for camera rotation
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    //glad initialization
    //loads all opengl function pointers for this driver/platform
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << '\n';
        return -1;
    }

    //constructs bloom object
    Bloom bloom(SCR_WIDTH, SCR_HEIGHT);

    //enables depth testing so closer geometry blocks geometry behind it
    glEnable(GL_DEPTH_TEST);

    //creating shader program by passing vertex and fragment shader names
    Shader shader("shaders/black_hole.vs", "shaders/black_hole.fs");
    BlackHole blackHole(1.0f); //radius of 1.0 in world units

    Shader diskShader("shaders/accretion_disk.vs", "shaders/accretion_disk.fs");
    AccretionDisk disk(2.0f, 8.0f, 30000);

    Shader jetShader("shaders/jets.vs", "shaders/jets.fs");
    Jets upperJet(1.0f, 1.0f, 20.0f, 0.15f, 25000);
    Jets lowerJet(-1.0f, -1.0f, 20.0f, 0.1f, 10);


    //makes sure first timediff isn't huge
    prevTime = glfwGetTime();

    //creating framebuffer
    unsigned int FBO;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    //creating texture for framebuffer
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    //generates texture image on the currently bound texture object
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); //set to null as we are only allocating memory for now. will fill later

    //sets texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

    glBindTexture(GL_TEXTURE_2D, 0);

    //attaches texture to frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);  

    //render buffer objects
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo);  

    //creates depth and stencil renderbuffer object
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    //attaches rbo to framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);  

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  

    glClearColor(0.0, 0.0f, 0.0f, 1.0f);

    //render loop
    while(!glfwWindowShouldClose(window))
    {

        processInput(window);

        //fps counter
        //updates window title about 30 times per second with current fps and ms per frame
        crntTime = glfwGetTime();
        timeDiff = crntTime - prevTime;
        counter++;
        if(timeDiff >= 1.0 / 30.0)
        {
            std::string FPS = std::to_string((1.0 / timeDiff) * counter);
            std::string ms = std::to_string((timeDiff / counter) * 1000);
            std::string newTitle = "Quasar simulation " + FPS + "FPS / " + ms + "ms";
            glfwSetWindowTitle (window, newTitle.c_str());
            prevTime = crntTime;
            counter = 0;
        }




        //builds transformation matrices
        //projection: converts 3d camera space to 2D clip space
        //45 degree fov, apect ratio from window dimensions, near = 0.1, and far = 100.0
        glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        //view: moves the wolrd relative to the camera position and orientation
        glm::mat4 view = camera.getViewMatrix();

        //redirects the scene rendering into HDR buffer
        bloom.bindHDR();


        //additive blending:
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);//particle colors add to whatever is behind them instead of replacing


        glDepthMask(GL_FALSE); //this disables depth writes so particles don't block other particles


        diskShader.use();
        diskShader.setMat4("projection", projection);
        diskShader.setMat4("view", view);
        diskShader.setFloat("time", (float) glfwGetTime());
        diskShader.setFloat("innerRadius", 2.0f);
        diskShader.setFloat("outerRadius", 8.0f);
        disk.draw(diskShader);

        jetShader.use();
        jetShader.setMat4("projection", projection);
        jetShader.setMat4("view", view);
        jetShader.setFloat("time", (float)glfwGetTime());
        jetShader.setFloat("direction", 1.0f);
        jetShader.setFloat("speed", 3.0f);
        jetShader.setFloat("length", 20.0f);
        jetShader.setFloat("baseY", 1.0f);
        upperJet.draw(jetShader);

        jetShader.setFloat("direction", -1.0f);
        jetShader.setFloat("speed", 3.0f);
        jetShader.setFloat("baseY", -1.0f);
        lowerJet.draw(jetShader);


        glDepthMask(GL_TRUE); 
        glDisable(GL_BLEND);

        //the draw scene
        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

                
        //model matrix set inside BlackHole::draw
        blackHole.draw(shader);

        bloom.render();



        //swaps thr front and back buffers, shows the rendered frame to the screen
        glfwSwapBuffers(window);

        //processes any queued OS events, fires the callsbacks like mouse, scroll, and resize
        glfwPollEvents();
    }
        glfwTerminate();
        return 0;
}