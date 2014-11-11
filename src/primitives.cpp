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

    // Returns normalized vector of triangle vec4s
    std::vector<glm::vec4> normalize_triangle_vector(std::vector<glm::vec4> input)
    {
        std::vector<glm::vec4> normalized;
        for(unsigned int i=0; i < input.size(); i++)
        {
            normalized.push_back(hypermath::exp0(glm::normalize(input[i])));
        }
        return normalized;
    }

    /* Subdivision Algorithm
    This works by finding the mid point between each pair of vertices, and generating 4 triangles.

                        a
                       *
                      * *
                     *   *
    midpoint a,b    * * * *  midpoint a,c
                   *       *
                  * *     * *
                 *   *   *   *
              b * * * * * * * * c 
                  midpoint b,c

    Passing true as the last argument discards the "middle" each step, resulting in sierpinski subdivision
    Increasing the number of divisions from 1 to 2 will perform the first division, then divide all of the newly generated triangles.
    Can crash the program for too large of a large number of divisions.
    */
    
    std::vector<glm::vec4> subdivide_triangle_vector(glm::vec4 a, glm::vec4 b, glm::vec4 c, int divisions, bool sierpinski)
    {
        std::vector<glm::vec4> triangles;
        if(divisions < 1)
        {
            triangles.push_back(a);
            triangles.push_back(b);
            triangles.push_back(c);
            return triangles;
        }
        glm::vec4 ab, bc, ac;
        std::vector<glm::vec4> t1, t2, t3, t4;

        ab = hypermath::midpoint(a, b, 0.5f);
        ac = hypermath::midpoint(a, c, 0.5f);
        bc = hypermath::midpoint(b, c, 0.5f);

        //recursive calls to each subdivided triangle
        if(divisions > 0)
        {
            t1 = subdivide_triangle_vector(a, ab, ac, divisions - 1, sierpinski);
            t2 = subdivide_triangle_vector(ab, b, bc, divisions -1, sierpinski);
            t3 = subdivide_triangle_vector(ac, bc, c, divisions -1, sierpinski);
            t4 = subdivide_triangle_vector(ab, ac, bc, divisions -1, sierpinski);
            for(unsigned int i = 0; i < t1.size(); i++)
            {
                triangles.push_back(t1[i]);
            }

            for(unsigned int i = 0; i < t2.size(); i++)
            {
                triangles.push_back(t2[i]);
            }

            for(unsigned int i = 0; i < t3.size(); i++)
            {
                triangles.push_back(t3[i]);
            }
            if(!sierpinski)
            {
                for(unsigned int i = 0; i < t4.size(); i++)
                {
                    triangles.push_back(t4[i]);
                }
            }
            return triangles;
        }
        //base case
         triangles.push_back(a);
         triangles.push_back(ab);
         triangles.push_back(ac);
         triangles.push_back(ab);
         triangles.push_back(b);
         triangles.push_back(bc);
         triangles.push_back(ac);
         triangles.push_back(bc);
         triangles.push_back(c);
         if(!sierpinski)
         {
            triangles.push_back(ab);
            triangles.push_back(ac);
            triangles.push_back(bc);
         }
         return triangles;
    }

}

namespace primitives
{
    // Create a line between two arbitrary points.
    // the line is uniformly colored
    mesh line(glm::vec4 a, glm::vec4 b, glm::vec4 col)
    {
        mesh result;
        result.vao = vao_from_pos_col({a,b}, {col,col});
        result.mode = GL_LINES;
        result.first = 0;
        result.count = 2;
        return result;
    }

    // Make a set of XYZ axes, colored red green and blue respectively, inside of a sphere of given radius
    mesh axes(float radius)
    {
        mesh result;
        std::vector<glm::vec4> pos;
        std::vector<glm::vec4> col;
        
        //first set the vectors for the axes.
        pos.push_back(hypermath::exp0(radius * glm::vec4(-1,0,0,0)));
        pos.push_back(hypermath::exp0(radius * glm::vec4(1,0,0,0)));
        pos.push_back(hypermath::exp0(radius * glm::vec4(0,-1,0,0)));
        pos.push_back(hypermath::exp0(radius * glm::vec4(0,1,0,0)));
        pos.push_back(hypermath::exp0(radius * glm::vec4(0,0,-1,0)));
        pos.push_back(hypermath::exp0(radius * glm::vec4(0,0,1,0)));
        
        //set the colors for each corresponding axis. each line is uniformly colored
        col.push_back(glm::vec4(1.0f,0.0f,0.0f,1.0f));
        col.push_back(glm::vec4(1.0f,0.0f,0.0f,1.0f));
        col.push_back(glm::vec4(0.0f,0.0f,1.0f,1.0f));
        col.push_back(glm::vec4(0.0f,0.0f,1.0f,1.0f));
        col.push_back(glm::vec4(0.0f,1.0f,0.0f,1.0f));
        col.push_back(glm::vec4(0.0f,1.0f,0.0f,1.0f));

        result.vao = vao_from_pos_col(pos, col);
        result.mode = GL_LINES;
        result.first = 0;
        result.count = 6;
        return result;
    }

