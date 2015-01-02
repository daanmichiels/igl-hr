
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

void object::transform(glm::dmat4 transformation)
{
    this->transformation = transformation * this->transformation;
}

void object::set_transformation(glm::dmat4 transformation)
{
    this->transformation = transformation;
}

object::object(mesh object_mesh)
{
    this->meshes.push_back(object_mesh);
}

object::object()
{

}
