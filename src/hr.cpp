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
	window = glfwCreateWindow(800, 600, "Hyperbolic space on the Rift", NULL, NULL);
	if (!window)
	{
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
	if(!build_program())
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	int frames_this_second = 0;
	double previoustime = 0;
	while (!glfwWindowShouldClose(window))
	{
		double t = glfwGetTime();
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float) height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef((float) t * 50.f, 0.f, 0.f, 1.f);
		glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(-0.6f, -0.4f, 0.f);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.6f, -0.4f, 0.f);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.6f, 0.f);
		glEnd();
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
