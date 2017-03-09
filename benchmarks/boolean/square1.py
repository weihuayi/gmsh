from gmshpy import *
lc = 0.5
GmshSetOption('Mesh', 'CharacteristicLengthFactor', lc)
 
g = GModel()

v1 = g.addVertex(0, 0, 0, lc)
v2 = g.addVertex(1, 0, 0, lc)
v3 = g.addVertex(1, 1, 0, lc)
v4 = g.addVertex(0, 1, 0, lc)
e1 = g.addLine(v2, v1)
e2 = g.addLine(v3, v2)
e3 = g.addLine(v4, v3)
e4 = g.addLine(v4, v1)
v11 = g.addVertex(.4, .4, 0, lc)
v12 = g.addVertex(.6, .4, 0, lc)
v13 = g.addVertex(.6, .5, 0, lc)
v14 = g.addVertex(.4, .6, 0, lc)
e11 = g.addLine(v11, v12)
e12 = g.addLine(v12, v13)
e13 = g.addLine(v13, v14)
e14 = g.addLine(v14, v11)

f = g.addPlanarFace ([[e1,e2,e3,e4],[e11,e12,e13,e14]])

g.mesh(2)
g.save("square1.msh")

#v100 = g.addVertex(0, 0, 0, .1)
#v200 = g.addVertex(0, 0, 1, .1)
#v300 = g.addVertex(0, .1, .33, .1)
#v400 = g.addVertex(.1, .1, .66, .1)
#line = g.addBezier(v100,v200,{{v300:x(),v300:y(),v300:z()},{v400:x(),v400:y(),v400:z()}});
#g.addPipe (f, {line})
#g.glue(1.e-9);

#myTool = GModel();
#myTool:addSphere(0.2,0.2,0.1,.52012);

#g.addSphere(1,1.3,1,.3);
#g.computeDifference(myTool,0);

#g.setAsCurrent();