#include "../src/math/hypermath.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, const char* argv[]) {
    if(argc < 2) {
        std::cout << "Please provide a filename." << std::endl;
		return 0;
    }
	std::string filename = argv[1];

	std::ifstream s;
	s.open(filename, std::ios_base::in);
	if(!s) {
		std::cout << "Couldn't open " << filename << "." << std::endl;
	}

    std::vector<glm::vec4> positions;
    std::vector<size_t> indices;
	// TODO: parse the file and layout the vertex data in memory
	// (we want to use indexed rendering)
    
    for(std::string line; getline(s, line); ) {
        if(line.size() < 2) {
            continue;
        }

        if(line[0] == 'v' && line[1] == ' ') {
            line = line.substr(2);
            double x, y, z;
        }
    }

	// TODO: write the vertex data to a file

    return 0;
}

