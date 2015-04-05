#include <GL/glew.h>
#include "glm/glm.hpp"
#include "hypermath.h"
#include "../data/mesh.h"
#include <vector>
#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include "primitives.h"

namespace{
    //convert the first ngon from using GL_TRIANGLE_FAN to GL_TRIANGLES
    std::vector<glm::dvec4> fan_to_triangle(std::vector<glm::dvec4> input){
        std::vector<glm::dvec4> out;
        for(int i=1; i < input.size()-1; i++){
            out.push_back(input[0]);
            out.push_back(input[i]);
            out.push_back(input[i+1]);
        }
        return out;
    }
    //make the first round of neighbors
    //(Reflect the ngon about each of it's sides)
    //Assumes gen0 is passed and is setup to use GL_TRIANGLES
    std::vector<glm::dvec4> generate_first_neighbors(std::vector<glm::dvec4> gen0){
        std::vector<glm::dvec4> neighbors;
        for(int j=1; j < gen0.size(); j++){
            for(int i=0; i < gen0.size(); i++){
                if(j%3 != 0 && (j+1)%3!=0){
                    glm::dvec4 newpt = hypermath::reflect_planar_point(gen0[i], gen0[j], gen0[j+1]);
                    neighbors.push_back(newpt);
                }
            }
        }
        return neighbors;
    }

    //turn our vector of vectors into a vector
    std::vector<glm::dvec4> post_process(std::vector<std::vector<glm::dvec4>> gens, const int sides){
        std::vector<glm::dvec4> clean;
        for(int i=0; i < gens.size(); i++){
            clean.insert(clean.end(), gens[i].begin(), gens[i].end());
        }
        return clean;
    }

    bool has(glm::dvec4 center, std::vector<glm::dvec4> two_back, double closeness){
        for(int i=0; i<two_back.size(); i++){
            if(hypermath::dist(center, two_back[i]) < closeness){
                return true;
            }
        }
        return false;
    }

    // This function generates each subsequent round of neighbors. NOTE: this really needs to use the broken "has" function.
    std::vector<glm::dvec4> generate_neighbors(std::vector<glm::dvec4> one_back, std::vector<glm::dvec4> two_back, int sides){
        std::vector<glm::dvec4> neighbors;
        //Step through the start of each ngon (Correct)
        for(int i=0; i < one_back.size(); i+=3*sides){
            //step through each of the sides (Correct)
            for(int k = i; k < i + 3*sides; k++){
                //Step through each vertex in the 'gon' THIS IS PROBABLY THE LINE WITH THE PROB!!
                for(int j = i; j < i + 3*sides; j++){
                    if(k % 3 !=0 && (k+1) % 3 != 0){
                        neighbors.push_back(hypermath::reflect_planar_point(one_back[j], one_back[k], one_back[k+1]));
                    }
                }
            }
        }
        return neighbors;
    }
}
namespace tilings{

    //main tile generation function
    mesh generate_tiling(const int sides, const double radius, const int iterations){
        std::vector<std::vector<glm::dvec4>> generations;
        std::vector<glm::dvec4> gen0;

        //Generate central ngon
        const double PI = 3.141592653589793238463;
        const double radians_between_vertices = 2*PI/sides;
        gen0.push_back(hypermath::exp0(glm::dvec4(0.0,0.0,0.0,0.0)));
        for(int i = 0; i <= sides; i++){
            double a = sin(i * radians_between_vertices);
            double b = cos(i * radians_between_vertices);
            gen0.push_back(hypermath::exp0(radius * glm::dvec4(a, 0, b, 0)));
        }
        generations.push_back(fan_to_triangle(gen0));
        //generate first round of neighbors to ensure that we can reach back two steps
        //in order to ensure against duplicates
        if(iterations > 0){
            generations.push_back(generate_first_neighbors(generations[0]));
        }
        for(int i=2; i <= iterations; i++){
            generations.push_back(generate_neighbors(generations.at(i-1), generations.at(i-2), sides));
        }
        std::vector<glm::dvec4> processed = post_process(generations, sides);
        //generate mesh from vector of triangles.
        return primitives::object(processed);
    }
}