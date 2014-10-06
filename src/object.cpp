
#include "object.h"
#include "../thirdparty/glm/glm/glm.hpp"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <cmath>
#include "primitives.h"

void object::transform(glm::mat4 transformation)
{
    this->transformation = transformation * this->transformation;
}

object::object(const char filename[])
{
  std::ifstream input(filename);

  std::vector<glm::vec4> vertices;

  int numVertices = 0;

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
      line = line.substr(index);

      w = std::stod(line);

      vertices.push_back(glm::vec4(x, y, z, w));
      numVertices++;
    }
    else if (line[0] == 'f') {
      int a, b, c;
      size_t index;

      line = line.substr(1);

      a = std::stoi(line, &index) - 1;
      line = line.substr(index);

      b = std::stoi(line, &index) - 1;
      line = line.substr(index);

      c = std::stoi(line) - 1;

      meshes.push_back(primitives::triangle(vertices[a], vertices[b], vertices[c]));
    }
  }
}

object::object()
{

}
