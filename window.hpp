#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "shader_program_builder.hpp"
#include "camera.hpp"

#include <iostream>
#include <functional>

class Window{
public:
    static void init() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // раскомментируйте эту строку, если используете macOS
    #endif
    }

    static void setWindowSize(unsigned int screenWidth, unsigned int screenHeight){
        mScreenWidth = screenWidth;
        mScreenHeight = screenHeight;
    }

    static void setWindowTitle(const char* title){
        mTitle = title;
    }

    static void setRenderFunc(std::function<void(Camera, float, float)> renderStepFunc) { mRenderStepfunc = renderStepFunc; }

    static void buildContext() {
        mWindow= glfwCreateWindow(mScreenWidth, mScreenHeight, mTitle, NULL, NULL);

        glfwMakeContextCurrent(mWindow);
        glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
        glfwSetCursorPosCallback(mWindow, mouse_callback);
        glfwSetScrollCallback(mWindow, scroll_callback);
        
	    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return;
        }

        stbi_set_flip_vertically_on_load(true);
	    glEnable(GL_DEPTH_TEST);
    }

    static void run() {

        while (!glfwWindowShouldClose(mWindow))
        {
            // Логическая часть работы со временем для каждого кадра
            float currentFrame = glfwGetTime();
            mDeltaTime = currentFrame - mLastFrame;
            mLastFrame = currentFrame;
            
            // Обработка ввода
            processInput(mWindow);

            // Рендеринг
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // очищаем буфер цвета и буфер глубины

            
            mRenderStepfunc(mCamera, mScreenWidth, mScreenHeight);

            // glfw: обмен содержимым front- и back- буферов. Отслеживание событий ввода/вывода (была ли нажата/отпущена кнопка, перемещен курсор мыши и т.п.)
            glfwSwapBuffers(mWindow);
            glfwPollEvents();
        }
    }

private:
    inline static unsigned int mScreenWidth = 600;
    inline static unsigned int mScreenHeight = 400;
    inline static const char* mTitle = "Window";
    inline static Camera mCamera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));;
    inline static float mLastX =  mScreenWidth / 2.0f;
    inline static float mLastY = mScreenHeight / 2.0f;

    inline static bool mFirstMouse = true;

    inline static float mDeltaTime = 0.0f;
    inline static float mLastFrame = 0.0f;

    inline static GLFWwindow* mWindow;

    inline static std::function<void(Camera, float, float)>mRenderStepfunc = [](Camera, float, float){};
    
    static void processInput(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            mCamera.ProcessKeyboard(FORWARD, mDeltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            mCamera.ProcessKeyboard(BACKWARD, mDeltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            mCamera.ProcessKeyboard(LEFT, mDeltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            mCamera.ProcessKeyboard(RIGHT, mDeltaTime);
    }

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        if (mFirstMouse)
        {
            mLastX = xpos;
            mLastY = ypos;
            mFirstMouse = false;
        }

        float xoffset = xpos - mLastX;
        float yoffset = mLastY - ypos;

        mLastX = xpos;
        mLastY = ypos;

        mCamera.ProcessMouseMovement(xoffset, yoffset);
    }

    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        mCamera.ProcessMouseScroll(yoffset);
    }

};
