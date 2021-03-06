
#include "object.h"
#include "glm/glm.hpp"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <cmath>
#include "../math/primitives.h"
#include "../math/hypermath.h"

/** \brief Transform an object by a given 4x4 double transformation matrix
 * \param 4x4 double transformation matrix
 * \return void
 */
void object::transform(glm::dmat4 transformation)
{
    this->transformation = transformation * this->transformation;
}

/** \brief Set the transformation matrix of an object to a given 4x4 double transformation matrix.
 * \param 4x4 double transformation matrix.
 * \return void
 */
void object::set_transformation(glm::dmat4 transformation)
{
    this->transformation = transformation;
}

/** \brief Object constructor from a given mesh.
 * \param Mesh for the object
 */
object::object(mesh object_mesh)
{
    this->meshes.push_back(object_mesh);
}

object::object(const char filename[], bool converted, double scale)
{
    std::ifstream input(filename);

    std::vector<glm::dvec4> vertices;

    std::vector<glm::dvec4> v;

    for(std::string line; getline(input, line); ) {
        if (line[0] == 'v') {
            double x, y, z, w;
            size_t index;

            line = line.substr(1);

            x = std::stod(line, &index);
            line = line.substr(index);

            y = std::stod(line, &index);
            line = line.substr(index);

            z = std::stod(line);

            if (converted) {
                line = line.substr(index);
                w = std::stod(line);
            }
            else {
                glm::dvec4 conv = hypermath::exp0(glm::dvec4(x * scale, y * scale, z * scale, 0));

                x = conv.x;
                y = conv.y;
                z = conv.z;
                w = conv.w;
            }

            vertices.push_back(glm::dvec4(x, y, z, w));
        }
        else if (line[0] == 'f') {
            std::stringstream linestream(line.substr(1));

            while(1) {
                int i;

                linestream >> i;
                if (!linestream) {
                    break;
                }

                v.push_back(vertices[i - 1]);
            }
        }
    }

    meshes.push_back(primitives::object(v));
}

/** \brief Empty object constructor
 * \param void
 */
object::object()
{

}
