import glob
import os
glslFiles = glob.glob('src/shaders/*.glsl')

for file in glslFiles:
    (root, ext) = os.path.splitext(file)
    basename = os.path.basename(file)
    (basename, _) = os.path.splitext(basename)
    outputFile = root + '.h'
    input = open(file, 'r')
    output = open(outputFile, 'w')
    output.write('#ifndef ' + basename.upper() + '_SHADER\n'
      + '#define ' + basename.upper() + '_SHADER\n'
      + '\n'
      + 'char _binary_src_shaders_' + basename + '_glsl_start[] = '
      + '\"' + input.read().replace('\'', '\\\'').replace('\\', '\\\\').replace('\n', '\\n') + '\";\n'
      + '\n'
      + '#endif')
