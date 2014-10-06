#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <iomanip>
#include <cmath>
#include "../../thirdparty/glm/glm/glm.hpp"
#include "../hypermath.h"

int main(int argc, const char* argv[])
{
  if (argc < 3) {
    printf("Please specify a file to convert and a file to write to\n");
    return 1;
  }
  else {
    const char* filename = argv[1];
    const char* writeTo = argv[2];
    
    double scale = 1.0;

    int precision = 10;

    if (argc > 3) {
      scale = std::stod(argv[3]);
    }

    if (argc > 4) {
      precision = std::stoi(argv[4]);
    }

    std::ifstream input(filename);
    std::ofstream output(writeTo);

    for(std::string line; getline(input, line); ) {
      if (line[0] == 'v') {
        double x, y, z;
        size_t index;

        line = line.substr(1);

        x = std::stod(line, &index) * scale;
        line = line.substr(index);

        y = std::stod(line, &index) * scale;
        line = line.substr(index);

        z = std::stod(line) * scale;

        glm::vec4 initial = glm::vec4(x, y, z, 0);

        glm::vec4 converted = hypermath::exp0(initial);

        output << "v " << std::fixed << std::setprecision(precision) << converted.x << " " 
            << std::fixed << std::setprecision(precision) << converted.y << " " 
            << std::fixed << std::setprecision(precision) << converted.z << " " 
            << std::fixed << std::setprecision(precision) << converted.w << "\n";
      }
      else {
        output << line << "\n";
      }
    }

    output.close();
  }
}