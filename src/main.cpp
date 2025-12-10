
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "vstd/vlogger.h"
#include "main.h"
#include "iostream"
#include <array>
#include "shaders.h"


constexpr auto WIDTH = 600;
constexpr auto HEIGHT = 600;

int main() 
{
	Logger::SetLevelDefault();
	
	GLFWwindow *Window = GetGLFWWindow();
	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);

	std::array<i32, 6> indices{ {
		0, 1, 3,
		1, 2, 3
	} };

	std::array<f32, 12> vertices { {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left
	} };


	u32 VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	u32 VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	u32 EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(f32) * 3, (void*)0);
	glEnableVertexAttribArray(0);


	Shader ShaderProgram(SHADERS_PATH "vs.glsl", SHADERS_PATH "fs.glsl");

	while (!glfwWindowShouldClose(Window))
	{
		processInput(Window);

		glClearColor(0.1f, 0.4f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ShaderProgram.UseProgram();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(Window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

GLFWwindow* GetGLFWWindow() {
	if (!glfwInit())
	{
		glfwTerminate();
		throw std::runtime_error("GLFW WAS NOT INITIALIZED\n");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifndef MACOS
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	GLFWwindow* Window = glfwCreateWindow(WIDTH, HEIGHT, "Perlin Noise", NULL, NULL);
	if (Window == NULL)
	{
		glfwTerminate();
		throw std::runtime_error("FAILED CREATE GLFW WINDOW");
	}

	glfwMakeContextCurrent(Window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		throw std::runtime_error("FAILED INITIALIZE GLAD");
	}

	return Window;

}
