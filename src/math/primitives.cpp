#include "glm/glm.hpp"
#include <vector>
#include "glm/gtx/string_cast.hpp"
#include <iostream>
#include "primitives.h"
#include "hypermath.h"

/*! \file Primitives */
/** Anonymous namespace.
 * Everything in here can only be accessed from this file (unit).
 */
namespace
{
    const glm::dvec4 red = glm::dvec4(1.0,0.0,0.0,1.0);
    const glm::dvec4 green = glm::dvec4(0.0,1.0,0.0,1.0);
    const glm::dvec4 blue = glm::dvec4(0.0,0.0,1.0,1.0);
    const glm::dvec4 yellow = glm::dvec4(1.0,1.0,0.0,1.0);

    /** \brief Create a vao from positions and colors.
     * \param A vector of position vectors
     * \param A vector of color vectors
     * \return GLuint
     */
    GLuint vao_from_pos_col(std::vector<glm::dvec4> pos, std::vector<glm::dvec4> col)
    {
        size_t n = pos.size();

        /*! should be satisfied */
        assert(col.size() == n);

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        float* data = new float[8*n];
        for(size_t i=0; i<n; i++) {
            data[8*i+0] = (float) pos[i].x;
            data[8*i+1] = (float) pos[i].y;
            data[8*i+2] = (float) pos[i].z;
            data[8*i+3] = (float) pos[i].w;
            data[8*i+4] = (float) col[i].r;
            data[8*i+5] = (float) col[i].g;
            data[8*i+6] = (float) col[i].b;
            data[8*i+7] = (float) col[i].a;
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

    /** Subdivision Algorithm
     * \brief This works by finding the mid point between each pair of vertices, and generating 4 triangles.

     *                    a
     *                    *
     *                   * *
     *                  *   *
     * midpoint a,b    * * * *  midpoint a,c
     *                *       *
     *               * *     * *
     *              *   *   *   *
     *           b * * * * * * * * c 
     *               midpoint b,c

     * Passing true as the last argument discards the "middle" each step, resulting in sierpinski subdivision
     * Increasing the number of divisions from 1 to 2 will perform the first division, then divide all of the newly generated triangles.
     * Can crash the program for too large of a large number of divisions.
     * 
     * \param Three vectors for the initial triangle
     * \param Number of divisions (WARNING GETS UNSTABLE FOR HIGH VALUES >7)
     * \param A Bboolean for whether or not to discard middle
     * \return Vector of triangles which are subdivided. These can be assembled using GL_TRIANGLES
     */
    
    std::vector<glm::dvec4> subdivide_triangle_vector(glm::dvec4 a, glm::dvec4 b, glm::dvec4 c, int divisions, bool sierpinski)
    {

        /*! TODO: precalculate the sizes of the vectors */
        std::vector<glm::dvec4> triangles;
        glm::dvec4 ab, bc, ac;
        std::vector<glm::dvec4> t1, t2, t3, t4;

        ab = hypermath::midpoint(a, b, 0.5);
        ac = hypermath::midpoint(a, c, 0.5);
        bc = hypermath::midpoint(b, c, 0.5);

        /*! recursive calls to each subdivided triangle */
        if(divisions > 0)
        {
            t1 = subdivide_triangle_vector(a, ab, ac, divisions - 1, sierpinski);
            t2 = subdivide_triangle_vector(ab, b, bc, divisions -1, sierpinski);
            t3 = subdivide_triangle_vector(ac, bc, c, divisions -1, sierpinski);
            if(!sierpinski)
            {
                t4 = subdivide_triangle_vector(ab, ac, bc, divisions -1, sierpinski);
            }

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
        else
        {
            /*! base case */
            triangles.push_back(a);
            triangles.push_back(b);
            triangles.push_back(c);
            return triangles;
        }

    }
    std::vector<glm::dvec4> subdivide_triangle_vector_euclidean(glm::dvec4 a, glm::dvec4 b, glm::dvec4 c, int divisions, bool sierpinski)
    {
        /*! TODO: precalculate sizes */
        std::vector<glm::dvec4> triangles;
        std::vector<glm::dvec4> t1, t2, t3, t4;

        glm::dvec4 ab = a+b;
        glm::dvec4 ac = a+c;
        glm::dvec4 bc = b+c;
        ab *= 0.5;
        ac *= 0.5;
        bc *= 0.5;
        /*! recursive calls to each subdivided triangle */
        if(divisions > 0)
        {
            t1 = subdivide_triangle_vector_euclidean(a, ab, ac, divisions - 1, sierpinski);
            t2 = subdivide_triangle_vector_euclidean(ab, b, bc, divisions - 1, sierpinski);
            t3 = subdivide_triangle_vector_euclidean(ac, bc, c, divisions - 1, sierpinski);

            if(!sierpinski)
            {
                t4 = subdivide_triangle_vector_euclidean(ab, ac, bc, divisions -1, sierpinski);
            }

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
        else
        {
            /*! base case */
            triangles.push_back(a);
            triangles.push_back(b);
            triangles.push_back(c);
            return triangles;
        }

    }
}

namespace primitives
{
    /** \brief Create a line between two arbitrary points.
     *  the line is uniformly colored
     * \param Two position vectors (call exp0!)
     * \param A color vector
     * \return Line mesh
     */
    mesh line(glm::dvec4 a, glm::dvec4 b, glm::dvec4 col)
    {
        mesh result;
        result.vao = vao_from_pos_col({a,b}, {col,col});
        result.mode = GL_LINES;
        result.first = 0;
        result.count = 2;
        return result;
    }

    /** \brief Make a set of XYZ axes, colored red green and blue respectively, inside of a sphere of given radius
     * \param The radius for which to set the axes within
     * \return Axes mesh
     */
    mesh axes(double radius)
    {
        mesh result;
        std::vector<glm::dvec4> pos;
        std::vector<glm::dvec4> col;
        
        /*! first set the vectors for the axes. */
        pos.push_back(hypermath::exp0(radius * glm::dvec4(-1,0,0,0)));
        pos.push_back(hypermath::exp0(radius * glm::dvec4(1,0,0,0)));
        pos.push_back(hypermath::exp0(radius * glm::dvec4(0,-1,0,0)));
        pos.push_back(hypermath::exp0(radius * glm::dvec4(0,1,0,0)));
        pos.push_back(hypermath::exp0(radius * glm::dvec4(0,0,-1,0)));
        pos.push_back(hypermath::exp0(radius * glm::dvec4(0,0,1,0)));
        
        /*! set the colors for each corresponding axis. each line is uniformly colored */
        col.push_back(glm::dvec4(1.0f,0.0f,0.0f,1.0f));
        col.push_back(glm::dvec4(1.0f,0.0f,0.0f,1.0f));
        col.push_back(glm::dvec4(0.0f,0.0f,1.0f,1.0f));
        col.push_back(glm::dvec4(0.0f,0.0f,1.0f,1.0f));
        col.push_back(glm::dvec4(0.0f,1.0f,0.0f,1.0f));
        col.push_back(glm::dvec4(0.0f,1.0f,0.0f,1.0f));

        result.vao = vao_from_pos_col(pos, col);
        result.mode = GL_LINES;
        result.first = 0;
        result.count = 6;
        return result;
    }

    /** \brief Create a grid mesh. X axis is red, Y axis is green, and Z axis is blue
     * \param Spacing between the grid
     * \return Grid Mesh
     */
    mesh grid(double grid_space)
    {
        mesh result;
        /*! TODO: precalculate sizes */
        std::vector<glm::dvec4> pos;
        std::vector<glm::dvec4> col;
        int steps = (int) ceil(10/grid_space);
        glm::dvec4 x_pos(hypermath::exp0(glm::dvec4(10, 0, 0, 0)));
        glm::dvec4 x_neg(hypermath::exp0(glm::dvec4(-10, 0, 0, 0)));
        glm::dvec4 y_pos(hypermath::exp0(glm::dvec4(0, 10, 0, 0)));
        glm::dvec4 y_neg(hypermath::exp0(glm::dvec4(0, -10, 0, 0)));
        glm::dvec4 z_pos(hypermath::exp0(glm::dvec4(0, 0, 10, 0)));
        glm::dvec4 z_neg(hypermath::exp0(glm::dvec4(0, 0, -10, 0)));

        for(int j=0; j<= 2*steps; j++)
        {
            for(int i=0; i<= 2*steps; i++)
            {
                int j_prime = -1 * steps + j;
                int i_prime = -1 * steps + i;
                /*! X Grid */
                pos.push_back(x_pos * hypermath::translation0(hypermath::exp0(glm::dvec4(0, grid_space * j_prime, grid_space * i_prime, 0))));
                pos.push_back(x_neg * hypermath::translation0(hypermath::exp0(glm::dvec4(0, grid_space * j_prime, grid_space * i_prime, 0))));
                /*! Y Grid */
                pos.push_back(y_pos * hypermath::translation0(hypermath::exp0(glm::dvec4(grid_space*i_prime, -0, grid_space*j_prime, 0))));
                pos.push_back(y_neg * hypermath::translation0(hypermath::exp0(glm::dvec4(grid_space*i_prime, 0, grid_space*j_prime, 0))));
                /*! Z Grid */
                pos.push_back(z_pos * hypermath::translation0(hypermath::exp0(glm::dvec4(grid_space * j_prime, grid_space * i_prime, 0, 0))));
                pos.push_back(z_neg * hypermath::translation0(hypermath::exp0(glm::dvec4(grid_space * j_prime, grid_space * i_prime, 0, 0))));

                col.push_back(glm::dvec4(1.0f,0.0f,0.0f,1.0f));
                col.push_back(glm::dvec4(1.0f,0.0f,0.0f,1.0f));
                col.push_back(glm::dvec4(0.0f,0.0f,1.0f,1.0f));
                col.push_back(glm::dvec4(0.0f,0.0f,1.0f,1.0f));
                col.push_back(glm::dvec4(0.0f,1.0f,0.0f,1.0f));
                col.push_back(glm::dvec4(0.0f,1.0f,0.0f,1.0f));
            }
        }            
        
        result.vao = vao_from_pos_col(pos, col);
        result.mode = GL_LINES;
        result.first = 0;
        result.count = pos.size();
        return result;
    }
    
    /** \brief Create a triangle. The corners will be red, green, blue.
     * \param Three vectors for where to draw the triangle
     * \return Triangle mesh
     */
    mesh triangle(glm::dvec4 a, glm::dvec4 b, glm::dvec4 c)
    {
        mesh result;
        result.vao = vao_from_pos_col({a,b,c},{red,green,blue});
        result.mode = GL_TRIANGLES;
        result.first = 0;
        result.count = 3;
        return result;
    }
    
    /** \brief Create a rectangle which lies in the xz-plane, and is centered at the origin. It's uniformly colored. 
     * \param The width of the rectangle
     * \param The depth of the rectangle
     * \param The color of the rectangle
     * \return Rectangle mesh
     */
    mesh rectangle(double width, double depth, glm::dvec4 color)
    {
        double w = width / 2;
        double d = depth / 2;
        std::vector<glm::dvec4> pos = {hypermath::exp0(glm::dvec4(-w,0,d,0)), \
            hypermath::exp0(glm::dvec4( w,0, d,0)), \
            hypermath::exp0(glm::dvec4(-w,0,-d,0)), \
            hypermath::exp0(glm::dvec4( w,0,-d,0))};

        mesh result;
        result.vao = vao_from_pos_col(pos, {color,color,color,color});
        result.mode = GL_TRIANGLE_STRIP;
        result.first = 0;
        result.count = 4;
        return result;
    }
    /** \brief Create a n sided polygon inside a circle of given radius.
     * \param The number of sides to give the n-gon
     * \param The radius to inscribe the n-gon within
     * \param The color of the n-gon
     * \return n sided polygon inscribed in given radius.
     */
    mesh circumscribed_ngon(int n, double radius, glm::dvec4 color)
    {
        const double PI = 3.141592653589793238463;
        double radians_between_vertices = 2*PI/n;
        std::vector<glm::dvec4> vertices;
        std::vector<glm::dvec4> col;
        for(int i = 0; i <= n; i++)
        {
            double a = sin(i * radians_between_vertices);
            double b = cos(i * radians_between_vertices);
            vertices.push_back(hypermath::exp0(radius * glm::dvec4(a, 0, b, 0)));
            col.push_back(color);
        }
        mesh result;
        result.vao = vao_from_pos_col(vertices, col);
        result.mode = GL_TRIANGLE_FAN;
        result.first = 0;
        result.count = vertices.size();
        return result;
    }

    /** \brief Create a tetrahedron.
     * 
     *It will be inscribed in a sphere of the given radius.
     *It will be centered at (0,0,0,1) and one of its tips will
     *point in the positive y-direction (up).
     *It will have nice colors. 
     * 
     * \param The radius within which to inscribe the tetrahedron
     * \return Tetrahedron mesh
    */
    mesh tetrahedron(double radius)
    {
        glm::dvec4 a = radius * glm::dvec4(0,1,0,0);
        glm::dvec4 b = radius * glm::dvec4(-sqrt(2)/3,-1.0/3,-sqrt(2.0/3),0);
        glm::dvec4 c = radius * glm::dvec4(-sqrt(2)/3,-1.0/3, sqrt(2.0/3),0);
        glm::dvec4 d = radius * glm::dvec4(2*sqrt(2.0)/3,-1.0/3,0,0);

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
    /** \brief Creates an octahedron at the origin. Each triangle is colored with red, green, and blue vertices.
     * \param Radius within which to inscribe the octahedron
     * \return Octahedron mesh.
     */
    mesh octahedron(double radius)
    {
        glm::dvec4 a = hypermath::exp0(radius * glm::dvec4(0,0,1,0));
        glm::dvec4 b = hypermath::exp0(radius * glm::dvec4(0,1,0,0));
        glm::dvec4 c = hypermath::exp0(radius * glm::dvec4(0,-1,0,0));
        glm::dvec4 d = hypermath::exp0(radius * glm::dvec4(1,0,0,0));
        glm::dvec4 e = hypermath::exp0(radius * glm::dvec4(-1,0,0,0));
        glm::dvec4 f = hypermath::exp0(radius * glm::dvec4(0,0,-1,0));
        mesh result;
        std::vector<glm::dvec4> colors;
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

    mesh object(std::vector<glm::dvec4> v)
    {
        std::vector<glm::dvec4> colors;

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
    
    /** \brief Creates a subdivided triangle mesh, passing true to sierpinski makes a sierpinski subdivided triangle. 
     * See the comment for the subdivision algorithm for more information 
     * ***DO NOT MAKE DIVISIONS TOO HIGH (OVER 6)
     * 
     * \param Three vertices for the triangle
     * \param The number of divisions to make
     * \param A boolean whether or not to discard the middle
     * \return Subdivided Triangle Mesh
     */
    
    mesh subdivided_triangle(glm::dvec4 a, glm::dvec4 b, glm::dvec4 c, int divisions, bool sierpinski)
    {
        std::vector<glm::dvec4> triangle;
        triangle = subdivide_triangle_vector(a,b,c,divisions,sierpinski);
        std::vector<glm::dvec4> colors;
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

    /** \brief Creates a subdivided octahedron mesh. Again, passing sierpinski as true creates sierpinski triangles
     * See the comment for the subdivision algorithm for more information 
     * ***DO NOT MAKE DIVISIONS TOO HIGH (OVER 7)
     * 
     * \param Radius within which to inscribe the octahedron
     * \param Number of divisions to make
     * \param A boolean whether or not to discard the middle.
     * \return Subdivided Octahedron Mesh
     */

    mesh subdivided_octahedron(double radius, int divisions, bool sierpinski)
    {
        glm::dvec4 a = hypermath::exp0(radius * glm::dvec4(0,0,1,0));
        glm::dvec4 b = hypermath::exp0(radius * glm::dvec4(0,1,0,0));
        glm::dvec4 c = hypermath::exp0(radius * glm::dvec4(0,-1,0,0));
        glm::dvec4 d = hypermath::exp0(radius * glm::dvec4(1,0,0,0));
        glm::dvec4 e = hypermath::exp0(radius * glm::dvec4(-1,0,0,0));
        glm::dvec4 f = hypermath::exp0(radius * glm::dvec4(0,0,-1,0));

        std::vector<glm::dvec4> t1, t2, t3, t4, t5, t6, t7, t8, collected;

        /*! generate each subdivided triangle */
        
        t1 = subdivide_triangle_vector(a, b, d, divisions, sierpinski);
        t2 = subdivide_triangle_vector(a, d, c, divisions, sierpinski);
        t3 = subdivide_triangle_vector(a, c, e, divisions, sierpinski);
        t4 = subdivide_triangle_vector(a, e, b, divisions, sierpinski);
        t5 = subdivide_triangle_vector(f, b, d, divisions, sierpinski);
        t6 = subdivide_triangle_vector(f, d, c, divisions, sierpinski);
        t7 = subdivide_triangle_vector(f, c, e, divisions, sierpinski);
        t8 = subdivide_triangle_vector(f, e, b, divisions, sierpinski);

        /*! push back each subdivision to the final vector. */
        for(unsigned int i=0; i < t1.size(); i++)
        {
            collected.push_back(t1[i]);
        }
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

        std::vector<glm::dvec4> colors;
        for(unsigned int i = 0; i < collected.size()/3; i++)
        {
            colors.push_back(red);
            colors.push_back(green);
            colors.push_back(blue);
        }
        mesh result;
        result.vao = vao_from_pos_col(collected, colors);
        result.mode = GL_TRIANGLES;
        result.first = 0;
        result.count = collected.size();
        return result;
    }

    /** \brief This is made by making an octahedron, subdividing the octahedron, and normalizing all vectors.
     *    Then we normalize every vertex, scale it according to the passed radius value.
     *    Finally we exponentiate the resulting vector.
     *    ***DO NOT MAKE DIVISIONS TOO HIGH (OVER 7)
     * 
     * \param Radius of the sphere
     * \param Number of subdivisions to make
     * \param Color of the sphere
     * \param Boolean for whether to discard the middle.
     * \return Sphere Mesh
     */
    mesh sphere(double radius, int divisions, glm::dvec4 col, bool sierpinski)
    {
        glm::dvec4 a = glm::dvec4(0,0,1,0);
        glm::dvec4 b = glm::dvec4(0,1,0,0);
        glm::dvec4 c = glm::dvec4(0,-1,0,0);
        glm::dvec4 d = glm::dvec4(1,0,0,0);
        glm::dvec4 e = glm::dvec4(-1,0,0,0);
        glm::dvec4 f = glm::dvec4(0,0,-1,0);

        std::vector<glm::dvec4> t1, t2, t3, t4, t5, t6, t7, t8, collected;

        /*! generate each subdivided triangle */
        
        t1 = subdivide_triangle_vector_euclidean(a, b, d, divisions, sierpinski);
        t2 = subdivide_triangle_vector_euclidean(a, d, c, divisions, sierpinski);
        t3 = subdivide_triangle_vector_euclidean(a, c, e, divisions, sierpinski);
        t4 = subdivide_triangle_vector_euclidean(a, e, b, divisions, sierpinski);
        t5 = subdivide_triangle_vector_euclidean(f, b, d, divisions, sierpinski);
        t6 = subdivide_triangle_vector_euclidean(f, d, c, divisions, sierpinski);
        t7 = subdivide_triangle_vector_euclidean(f, c, e, divisions, sierpinski);
        t8 = subdivide_triangle_vector_euclidean(f, e, b, divisions, sierpinski);
        
        /*! push back each subdivision to the final vector. */
        for(unsigned int i=0; i < t1.size(); i++)
        {
            collected.push_back(t1[i]);
        }

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

        std::vector<glm::dvec4> normalized;
        for(unsigned int i=0; i < collected.size(); i++)
        {
            normalized.push_back(hypermath::exp0(radius * hypermath::normalize(collected[i])));
        }

        std::vector<glm::dvec4> colors;
        for(unsigned int i = 0; i < collected.size()/3; i++)
        {
            colors.push_back(yellow);
            colors.push_back(green);
            colors.push_back(blue);
        }
        mesh result;
        result.vao = vao_from_pos_col(normalized, colors);
        result.mode = GL_TRIANGLES;
        result.first = 0;
        result.count = normalized.size();
        return result;
    }
}
