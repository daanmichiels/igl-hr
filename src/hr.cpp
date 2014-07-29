// trying to get this up and running
// code taken from
// http://www.glfw.org/docs/latest/quick.html

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "shaders.h"

static void error_callback(int error, const char* description)
{
	std::cerr << description;
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
int main()
{
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	window = glfwCreateWindow(800, 600, "Hyperbolic space on the Rift", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create window. Do you have OpenGL 3.0 or higher?\n";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	// we have an OpenGL context
	// print some information
	GLenum error = glewInit();
	if(error != GLEW_OK)
	{
		std::cout << "Error: " << glewGetErrorString(error) << "\n";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << "\n";
	std::cout << "Using OpenGL " << glGetString(GL_VERSION) << "\n";
	GLuint program = build_program();
	if(!program)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// generate a buffer and fill it
	const float buffer_data[] = {
		// positions
		0.75f, 0.75f, 0.0f, 1.0f,
		0.75f, -0.75f, 0.0f, 1.0f,
		-0.75f, -0.75f, 0.0f, 1.0f,

		// colors
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.5f,
		0.0f, 0.0f, 1.0f,
	};
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_data), buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	int frames_this_second = 0;
	double previoustime = 0;
	while (!glfwWindowShouldClose(window))
	{
		double t = glfwGetTime();

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(program);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(12*sizeof(float)));

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glUseProgram(0);

		glfwSwapBuffers(window);
		glfwPollEvents();

		frames_this_second++;
		if(t >= previoustime + 1.0)
		{
			previoustime += 1.0;
			glfwSetWindowTitle(window, std::to_string(frames_this_second).c_str());
			std::cout << std::to_string(frames_this_second) << "\n";
			frames_this_second = 0;
		}
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
