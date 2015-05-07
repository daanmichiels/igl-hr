import sys
import os
import math
import numpy
from array import array
from plyfile import PlyData, PlyElement

# ----------- HYPERMATH ------------

ORIGIN = numpy.array([0,0,0,1])

def dot(v, w):
	return v[0]*w[0] + v[1]*w[1] + v[2]*w[2] - v[3]*w[3]

def length(v):
	return math.sqrt(dot(v,v))

def exp(pos, dir):
	r = length(dir)
	if r==0:
		return pos
	return math.cosh(r) * pos + (math.sinh(r)/r) * dir
	
def exp0(dir):
	return exp(ORIGIN, dir)
	
def dexp0(v, n):
	x = v[0]
	y = v[1]
	z = v[2]
	r = math.sqrt(x**2 + y**2 + z**2)
	output = numpy.matrix(
			[[   (r*(x**2)*math.cosh(r)+(y**2+z**2)*math.sinh(r))/(r**3)    ,   x*y*(((math.cosh(r))/(r**2))-((math.sinh(r))/(r**3)))     ,   x*z*(((math.cosh(r))/(r**2))-((math.sinh(r))/(r**3)))     ,   0   ], 
			 [   x*y*(((math.cosh(r))/(r**2))-((math.sinh(r))/(r**3)))      ,   (r*(y**2)*math.cosh(r)+(x**2+z**2)*math.sinh(r))/(r**3)   ,   y*z*(((math.cosh(r))/(r**2))-((math.sinh(r))/(r**3)))     ,   0   ], 
			 [   x*z*(((math.cosh(r))/(r**2))-((math.sinh(r))/(r**3)))      ,   y*z*(((math.cosh(r))/(r**2))-((math.sinh(r))/(r**3)))     ,   (r*(z**2)*math.cosh(r)+(x**2+y**2)*math.sinh(r))/(r**3)   ,   0   ], 
			 [   (x*math.sinh(r))/(r)                                       ,   (y*math.sinh(r))/(r)                                      ,   (z*math.sinh(r))/(r)                                      ,   0   ]]
	)
	a = numpy.matrix(n).reshape((4,1))
	result = output * a
	result = numpy.array(result).ravel()
	return result
	
# ----------------------------------

if len(sys.argv) < 2:
    print("No filename given.")
    print("Usage: python ply2hr.py <filename> [scale]")
    sys.exit(0)

filename = os.path.abspath(sys.argv[1])
scale = 1.0
if len(sys.argv) > 2:
    scale = float(sys.argv[2])
savefile = os.path.splitext(filename)[0] + ".hr"

print("Source file: " + filename)
print("Output file: " + savefile)
print("Scale: " + str(scale))

# Parse the file
print("")
plydata = PlyData.read(filename)

# Print some information
nrfaces = len(plydata['face'].data)
print("Face count: " + str(nrfaces))

nrverts = len(plydata['vertex'].data)
print("Vert count: " + str(nrverts))

# Detect which vertex properties are present in the .ply-file
has_pos = False
has_col = False
has_nor = False
for prop in plydata.elements[0].properties:
    if prop.name == "x":
        has_pos = True
        print("Vertex positions in file.")
    if prop.name == "red":
        has_col = True
        print("Vertex colors in file.")
    if prop.name == "nx":
        has_nor = True
        print("Vertex normals in file.")

# This function will return an array of floats
# containing all the vertex data
# given the vertex' index
def process_vertex(j):
    processed_vertex = []
    vdata = plydata['vertex'].data[j]
    if has_pos:
        pos = numpy.array([vdata['x'], vdata['y'], vdata['z'], 0.0])
        pos = scale * pos
        pos = exp0(pos)
        processed_vertex.extend(pos)
    if has_col:
        col = numpy.array([vdata['red']/255.0, vdata['green']/255.0, vdata['blue']/255.0, 1.0])
        processed_vertex.extend(col)
    if has_nor:
        nor = numpy.array([vdata['nx'], vdata['ny'], vdata['nz'], 0.0])
        #TODO: apply dexp0
        processed_vertex.extend(nor)
    return processed_vertex

# Now fill up a buffer with all the data we want in the output file
data = array('f', [])
for i in range(nrfaces):
    thisface = plydata['face'].data[i]
    vertexindices = thisface[0]
    if len(vertexindices) != 3:
        print("The model has to be triangulated.")
        sys.exit(0)
    for j in range(3):
        data.extend(process_vertex(vertexindices[j]))

# Write to the file
# We start with a short header
hr = open(savefile, "wb")
hr.write("hr00")
hr.write("p" if has_pos else "_")
hr.write("c" if has_col else "_")
hr.write("n" if has_nor else "_")
hr.write("_") #could be used for an extra attribute, like texture coordinates

# Now the data
data.tofile(hr)

# Done
print("")
print(str(8 + data.itemsize * len(data)) + " bytes written.")

