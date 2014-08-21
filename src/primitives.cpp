#include "../thirdparty/glm/glm/glm.hpp"
#include <vector>

#include "primitives.h"
#include "hypermath.h"

namespace primitives
{
    mesh triangle(glm::vec4 a, glm::vec4 b, glm::vec4 c)
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

        mesh result;
        result.vao = vao;
        result.mode = GL_TRIANGLES;
        result.first = 0;
        result.count = 3;
        return result;
    }

    mesh rectangle(float width, float depth, int m, int n)
    {
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // interleaved vertex coordinates and colors
        // should we just use an array?
        std::vector<float> data;
        // m*n cells, 2 triangles each, 24 floats per triangle
        data.reserve(24*2*m*n);
        for(int i=0; i<m; i++)
        {
            for(int j=0; j<n; j++)
            {
                glm::vec4 a = hypermath::exp0(glm::vec4(-width/2.0f+width/m*i,     0, depth/2.0f-depth/n*j,     0));
                glm::vec4 b = hypermath::exp0(glm::vec4(-width/2.0f+width/m*(i+1), 0, depth/2.0f-depth/n*j,     0));
                glm::vec4 c = hypermath::exp0(glm::vec4(-width/2.0f+width/m*i,     0, depth/2.0f-depth/n*(j+1), 0));
                glm::vec4 d = hypermath::exp0(glm::vec4(-width/2.0f+width/m*(i+1), 0, depth/2.0f-depth/n*(j+1), 0));

                // *cough elegant cough*
                data.push_back(a.x);
                data.push_back(a.y);
                data.push_back(a.z);
                data.push_back(a.w);
                data.push_back(1.0f);
                data.push_back(1.0f);
                data.push_back(1.0f);
                data.push_back(1.0f);

                data.push_back(b.x);
                data.push_back(b.y);
                data.push_back(b.z);
                data.push_back(b.w);
                data.push_back(0.0f);
                data.push_back(1.0f);
                data.push_back(0.0f);
                data.push_back(1.0f);

                data.push_back(c.x);
                data.push_back(c.y);
                data.push_back(c.z);
                data.push_back(c.w);
                data.push_back(1.0f);
                data.push_back(0.0f);
                data.push_back(0.0f);
                data.push_back(1.0f);

                data.push_back(c.x);
                data.push_back(c.y);
                data.push_back(c.z);
                data.push_back(c.w);
                data.push_back(1.0f);
                data.push_back(0.0f);
                data.push_back(0.0f);
                data.push_back(1.0f);

                data.push_back(b.x);
                data.push_back(b.y);
                data.push_back(b.z);
                data.push_back(b.w);
                data.push_back(0.0f);
                data.push_back(1.0f);
                data.push_back(0.0f);
                data.push_back(1.0f);

                data.push_back(d.x);
                data.push_back(d.y);
                data.push_back(d.z);
                data.push_back(d.w);
                data.push_back(0.0f);
                data.push_back(0.0f);
                data.push_back(1.0f);
                data.push_back(1.0f);
            }
        }
        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24*2*m*n, &data[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), 0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (void*)(4*sizeof(GL_FLOAT)));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        mesh result;
        result.vao = vao;
        result.mode = GL_TRIANGLES;
        result.first = 0;
        result.count = 6;
        return result;
    }
}
