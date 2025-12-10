
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "vstd/vlogger.h"
#include "main.h"
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>



constexpr auto WIDTH = 600;
constexpr auto HEIGHT = 600;

int main() 
{
	Logger::SetLevelDefault();

	if (!glfwInit())
	{
		ERROR("GLFW WAS NOT INITIALIZED\n");
		glfwTerminate();
		return -1;
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
		ERROR("FAILED TO CREATE WINDOW\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(Window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		ERROR("FAILED TO INITIALIZE GLAD\n");
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);


	std::array<f32, 9> vertices { {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	}};
	
	u32 VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	u32 VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(f32) * 3, (void*)0);
	glEnableVertexAttribArray(0);


	
	std::string VertexSource = LoadFile(SHADERS_PATH "vs.glsl");
	std::string FragmentSource = LoadFile(SHADERS_PATH "fs.glsl");
	u32 VertexShader, FragmentShader;
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* source = VertexSource.c_str();
	glShaderSource(VertexShader, 1, &source, NULL);
	glCompileShader(VertexShader);
	
	int  Success;
	char InfoLog[512];
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);

	if (!Success)
	{
		glGetShaderInfoLog(VertexShader, 512, NULL, InfoLog);
		ERROR("ERROR::SHADER::VERTEX:COMPILATION\n");
		std::cout << InfoLog << std::endl;
	}
	
	source = FragmentSource.c_str();
	glShaderSource(FragmentShader, 1, &source, NULL);
	glCompileShader(FragmentShader);

	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(FragmentShader, 512, NULL, InfoLog);
		ERROR("ERROR::SHADER::FRAGMENT:COMPILATION\n");
		std::cout << InfoLog << std::endl;
	}

	u32 ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);
	glLinkProgram(ShaderProgram);

	glGetShaderiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(ShaderProgram, 512, NULL, InfoLog);
		ERROR("ERROR::SHADER::LINKAGE\n");
		std::cout << InfoLog << std::endl;
	}
	
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);

	while (!glfwWindowShouldClose(Window))
	{
		processInput(Window);

		glClearColor(0.1f, 0.4f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(ShaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

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


std::string LoadFile(const std::string& path)
{
	std::ifstream file(path);

	if (!file.is_open())
	{
		throw std::runtime_error("FAILED TO OPEN FILE: " + path);
	} 
	
	std::stringstream buff;
	buff << file.rdbuf();
	return buff.str();
}
