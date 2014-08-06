#define GLM_FORCE_RADIANS
#include "../thirdparty/glm/glm/glm.hpp"

#include "primitives.h"

namespace primitives
{
	GLuint triangle(glm::vec4 a, glm::vec4 b, glm::vec4 c)
	{
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// interleaved vertex coordinates and colors
		GLuint buffer;
		glGenBuffers(1, &buffer);
		float data[] = {a.x, a.y, a.z, a.w,
			1.0f, 0.0f, 0.0f, 1.0f,
			b.x, b.y, b.z, b.w,
			0.0f, 1.0f, 0.0, 1.0f,
			c.x, c.y, c.z, c.w,
			0.0f, 0.0f, 1.0f, 1.0f};
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (void*)(4*sizeof(GL_FLOAT)));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		return vao;
	}
}