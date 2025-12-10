
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "vstd/vlogger.h"
#include "main.h"
#include "iostream"
#include <array>
#include "shaders.h"
#include <Windows.h>

//CHECK https://www.songho.ca/opengl/gl_pbo.html

constexpr auto WIDTH = 600;
constexpr auto HEIGHT = 600;
constexpr auto SIZE_COLOR_BUFFER = WIDTH * HEIGHT * 4;

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

	std::array<f32, 16> vertices { {
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f,
		-1.0f,  1.0f,  0.0f, 1.0f
	} };
	
	LPVOID ColorBuffer = VirtualAlloc(0, SIZE_COLOR_BUFFER, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!ColorBuffer) throw std::runtime_error("FAILED ALLOCATING COLOR BUFFER.");
	WriteWeirdGradient(ColorBuffer);

	Shader ShaderProgram(SHADERS_PATH "vs.glsl", SHADERS_PATH "fs.glsl");

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

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(f32) * 4, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(f32) * 4, (void*)(2 * sizeof(f32)));
	glEnableVertexAttribArray(1);

	u32 texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)0);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, ColorBuffer);




	while (!glfwWindowShouldClose(Window))
	{
		processInput(Window);

		glClearColor(0.1f, 0.4f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ShaderProgram.UseProgram();
		glBindVertexArray(VAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(ShaderProgram.GetProgram(), "ColorBuff"), 0);


		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(Window);
		glfwPollEvents();
	}

	glfwTerminate();

	VirtualFree(ColorBuffer, 0, MEM_RELEASE);
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


void WriteWeirdGradient(void* buff) 
{
	u32 Pitch = 4 * WIDTH;
	u8* Row = (u8 *)buff;
	for (int x = 0; x < HEIGHT; x++)
	{
		u32* Pixel = (u32 *) Row;
		for (int y = 0; y < WIDTH; y++)
		{
			// RGBA
			u8 Blue = x % 255;
			u8 Green = y % 255;

			*Pixel++ = (Blue << 8 | Green << 16);
		}
		Row += Pitch;
	}
}
