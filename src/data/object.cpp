
#include "object.h"
#include "glm/glm.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <cmath>
#include "../math/primitives.h"
#include "../math/hypermath.h"
#include "../logmanager/logmanager.h"

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
object::object(){

}
/** \brief Writes the object to a file in /resources/filename.hr
 *  \param The filename (DOES NOT ALLOW DUPLICATE FILES TO BE CREATED)
 *  \return void
 */
bool object::write_file(const std::string filename){

    // If the mode isn't GL_TRIANGLES, return false and warn user.
    if(this->meshes.at(0).mode != GL_TRIANGLES){
        LogManager::log_warning("The file you attempted to write " + filename +
        "was using a mode other than GL_TRIANGLES. This can be problematic.", 1);
        return false;
    }
    // If the filename already exists, return false and warn user.
    else if(file_exists(filename)) {
        LogManager::log_warning(filename + " already exists, please give a different name", 1);
        return false;
    }
    // Otherwise we continue, provided in_ram is true on the mesh and there is some data
    else if(this->meshes.at(0).in_ram && this->meshes.at(0).data != nullptr){
        FILE* file;
        std::string name = "resources/" + filename + ".hr";

        /* Write the file header */
        std::ofstream header(name.c_str(), std::ofstream::app);
        header << "hr00pcn_";
        header.close();

        /* OH GOD THE HORROR! (Opening the file in an if statement) */
        if((file = std::fopen(name.c_str(), "ab")) == NULL){
            LogManager::log_warning("Cannot open file\n.", 1);
            return false;
        }

        /* MADNESS! (Writing the file in an if statement) */
        if(std::fwrite(this->meshes.at(0).data, sizeof(float), this->meshes.at(0).data_size, file) != this->meshes.at(0).data_size) {
        LogManager::log_warning("File write error.", 1);
        return false;
        }
        std::fclose(file);
        LogManager::log_info("Successfully wrote " + filename + " to file!", 2);
        return true;
    }
    // If all else fails, throw an error and return false.
    else{
        LogManager::log_warning("Object unable to be written, data is not stored in memory.", 1);
        return false;
    }
}

/** \brief Private function to check if the file exists
 * \param filename as a string
 * \return Returns boolean for whether or not the file exists
 */
bool object::file_exists(const std::string& name) {
    std::string n = "resources/" + name + ".hr";
    std::ifstream f(n.c_str());
    if (f.good()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }
}
