
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

/** \brief Empty object constructor
 * \param void
 */
object::object()
{

}
