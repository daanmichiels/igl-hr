#"c:/users/byron/programming/py_4_notepad.bat" "c:/users/byron/oculus rift project/igl-hr/" obj2hr.py
#'''
#this script takes in an obj file name from the command line
#and turns the information into 
import math
def exp0(dir):
	basepoint = [0,0,0,1];
	dot = sum([x**2 for x in dir]);
	r = dot**(.5);
	if r == 0:
		return basepoint;
	
	return [ m+n for m,n in zip([i*math.cosh(r) for i in basepoint], [j*(math.sinh(r)/r) for j in dir]) ];

obj = open("resources/suzy.obj","r");
verts = [];
text = "";

for line in obj:
	if line[0] == "v":
		parts = line.split(" ");
		x=float(parts[1]);
		y=float(parts[2]);
		z=float(parts[3]);
		vec=exp0([x,y,z,0]);
		x=vec[0];
		y=vec[1];
		z=vec[2];
		w=vec[3];
		verts.append("("+str(x)+","+str(y)+","+str(z)+","+str(w)+")");
	elif line[0] == "f":
		print(len(verts));
		parts = line.split(" ");
		p1 = int(parts[1])-1;
		p2 = int(parts[2])-1;
		p3 = int(parts[3])-1;
		if len(text) > 0:
			text += ","+verts[p1]+","+verts[p2]+","+verts[p3];
		else:
			text += verts[p1]+","+verts[p2]+","+verts[p3];
			
hr = open("resources/suzy.hr","w");
hr.write(text);
obj.close();
hr.close();
#'''
