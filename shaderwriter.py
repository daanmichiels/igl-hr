glslFiles = {'fragment', 'vertex'}
for file in glslFiles:
  inputFile = 'src/shaders/' + file + '.glsl'
  outputFile = 'src/shaders/' + file + '.h'
  input = open(inputFile, 'r')
  output = open(outputFile, 'w')
  output.write('#ifndef ' + file.upper() + '_SHADER\n'
    + '#define ' + file.upper() + '_SHADER\n'
    + '\n'
    + 'char _binary_src_shaders_' + file + '_glsl_start[] = '
    + '\"' + input.read().replace('\'', '\\\'').replace('\\', '\\\\').replace('\n', '\\n') + '\";\n'
    + '\n'
    + '#endif')