    // Create a grid mesh. X axis is red, Y axis is green, and Z axis is blue
    mesh grid(double grid_space)
    {
        mesh result;
        std::vector<glm::vec4> pos;
        std::vector<glm::vec4> col;
        int steps = (int) ceil(10/grid_space);
        glm::vec4 x_pos(hypermath::exp0(glm::vec4(10, 0, 0, 0)));
        glm::vec4 x_neg(hypermath::exp0(glm::vec4(-10, 0, 0, 0)));
        glm::vec4 y_pos(hypermath::exp0(glm::vec4(0, 10, 0, 0)));
        glm::vec4 y_neg(hypermath::exp0(glm::vec4(0, -10, 0, 0)));
        glm::vec4 z_pos(hypermath::exp0(glm::vec4(0, 0, 10, 0)));
        glm::vec4 z_neg(hypermath::exp0(glm::vec4(0, 0, -10, 0)));

        for(int j=0; j<= 2*steps; j++)
        {
            for(int i=0; i<= 2*steps; i++)
            {
                int j_prime = -1 * steps + j;
                int i_prime = -1 * steps + i;
                // X Grid
                pos.push_back(x_pos * hypermath::translation0(hypermath::exp0(glm::vec4(0, grid_space * j_prime, grid_space * i_prime, 0))));
                pos.push_back(x_neg * hypermath::translation0(hypermath::exp0(glm::vec4(0, grid_space * j_prime, grid_space * i_prime, 0))));
                // Y Grid
                pos.push_back(y_pos * hypermath::translation0(hypermath::exp0(glm::vec4(grid_space*i_prime, -0, grid_space*j_prime, 0))));
                pos.push_back(y_neg * hypermath::translation0(hypermath::exp0(glm::vec4(grid_space*i_prime, 0, grid_space*j_prime, 0))));
                // Z Grid
                pos.push_back(z_pos * hypermath::translation0(hypermath::exp0(glm::vec4(grid_space * j_prime, grid_space * i_prime, 0, 0))));
                pos.push_back(z_neg * hypermath::translation0(hypermath::exp0(glm::vec4(grid_space * j_prime, grid_space * i_prime, 0, 0))));

                col.push_back(glm::vec4(1.0f,0.0f,0.0f,1.0f));
                col.push_back(glm::vec4(1.0f,0.0f,0.0f,1.0f));
                col.push_back(glm::vec4(0.0f,0.0f,1.0f,1.0f));
                col.push_back(glm::vec4(0.0f,0.0f,1.0f,1.0f));
                col.push_back(glm::vec4(0.0f,1.0f,0.0f,1.0f));
                col.push_back(glm::vec4(0.0f,1.0f,0.0f,1.0f));
            }
        }            
        
        result.vao = vao_from_pos_col(pos, col);
        result.mode = GL_LINES;
        result.first = 0;
        result.count = pos.size();
        return result;
    }

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
        std::vector<glm::vec4> pos = {hypermath::exp0(glm::vec4(-w,0,d,0)), \
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
    // Creates an octahedron at the origin. Each triangle is colored with red, green, and blue vertices.
    mesh octahedron(float radius)
    {
        glm::vec4 a = hypermath::exp0(radius * glm::vec4(0,0,1,0));
        glm::vec4 b = hypermath::exp0(radius * glm::vec4(0,1,0,0));
        glm::vec4 c = hypermath::exp0(radius * glm::vec4(0,-1,0,0));
        glm::vec4 d = hypermath::exp0(radius * glm::vec4(1,0,0,0));
        glm::vec4 e = hypermath::exp0(radius * glm::vec4(-1,0,0,0));
        glm::vec4 f = hypermath::exp0(radius * glm::vec4(0,0,-1,0));
        mesh result;
        std::vector<glm::vec4> colors;
        for(unsigned int i = 0; i < 24; i++)
        {
            switch (i % 3) 
            {
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
        result.vao = vao_from_pos_col({a,b,d,a,d,c,a,c,e,a,e,b, f,b,d,f,d,c,f,c,e,f,e,b}, colors);
        result.mode = GL_TRIANGLES;
        result.first = 0;
        result.count = 24;
        return result;
    }

    mesh object(std::vector<glm::vec4> v)
    {
        std::vector<glm::vec4> colors;

        for (unsigned int i = 0; i < v.size(); i++) {
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
    
    // Creates a subdivided triangle mesh, passing true to sierpinski makes a sierpinski subdivided triangle. 
    // See the comment for the subdivision algorithm for more information
    
    mesh subdivided_triangle(glm::vec4 a, glm::vec4 b, glm::vec4 c, int divisions, bool sierpinski)
    {
        std::vector<glm::vec4> triangle;
        triangle = subdivide_triangle_vector(a,b,c,divisions,sierpinski);
        std::vector<glm::vec4> colors;
        for(unsigned int i = 0; i < triangle.size()/3; i++)
        {
            colors.push_back(red);
            colors.push_back(green);
            colors.push_back(blue);
        }

        mesh result;
        result.vao = vao_from_pos_col(triangle, colors);
        result.mode = GL_TRIANGLES;
        result.first = 0;
        result.count = triangle.size();
        return result;

    }

    // Creates a subdivided octahedron mesh. Again, passing sierpinski as true creates sierpinski triangles
    // See the comment for the subdivision algorithm for more information

    mesh subdivided_octahedron(float radius, int divisions, bool sierpinski)
    {
        glm::vec4 a = hypermath::exp0(radius * glm::vec4(0,0,1,0));
        glm::vec4 b = hypermath::exp0(radius * glm::vec4(0,1,0,0));
        glm::vec4 c = hypermath::exp0(radius * glm::vec4(0,-1,0,0));
        glm::vec4 d = hypermath::exp0(radius * glm::vec4(1,0,0,0));
        glm::vec4 e = hypermath::exp0(radius * glm::vec4(-1,0,0,0));
        glm::vec4 f = hypermath::exp0(radius * glm::vec4(0,0,-1,0));

        std::vector<glm::vec4> t1, t2, t3, t4, t5, t6, t7, t8, collected;

        //generate each subdivided triangle
        
        t1 = subdivide_triangle_vector(a, b, d, divisions, sierpinski);
        t2 = subdivide_triangle_vector(a, d, c, divisions, sierpinski);
        t3 = subdivide_triangle_vector(a, c, e, divisions, sierpinski);
        t4 = subdivide_triangle_vector(a, e, b, divisions, sierpinski);
        t5 = subdivide_triangle_vector(f, b, d, divisions, sierpinski);
        t6 = subdivide_triangle_vector(f, d, c, divisions, sierpinski);
        t7 = subdivide_triangle_vector(f, c, e, divisions, sierpinski);
        t8 = subdivide_triangle_vector(f, e, b, divisions, sierpinski);
        
        //push back each subdivision to the final vector.
        for(unsigned int i=0; i < t1.size(); i++)
        {
            collected.push_back(t1[i]);
        }
        //this format will be useful when we turn this octahedron into a sphere.
        for(unsigned int i=0; i < t2.size(); i++)
        {
            collected.push_back(t2[i]);
        }
        
        for(unsigned int i=0; i < t3.size(); i++)
        {
            collected.push_back(t3[i]);
        }
        
        for(unsigned int i=0; i < t4.size(); i++)
        {
            collected.push_back(t4[i]);
        }
        
        for(unsigned int i=0; i < t5.size(); i++)
        {
            collected.push_back(t5[i]);
        }
        
        for(unsigned int i=0; i < t6.size(); i++)
        {
            collected.push_back(t6[i]);
        }
        
        for(unsigned int i=0; i < t7.size(); i++)
        {
            collected.push_back(t7[i]);
        }
        
        for(unsigned int i=0; i < t8.size(); i++)
        {
            collected.push_back(t8[i]);
        }
        //setup colors
        std::vector<glm::vec4> colors;
        for(unsigned int i = 0; i < collected.size()/3; i++)
        {
            colors.push_back(red);
            colors.push_back(green);
            colors.push_back(blue);
        }
        //create mesh
        mesh result;
        result.vao = vao_from_pos_col(collected, colors);
        result.mode = GL_TRIANGLES;
        result.first = 0;
        result.count = collected.size();
        return result;
    }

}


