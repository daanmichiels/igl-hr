#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <cmath>
#include "../../thirdparty/glm/glm/glm.hpp"
#include "../hypermath.h"

int main(int argc, const char* argv[])
{
  if (argc < 2) {
    printf("Please specify a file to convert and a file to write to\n");
    return 1;
  }
  else {
    const char* filename = argv[1];
    const char* writeTo = argv[2];

    std::ifstream input(filename);
    std::ofstream output(writeTo);

    for(std::string line; getline(input, line); )
    {
      if (line[0] == 'v') {
        double x, y, z;
        size_t index;

        line = line.substr(1);

        x = std::stod(line, &index);
        line = line.substr(index);

        y = std::stod(line, &index);
        line = line.substr(index);

        z = std::stod(line);

        glm::vec4 initial = glm::vec4(x, y, z, 0);

        glm::vec4 converted = hypermath::exp0(initial);

        output << "v " << converted.x << " " << converted.y << " " << converted.z << " " 
            << converted.w << "\n";
      }
      else {
        output << line << "\n";
      }
    }

    output.close();
  }
}