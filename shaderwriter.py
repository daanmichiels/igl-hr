import glob
import os

print("shaderwriter starting")

glslFiles = glob.glob('src/shaders/*.glsl')
output = open('src/shaders/shaders.h', 'w')
output.write('#ifndef SHADERS_H\n')
output.write('#define SHADERS_H\n\n')
output.write('#include <string>\n')
output.write('#include <map>\n\n')
output.write('std::map<std::string, std::string> shader_sources = {\n')

for file in glslFiles:
    (root, ext) = os.path.splitext(file)
    basename = os.path.basename(file)
    (basename, _) = os.path.splitext(basename)
    input = open(file, 'r')
    output.write('{\"' + basename + '\", \"'
      + input.read().replace('\'', '\\\'').replace('\\', '\\\\').replace('\n', '\\n') + '\"},'
      + '\n')

output.write('};\n\n#endif\n')

print("shaderwriter done")
