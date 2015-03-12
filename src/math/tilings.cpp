#include <GL/glew.h>
#include "glm/glm.hpp"
#include "hypermath.h"
#include "../data/mesh.h"
#include <vector>

namespace{
    //ccw must pass size with array, because arrays are passed as pointers
    std::vector<glm::dvec4> ngon(glm::dvec4 vertices[], int size){
        std::vector<glm::dvec4> gon;
        int first_third = (int) floor(size/3);
        int second_third = first_third * 2;
        glm::dvec4 mid_thirds = hypermath::midpoint(vertices[first_third], vertices[second_third], 0.5);
        glm::dvec4 center = hypermath::midpoint(vertices[0], mid_thirds, 0.5);
        gon.push_back(center);
        for(int i=0; i < size; i++){
            gon.push_back(vertices[i]);
        }
        return gon;
    }

    //make the first round of neighbors

    /****************TODO: ALL AFTER THIS ***************/
    
    std::vector<glm::dvec4> generate_first_neighbors(std::vector<glm::dvec4> gen0){
        std::vector<glm::dvec4> neighbors;
        for(int i=0; i < gen0.size()-1; i++){
            neighbors.push_back(hypermath::reflect_planar_point(gen0[0], gen0[1], gen0[2]));
        }
        return neighbors;
    }

    //make the next round of neighbors, given we pass the previous two
    std::vector<glm::dvec4> generate_neighbors(std::vector<glm::dvec4> one_back, std::vector<glm::dvec4> two_back){
        std::vector<glm::dvec4> neighbors;
        return neighbors;
    }

    //turn our vector of vectors into a vector
    std::vector<glm::dvec4> post_process(std::vector<std::vector<glm::dvec4>> gens, const int sides){
        std::vector<glm::dvec4> clean;
        return clean;
    }
    //generate our mesh *****CHANGE VOID TO MESH
    void generate_mesh(std::vector<std::vector<glm::dvec4>> gens, const int sides){
        std::vector<glm::dvec4> clean = post_process(gens, sides);
        //todo: add code to turn clean into a vao and make the mesh
        return;
    }
}
namespace tilings{

    //main tile generation function
    mesh generate_tiling(const int sides, const double radius,  const int around_vertex, const int iterations){
        std::vector<std::vector<glm::dvec4>> generations;
        std::vector<glm::dvec4> gen0;

        //Generate central ngon
        const double PI = 3.141592653589793238463;
        const double radians_between_vertices = 2*PI/sides;
        for(int i = 0; i <= sides; i++){
            double a = sin(i * radians_between_vertices);
            double b = cos(i * radians_between_vertices);
            gen0.push_back(hypermath::exp0(radius * glm::dvec4(a, 0, b, 0)));
        }
        generations[0] = gen0;
        //generate first round of neighbors to ensure that we can reach back two steps
        //in order to ensure against duplicates
        generations[1] = generate_first_neighbors(generations[0]);
        for(int i=2; i <= iterations; i++){
            generations[i] = generate_neighbors(generations[i-1], generations[i-2]);
        }
        //generate mesh from vector of triangles.
       // mesh tile = gen_mesh(generations, sides);
    }
}