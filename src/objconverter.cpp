#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <cmath>

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
    std::ofstream output("out.obj");

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

        output << "v " << x << " " << y << " " << z << " " 
            << sqrt(1 + (x * x) + (y * y) + (z * z)) << "\n";
      }
      else {
        output << line;
      }
    }

    output.close();
  }
}