#"c:/users/byron/programming/py_4_notepad.bat" "c:/users/byron/oculus rift project/igl-hr/" obj2hr.py "suzy.obj 2" <- ignore this
#'''
#this script takes in an obj file name from the command line
#and turns the information into hyperbolic VAO information
#example usage>> python obj2hr.py suzy.obj .4
import sys;
import math;

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

	

filename = sys.argv[1]; #get filename from first cmd argument
scale = float(sys.argv[2]) if len(sys.argv) > 2 else 1; #get scale from second cmd argument or use 1 if absent
obj = open("resources/"+filename,"r");
verts = [];
text = "";

for line in obj:
	if line[0] == "v":
		parts = line.split(" "); #get the coordinates
		x=float(parts[1]);
		y=float(parts[2]);
		z=float(parts[3]);
		vec = exp0( [i*scale for i in [x,y,z,0] ] ); #calculates the scaled hyperbolic coordinate
		x=vec[0];
		y=vec[1];
		z=vec[2];
		w=vec[3];
		verts.append("("+str(x)+","+str(y)+","+str(z)+","+str(w)+")"); #store this vertex
	elif line[0] == "f":
		parts = line.split(" "); #get the vertex indices
		p1 = int(parts[1])-1; #subtract one because obj syntax begins counting at one
		p2 = int(parts[2])-1;
		p3 = int(parts[3])-1;
		if len(text) > 0: #add vertex indices to final output
			text += ","+verts[p1]+","+verts[p2]+","+verts[p3];
		else:
			text += verts[p1]+","+verts[p2]+","+verts[p3];
			
hr = open("resources/suzy.hr","w");
hr.write(text);
obj.close();
hr.close();
#'''