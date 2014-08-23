
#include "object.h"
#include "../thirdparty/glm/glm/glm.hpp"

void object::transform(glm::mat4 transformation)
{
    this->transformation = transformation * this->transformation;
}


