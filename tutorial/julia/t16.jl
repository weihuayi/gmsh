# This file reimplements gmsh/tutorial/t16.geo in Julia.

import gmsh

model = gmsh.model
factory = model.occ

gmsh.initialize()
gmsh.option.setNumber("General.Terminal", 1)

model.add("t16")

factory.addBox(0,0,0, 1,1,1, 1)
factory.addBox(0,0,0, 0.5,0.5,0.5, 2)
factory.cut([(3,1)], [(3,2)], 3)

x = 0; y = 0.75; z = 0; r = 0.09

holes = []
for t in 1:5
    global x, z
    x += 0.166
    z += 0.166
    factory.addSphere(x,y,z,r, 3 + t)
    t = (3, 3 + t)
    push!(holes, t)
end

ov = factory.fragment([(3,3)], holes)
factory.synchronize()

lcar1 = .1
lcar2 = .0005
lcar3 = .055

ov = model.getEntities(0);
model.mesh.setSize(ov, lcar1);

ov = model.getBoundary(holes, false, false, true);
model.mesh.setSize(ov, lcar3);

eps = 1e-3
ov = model.getEntitiesInBoundingBox(0.5-eps, 0.5-eps, 0.5-eps,
                                    0.5+eps, 0.5+eps, 0.5+eps, 0)
model.mesh.setSize(ov, lcar2)

model.mesh.generate(3)

gmsh.write("t16.msh")

gmsh.finalize()