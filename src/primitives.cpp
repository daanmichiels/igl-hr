#include "../thirdparty/glm/glm/glm.hpp"
#include <vector>

#include "primitives.h"
#include "hypermath.h"

// This file needs cleaning up.
// A lot of code occurs multiple times. This should be separated in
// simple functions.

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
    
    // Create a rectangle.
    // Lies in the xz-plane, and is centered at the origin.
    // It's uniformly colored.
    mesh rectangle(float width, float depth, glm::vec4 color)
    {
        // interleaved vertex coordinates and colors
        float data[32];
        float w = width / 2;
        float d = depth / 2;
        glm::vec4 points[4] = {hypermath::exp0(glm::vec4(-w,0, d,0)), \
            hypermath::exp0(glm::vec4( w,0, d,0)), \
            hypermath::exp0(glm::vec4(-w,0,-d,0)), \
            hypermath::exp0(glm::vec4( w,0,-d,0))};
        for(int i=0; i<4; i++)
        {
            data[8*i+0] = points[i].x;
            data[8*i+1] = points[i].y;
            data[8*i+2] = points[i].z;
            data[8*i+3] = points[i].w;
            data[8*i+4] = color.r;
            data[8*i+5] = color.g;
            data[8*i+6] = color.b;
            data[8*i+7] = color.a;
        }

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), 0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (void*)(4*sizeof(GL_FLOAT)));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        mesh result;
        result.vao = vao;
        result.mode = GL_TRIANGLE_STRIP;
        result.first = 0;
        result.count = 4;
        return result;
    }

    // Create a tetrahedron.
    // It will be inscribed in a sphere of the given radius.
    // It will be centered at (0,0,0,1) and one of its tips will
    // point in the positive y-direction (up).
    // It will have nice colors.
    mesh tetrahedron(float radius)
    {
        glm::vec4 a = radius * glm::vec4(0,1,0,0);
        glm::vec4 b = radius * glm::vec4(-sqrt(2)/3,-1.0/3,-sqrt(2.0/3),0);
        glm::vec4 c = radius * glm::vec4(-sqrt(2)/3,-1.0/3, sqrt(2.0/3),0);
        glm::vec4 d = radius * glm::vec4(2*sqrt(2.0)/3,-1.0/3,0,0);

        a = hypermath::exp0(a);
        b = hypermath::exp0(b);
        c = hypermath::exp0(c);
        d = hypermath::exp0(d);

        glm::vec4 red = glm::vec4(1.0f,0.0f,0.0f,1.0f);
        glm::vec4 green = glm::vec4(0.0f,1.0f,0.0f,1.0f);
        glm::vec4 blue = glm::vec4(0.0f,0.0f,1.0f,1.0f);
        glm::vec4 yellow = glm::vec4(1.0f,1.0f,0.0f,1.0f);

        glm::vec4 points[6] = {a,b,c,d,a,b};
        glm::vec4 colors[6] = {red,green,blue,yellow,red,green};
        float data[48];
        for(int i=0; i<6; i++)
        {
            data[8*i+0] = points[i].x;
            data[8*i+1] = points[i].y;
            data[8*i+2] = points[i].z;
            data[8*i+3] = points[i].w;
            data[8*i+4] = colors[i].r;
            data[8*i+5] = colors[i].g;
            data[8*i+6] = colors[i].b;
            data[8*i+7] = colors[i].a;
        }

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), 0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (void*)(4*sizeof(GL_FLOAT)));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        mesh result;
        result.vao = vao;
        result.mode = GL_TRIANGLE_STRIP;
        result.first = 0;
        result.count = 6;
        return result;
    }
}

