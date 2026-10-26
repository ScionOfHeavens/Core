#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_program_builder.hpp"
#include "model.hpp"
#include "window.hpp"


#include <iostream>
#include <stb/stb_image.h>

int main()
{
	Window::init();
	Window::setWindowSize(800, 600);
	Window::setWindowTitle("Java Core project");
    Window::buildContext();

    ShaderProgramBuilder builder;
	ShaderProgram ourShader = builder.withVertexShader("shaders/shader.vs")
									 .withFragmentShader("shaders/shader.fs")
									 .build();
	
    // Загрузка моделей
    Model ourModel("../resources/meshes/spider/Spider.fbx");

    // Отрисовка в режиме каркаса
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Window::setRenderFunc([&ourShader, &ourModel](Camera camera, float screenWidth, float screenHeight){
		ourShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), screenWidth / screenHeight, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // смещаем вниз чтобы быть в центре сцены
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// объект слишком большой для нашей сцены, поэтому немного уменьшим его
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);
	});

	Window::run();


    // glfw: завершение, освобождение всех выделенных ранее GLFW-реcурсов
    glfwTerminate();
    return 0;
}

