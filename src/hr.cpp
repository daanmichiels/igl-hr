
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "shaders.h"
#include "camera.h"
#include "hypermath.h"

#include "../thirdparty/glm/glm/gtx/string_cast.hpp"

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
	// test hypermath
	glm::vec4 p0(0,0,0,1);
	glm::vec4 p1(0,0.1,0,sqrt(1+0.1*0.1));
	glm::vec4 p2(0.1,0,0.2,sqrt(1+0.1*0.1+0.2*0.2));
	std::cout << "Distance: " << std::to_string(hypermath::dist(p1,p2)) << "\n";

	glm::vec4 p3 = hypermath::exp(p0, glm::vec4(1,1,0,0));
	std::cout << "          Point after exp: " << glm::to_string(p3) << "\n";
	p3 = hypermath::correct_point(p3);
	std::cout << "Corrected point after exp: " << glm::to_string(p3) << "\n";
	std::cout << "Distance: " << std::to_string(hypermath::dist(p0,p3)) << "\n";
	glm::vec4 direction = hypermath::expinv(p0,p3);
	std::cout << "Direction vector: (" << glm::to_string(direction) << "\n";
	std::cout << "\n";

	std::cout << "Boost matrix: " << glm::to_string(hypermath::translation0(p3)) << "\n";

	// Let's test the translation0 a bit.
	std::cout << "\nTesting whether exp gives a 1-parameter subgroup:\n";
	glm::mat4 a = hypermath::translation0(hypermath::exp(p0, glm::vec4(1,2,3,0)));
	glm::mat4 a2 = hypermath::translation0(hypermath::exp(p0, glm::vec4(2,4,6,0)));
	glm::mat4 ainv = hypermath::translation0(hypermath::exp(p0, glm::vec4(-1,-2,-3,0)));
	std::cout << "\nMatrix A:\n" << glm::to_string(a) << "\n";
	std::cout << "\nMatrix A^2 (first version):\n" << glm::to_string(a2) << "\n";
	std::cout << "\nMatrix A^2 (second version):\n" << glm::to_string(a*a) << "\n";
	std::cout << "\nThis should be Id:\n" << glm::to_string(ainv*a) << "\n";

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
		// (yeah, we're looking in the negative z-direction)
		0.0f, 0.0f, -1.0f, 1.0f,
		0.5f, -0.50f, -1.0f, 1.0f,
		-0.5f, -0.5f, -1.0f, 1.0f,

		// colors
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
	};
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_data), buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	Camera cam(1.2f, 800.0f/600.0f, 0.01f, 10.0f);

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
		cam.use(program);
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
