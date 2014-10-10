#include "../thirdparty/glm/glm/glm.hpp"
#include <vector>

#include "primitives.h"
#include "hypermath.h"

// Anonymous namespace.
// Everything in here can only be accessed from this file (unit).
namespace
{
    const glm::vec4 red = glm::vec4(1.0f,0.0f,0.0f,1.0f);
    const glm::vec4 green = glm::vec4(0.0f,1.0f,0.0f,1.0f);
    const glm::vec4 blue = glm::vec4(0.0f,0.0f,1.0f,1.0f);
    const glm::vec4 yellow = glm::vec4(1.0f,1.0f,0.0f,1.0f);

    // Create a vao from positions and colors.
    GLuint vao_from_pos_col(std::vector<glm::vec4> pos, std::vector<glm::vec4> col)
    {
        size_t n = pos.size();
        if(col.size() != n) {
            return 0;
        }

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        float* data = new float[8*n];
        for(size_t i=0; i<n; i++) {
            data[8*i+0] = pos[i].x;
            data[8*i+1] = pos[i].y;
            data[8*i+2] = pos[i].z;
            data[8*i+3] = pos[i].w;
            data[8*i+4] = col[i].r;
            data[8*i+5] = col[i].g;
            data[8*i+6] = col[i].b;
            data[8*i+7] = col[i].a;
        }
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8*n, data, GL_STATIC_DRAW);
        delete [] data;

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), 0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (void*)(4*sizeof(GL_FLOAT)));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return vao;
    }
}

namespace primitives
{
    // Create a triangle.
    // The corners will be red, green, blue.
    mesh triangle(glm::vec4 a, glm::vec4 b, glm::vec4 c)
    {
        mesh result;
        result.vao = vao_from_pos_col({a,b,c},{red,green,blue});
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
        float w = width / 2;
        float d = depth / 2;
        std::vector<glm::vec4> pos = {hypermath::exp0(glm::vec4(-w,0, d,0)), \
            hypermath::exp0(glm::vec4( w,0, d,0)), \
            hypermath::exp0(glm::vec4(-w,0,-d,0)), \
            hypermath::exp0(glm::vec4( w,0,-d,0))};

        mesh result;
        result.vao = vao_from_pos_col(pos, {color,color,color,color});
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

        mesh result;
        result.vao = vao_from_pos_col({a,b,c,d,a,b},{red,green,blue,yellow,red,green});
        result.mode = GL_TRIANGLE_STRIP;
        result.first = 0;
        result.count = 6;
        return result;
    }

    // Create a generic mesh based on a list of vertices
    // The mesh will be multicolored
    mesh generic(std::vector<glm::vec4> v)
    {
        std::vector<glm::vec4> colors;

        for (int i = 0; i < v.size(); i++) {
            switch (i % 3) {
                case 0 :
                    colors.push_back(red);
                    break;

                case 1 :
                    colors.push_back(green);
                    break;

                default :
                    colors.push_back(blue);
                    break; 
            }
        }

        mesh result;
        result.vao = vao_from_pos_col(v, colors);
        result.mode = GL_TRIANGLE_FAN;
        result.first = 0;
        result.count = v.size();
        return result;
    }

    mesh object(std::vector<glm::vec4> v)
    {
        std::vector<glm::vec4> colors;

        for (int i = 0; i < v.size(); i++) {
            switch (i % 3) {
                case 0 :
                    colors.push_back(red);
                    break;

                case 1 :
                    colors.push_back(green);
                    break;

                default :
                    colors.push_back(blue);
                    break; 
            }
        }

        mesh result;
        result.vao = vao_from_pos_col(v, colors);
        result.mode = GL_TRIANGLES;
        result.first = 0;
        result.count = v.size();
        return result;
    }
}

