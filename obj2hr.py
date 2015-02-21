#"c:/users/byron/programming/py_4_notepad.bat" "c:/users/byron/oculus rift project/igl-hr/" obj2hr.py
#'''
import math
def exp0(dir):
	basepoint = [0,0,0,1];
	dot = sum([x**2 for x in dir]);
	r = dot**(.5);
	if r == 0:
		return basepoint;
	
	return [ m+n for m,n in zip([i*math.cosh(r) for i in basepoint], [j*(math.sinh(r)/r) for j in dir]) ];
obj = open("resources/suzy.obj","r");

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
		if len(text) > 0:
			text += ",("+str(x)+","+str(y)+","+str(z)+","+str(w)+")";
		else:
			text += "("+str(x)+","+str(y)+","+str(z)+","+str(w)+")";
			
hr = open("resources/suzy.hr","w");
hr.write(text);
obj.close();
hr.close();
#'''
