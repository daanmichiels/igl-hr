
#include <iostream>
#include <string>

// Shaders will be linked to these
extern char source_vs_start[];
extern char source_vs_end[];
extern char source_vs_size[];
extern char source_fs_start[];
extern char source_fs_end[];
extern char source_fs_size[];

// Just testing whether the shaders get embedded correctly
void print_shader_source() {
	std::string source_vs(source_vs_start, (int)source_vs_size);
	std::string source_fs(source_fs_start, (int)source_fs_size);

	std::cout << "------- VERTEX SHADER -------\n";
	std::cout << source_vs << "\n";

	std::cout << "\n\n------ FRAGMENT SHADER ------\n";
	std::cout << source_fs << "\n";
}

