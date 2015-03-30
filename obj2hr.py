#"c:/users/byron/programming/py_4_notepad.bat" "c:/users/byron/oculus rift project/igl-hr/" obj2hr.py "square-p-1.obj 2" <- ignore this
#'''
#this script takes in an obj file name from the command line
#and turns the information into hyperbolic VAO information
#example usage>> python obj2hr.py suzy.obj .4
import sys;
import math;
#import struct;
from array import array;

if len(sys.argv) == 1:
	sys.exit("No file given.");
	
#math calculation of exp0 done here
def exp0(dir):
	basepoint = [0,0,0,1];
	dot = sum([float(x)**2 for x in dir]); #calculates inner product of direction
	r = dot**(.5); #length of direction
	if r == 0:
		return basepoint;
		
	#equivalent to cosh(r)*basepoint + sinh(r)/r * direction
	return [ m+n for m,n in zip([i*math.cosh(r) for i in basepoint], [j*(math.sinh(r)/r) for j in dir]) ];
#--

filename = sys.argv[1]; #get filename from first cmd argument
scale = float(sys.argv[2]) if len(sys.argv) > 2 else 1; #get scale from second cmd argument or use 1 if absent
savename = filename[:-4]+".hr";
obj = open("resources/"+filename,"r");
verts = [];
colors = [];
norms = [];
floats = array('f');
k=0;

""" This should go in the for loop below and process the vertex normals
elif line[:2] == "vn":
	parts = line.split(" ");
	norms.push( [ float(parts[1]), float(parts[2]), float(parts[3]) ] );
#"""
	

for line in obj:
	if line[:2] == "vc": #process the vertex color by adding it to the list of colors
		parts = line.split(" ");
		colors.push( [ float(parts[1]), float(parts[2]), float(parts[3]) ] );
	elif line[:2] == "v ":
		parts = line.split(" "); #get the coordinates
		x=float(parts[1]);
		y=float(parts[2]);
		z=float(parts[3]);
		vec = exp0( [i*scale for i in [x,y,z,0] ] ); #calculates the scaled hyperbolic coordinate
		if len(colors) == 0:
			vec.append(1.0 if (k%3)==0 else 0.0);
			vec.append(1.0 if (k%3)==1 else 0.0);
			vec.append(1.0 if (k%3)==2 else 0.0);
			vec.append(1.0);
			k+=1;
		verts.append(vec); #store this vertex
	elif line[0] == "f":
		if len(verts) == 0: #kill the script if no verticies are given
			obj.close();
			sys.exit('No verticies given.');
		face = line.split(" "); #get the verticies
		face.pop(0);
		# "x/x/x" <--- structure of face[i] at this point
		# ["x/x/x", "x/x/x", "x/x/x"]
		face = [ fa=fa.split("/") for fa in face ];
		# [["x","x","x"], ["x","x","x"], ["x","x","x"]] <--- structure of face here.
		#face = [ int(f)-1 if f!="" else None for f in face ];
		face = [ int(f)-1 if f!="" else None for f in [fa for fa in face]];
		"""
		for fa in face:
			for f in fa:
				if f != "":
					f = int(f)-1;
		"""
		# [[x-1,x-1,x-1], [x-1,x-1,x-1], [x-1,x-1,x-1]] <--- structure of face here.
		for f in face:
			for v in verts[f[0]]:
				floats.append(v);
			if len(colors)>0:
				for c in colors[f[1]]:
					floats.append(c);
			if len(norms)>0:
				for n in norms[f[2]]:
					floats.append(n);
					
			
#s = struct.pack('f'*len(floats), *floats)
hr = open("resources/"+savename,"wb");
floats.tofile(hr);
obj.close();
hr.close();
#'''