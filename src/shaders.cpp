
#include <iostream>

extern char source_vs_start[];
extern char source_vs_end[];
extern char source_fs_start[];
extern char source_fs_end[];

void print_shader_source() {
	std::cout << "------- VERTEX SHADER -------\n";
	for(char* p = source_vs_start; p < source_vs_end; p++)
	{
		std::cout << *p;
	}
	std::cout << "\n\n------ FRAGMENT SHADER ------\n";
	for(char* p = source_fs_start; p < source_fs_end; p++)
	{
		std::cout << *p;
	}
}