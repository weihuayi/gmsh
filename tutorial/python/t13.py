# ------------------------------------------------------------------------------
#
#  Gmsh Python tutorial 13
#
#  Remeshing an STL file without an underlying CAD model
#
# ------------------------------------------------------------------------------

import gmsh
import math
import os
import sys

gmsh.initialize()

# Let's merge an STL mesh that we would like to remesh (from the parent
# directory):
path = os.path.dirname(os.path.abspath(__file__))
gmsh.merge(os.path.join(path, os.pardir, 't13_data.stl'))

# We first classify ("color") the surfaces by splitting the original surface
# along sharp geometrical features. This will create new discrete surfaces,
# curves and points.

# Angle between two triangles above which an edge is considered as sharp:
angle = 40

# For complex geometries, patches can be too complex, too elongated or too large
# to be parametrized; setting the following option will force the creation of
# patches that are amenable to reparametrization:
forceParametrizablePatches = False

# For open surfaces include the boundary edges in the classification process:
includeBoundary = True

# Force curves to be split on given angle:
curveAngle = 180

gmsh.model.mesh.classifySurfaces(angle * math.pi / 180., includeBoundary,
                                 forceParametrizablePatches,
                                 curveAngle * math.pi / 180.)

# Create a geometry for all the discrete curves and surfaces in the mesh, by
# computing a parametrization for each one
gmsh.model.mesh.createGeometry()

# Note that if a CAD model (e.g. as a STEP file, see `t20.py') is available
# instead of an STL mesh, it is usually better to use that CAD model instead of
# the geometry created by reparametrizing the mesh. Indeed, CAD geometries will
# in general be more accurate, with smoother parametrizations, and will lead to
# more efficient and higher quality meshing. Discrete surface remeshing in Gmsh
# is optimized to handle dense STL meshes coming from e.g. imaging systems /
# where no CAD is available; it less well suited for the poor quality STL
# triangulations (optimized for size, with e.g. very elongated triangles) that
# are usually generated by CAD tools for e.g. 3D printing.

# Create a volume from all the surfaces
s = gmsh.model.getEntities(2)
l = gmsh.model.geo.addSurfaceLoop([s[i][1] for i in range(len(s))])
gmsh.model.geo.addVolume([l])

gmsh.model.geo.synchronize()

# We specify element sizes imposed by a size field, just because we can :-)
funny = False
f = gmsh.model.mesh.field.add("MathEval")
if funny:
    gmsh.model.mesh.field.setString(f, "F", "2*Sin((x+y)/5) + 3")
else:
    gmsh.model.mesh.field.setString(f, "F", "4")
gmsh.model.mesh.field.setAsBackgroundMesh(f)

gmsh.model.mesh.generate(3)
gmsh.write('t13.msh')

# Launch the GUI to see the results:
if '-nopopup' not in sys.argv:
    gmsh.fltk.run()

gmsh.finalize()
