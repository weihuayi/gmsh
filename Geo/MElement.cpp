// $Id: MElement.cpp,v 1.29 2007-01-22 16:31:43 geuzaine Exp $
//
// Copyright (C) 1997-2007 C. Geuzaine, J.-F. Remacle
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
// 
// Please report all bugs and problems to <gmsh@geuz.org>.

#include <math.h>
#include "MElement.h"
#include "GEntity.h"
#include "Numeric.h"
#include "Message.h"

int edges_tetra[6][2] = {
  {0, 1},
  {1, 2},
  {2, 0},
  {3, 0},
  {3, 2},
  {3, 1}
};

int edges_quad[4][2] = {
  {0, 1},
  {1, 2},
  {2, 3},
  {3, 0}
};

int edges_hexa[12][2] = {
  {0, 1},
  {0, 3},
  {0, 4},
  {1, 2},
  {1, 5},
  {2, 3},
  {2, 6},
  {3, 7},
  {4, 5},
  {4, 7},
  {5, 6},
  {6, 7}
};

int edges_prism[9][2] = {
  {0, 1},
  {0, 2},
  {0, 3},
  {1, 2},
  {1, 4},
  {2, 5},
  {3, 4},
  {3, 5},
  {4, 5}
};

int edges_pyramid[8][2] = {
  {0, 1},
  {0, 3},
  {0, 4},
  {1, 2},
  {1, 4},
  {2, 3},
  {2, 4},
  {3, 4}
};

int trifaces_tetra[4][3] = {
  {0, 2, 1},
  {0, 1, 3},
  {0, 3, 2},
  {3, 1, 2}
};

int trifaces_prism[2][3] = {
  {0, 2, 1},
  {3, 4, 5}
};

int trifaces_pyramid[4][3] = {
  {0, 1, 4},
  {3, 0, 4},
  {1, 2, 4},
  {2, 3, 4}
};

int quadfaces_hexa[6][4] = {
  {0, 3, 2, 1},
  {0, 1, 5, 4},
  {0, 4, 7, 3},
  {1, 2, 6, 5},
  {2, 3, 7, 6},
  {4, 5, 6, 7}
};

int quadfaces_prism[3][4] = {
  {0, 1, 4, 3},
  {0, 3, 5, 2},
  {1, 2, 5, 4}
};

int quadfaces_pyramid[1][4] = {
  {0, 3, 2, 1}
};

int MElement::_globalNum = 0;
double MElementLessThanLexicographic::tolerance = 1.e-6;

double MElement::minEdge()
{
  double m = 1.e25;
  for(int i = 0; i < getNumEdges(); i++){
    MEdge e = getEdge(i);
    m = std::min(m, e.getVertex(0)->distance(e.getVertex(1)));
  }
  return m;
}

double MElement::maxEdge()
{
  double m = 0.;
  for(int i = 0; i < getNumEdges(); i++){
    MEdge e = getEdge(i);
    m = std::max(m, e.getVertex(0)->distance(e.getVertex(1)));
  }
  return m;
}

double MElement::rhoShapeMeasure()
{
  double min = minEdge();
  double max = maxEdge();
  if(max)
    return min / max;
  else
    return 0.;
}

double MTetrahedron::gammaShapeMeasure()
{
  double p0[3] = { _v[0]->x(), _v[0]->y(), _v[0]->z() };
  double p1[3] = { _v[1]->x(), _v[1]->y(), _v[1]->z() };
  double p2[3] = { _v[2]->x(), _v[2]->y(), _v[2]->z() };
  double p3[3] = { _v[3]->x(), _v[3]->y(), _v[3]->z() };
  double s1 = fabs(triangle_area(p0, p1, p2));
  double s2 = fabs(triangle_area(p0, p2, p3));
  double s3 = fabs(triangle_area(p0, p1, p3));
  double s4 = fabs(triangle_area(p1, p2, p3));
  double rhoin = 3. * fabs(getVolume()) / (s1 + s2 + s3 + s4);
  return 12. * rhoin / (sqrt(6.) * maxEdge());
}

double MTetrahedron::etaShapeMeasure()
{
  double lij2 = 0.;
  for(int i = 0; i <= 3; i++) {
    for(int j = i + 1; j <= 3; j++) {
      double lij = _v[i]->distance(_v[j]);
      lij2 += lij * lij;
    }
  }
  double v = fabs(getVolume());
  return 12. * pow(0.9 * v * v, 1./3.) / lij2;
}

SPoint3 MElement::barycenter()
{
  SPoint3 p(0., 0., 0.);
  int n = getNumVertices();
  for(int i = 0; i < n; i++) {
    MVertex *v = getVertex(i);
    p[0] += v->x();
    p[1] += v->y();
    p[2] += v->z();
  }
  p[0] /= (double)n;
  p[1] /= (double)n;
  p[2] /= (double)n;
  return p;
}

std::string MElement::getInfoString()
{
  char tmp[256];
  sprintf(tmp, "Element %d", getNum());
  return std::string(tmp);
}

void MElement::writeMSH(FILE *fp, double version, bool binary, int num, 
			int elementary, int physical)
{
  int type = getTypeForMSH();
  if(!type) return;

  // if necessary, change the ordering of the vertices to get positive
  // volume
  setVolumePositive();
  int n = getNumVertices();

  if(!binary){
    fprintf(fp, "%d %d", num ? num : _num, type);
    if(version < 2.0)
      fprintf(fp, " %d %d %d", abs(physical), elementary, n);
    else
      fprintf(fp, " 3 %d %d %d", abs(physical), elementary, _partition);
  }
  else{
    int tags[4] = {num ? num : _num, abs(physical), elementary, _partition};
    fwrite(tags, sizeof(int), 4, fp);
  }

  if(physical < 0) revert();

  int verts[30];
  for(int i = 0; i < n; i++)
    verts[i] = getVertex(i)->getNum();

  if(!binary){
    for(int i = 0; i < n; i++)
      fprintf(fp, " %d", verts[i]);
    fprintf(fp, "\n");
  }
  else{
    fwrite(verts, sizeof(int), n, fp);
  }

  if(physical < 0) revert();
}

void MElement::writePOS(FILE *fp, double scalingFactor, int elementary)
{
  char *str = getStringForPOS();
  if(!str) return;

  int n = getNumVertices();
  double gamma = gammaShapeMeasure();
  double eta = etaShapeMeasure();
  double rho = rhoShapeMeasure();
  fprintf(fp, "%s(", str);
  for(int i = 0; i < n; i++){
    if(i) fprintf(fp, ",");
    fprintf(fp, "%g,%g,%g", getVertex(i)->x() * scalingFactor, 
	    getVertex(i)->y() * scalingFactor, getVertex(i)->z() * scalingFactor);
  }
  fprintf(fp, "){");
  for(int i = 0; i < n; i++)
    fprintf(fp, "%d,", elementary);
  for(int i = 0; i < n; i++)
    fprintf(fp, "%d,", getNum());
  for(int i = 0; i < n; i++)
    fprintf(fp, "%g,", gamma);
  for(int i = 0; i < n; i++)
    fprintf(fp, "%g,", eta);
  for(int i = 0; i < n; i++){
    if(i == n - 1)
      fprintf(fp, "%g", rho);
    else
      fprintf(fp, "%g,", rho);
  }
  fprintf(fp, "};\n");
}

void MElement::writeSTL(FILE *fp, bool binary, double scalingFactor)
{
  if(getNumEdges() != 3 && getNumEdges() != 4) return;
  int qid[3] = {0, 2, 3};
  SVector3 n = getFace(0).normal();
  if(!binary){
    fprintf(fp, "facet normal %g %g %g\n", n[0], n[1], n[2]);
    fprintf(fp, "  outer loop\n");
    for(int j = 0; j < 3; j++)
      fprintf(fp, "    vertex %g %g %g\n", 
	      getVertex(j)->x() * scalingFactor, 
	      getVertex(j)->y() * scalingFactor, 
	      getVertex(j)->z() * scalingFactor);
    fprintf(fp, "  endloop\n");
    fprintf(fp, "endfacet\n");
    if(getNumVertices() == 4){
      fprintf(fp, "facet normal %g %g %g\n", n[0], n[1], n[2]);
      fprintf(fp, "  outer loop\n");
      for(int j = 0; j < 3; j++)
	fprintf(fp, "    vertex %g %g %g\n", 
		getVertex(qid[j])->x() * scalingFactor, 
		getVertex(qid[j])->y() * scalingFactor, 
		getVertex(qid[j])->z() * scalingFactor);
      fprintf(fp, "  endloop\n");
      fprintf(fp, "endfacet\n");
    }
  }
  else{
    char data[50];
    float *coords = (float*)data;
    coords[0] = n[0];
    coords[1] = n[1];
    coords[2] = n[2];
    for(int j = 0; j < 3; j++){
      coords[3 + 3 * j] = getVertex(j)->x() * scalingFactor;
      coords[3 + 3 * j + 1] = getVertex(j)->y() * scalingFactor;
      coords[3 + 3 * j + 2] = getVertex(j)->z() * scalingFactor;
    }
    data[48] = data[49] = 0;
    fwrite(data, sizeof(char), 50, fp);
    if(getNumVertices() == 4){
      for(int j = 0; j < 3; j++){
	coords[3 + 3 * j] = getVertex(qid[j])->x() * scalingFactor;
	coords[3 + 3 * j + 1] = getVertex(qid[j])->y() * scalingFactor;
	coords[3 + 3 * j + 2] = getVertex(qid[j])->z() * scalingFactor;
      }
      fwrite(data, sizeof(char), 50, fp);
    }
  }
}

void MElement::writeVRML(FILE *fp)
{
  for(int i = 0; i < getNumVertices(); i++)
    fprintf(fp, "%d,", getVertex(i)->getNum() - 1);
  fprintf(fp, "-1,\n");
}

void MElement::writeUNV(FILE *fp, int num, int elementary, int physical)
{
  int type = getTypeForUNV();
  if(!type) return;

  setVolumePositive();
  int n = getNumVertices();
  int physical_property = elementary;
  int material_property = abs(physical);
  int color = 7;
  fprintf(fp, "%10d%10d%10d%10d%10d%10d\n",
	  num ? num : _num, type, physical_property, material_property, color, n);
  if(type == 21 || type == 24) // linear beam or parabolic beam
    fprintf(fp, "%10d%10d%10d\n", 0, 0, 0);

  if(physical < 0) revert();

  for(int k = 0; k < n; k++) {
    fprintf(fp, "%10d", getVertexUNV(k)->getNum());
    if(k % 8 == 7)
      fprintf(fp, "\n");
  }
  if(n - 1 % 8 != 7)
    fprintf(fp, "\n");

  if(physical < 0) revert();
}

void MElement::writeMESH(FILE *fp, int elementary)
{
  for(int i = 0; i < getNumVertices(); i++)
    fprintf(fp, " %d", getVertex(i)->getNum());
  fprintf(fp, " %d\n", elementary);
}

void MElement::writeBDF(FILE *fp, int format, int elementary)
{
  char *str = getStringForBDF();
  if(!str) return;

  setVolumePositive();
  int n = getNumVertices();
  const char *cont[4] = {"E", "F", "G", "H"};
  int ncont = 0;
  
  if(format == 0){ // free field format
    fprintf(fp, "%s,%d,%d", str, _num, elementary);
    for(int i = 0; i < n; i++){
      fprintf(fp, ",%d", getVertex(i)->getNum());
      if(i != n - 1 && !((i + 3) % 8)){
	fprintf(fp, ",+%s%d\n+%s%d", cont[ncont], _num, cont[ncont], _num);
	ncont++;
      }
    }
    if(n == 2) // CBAR
      fprintf(fp, ",0.,0.,0.");
    fprintf(fp, "\n");
  }
  else{ // small or large field format
    fprintf(fp, "%-8s%-8d%-8d", str, _num, elementary);
    for(int i = 0; i < n; i++){
      fprintf(fp, "%-8d", getVertex(i)->getNum());
      if(i != n - 1 && !((i + 3) % 8)){
	fprintf(fp, "+%s%-6d\n+%s%-6d", cont[ncont], _num, cont[ncont], _num);
	ncont++;
      }
    }
    if(n == 2) // CBAR
      fprintf(fp, "%-8s%-8s%-8s", "0.", "0.", "0.");
    fprintf(fp, "\n");
  }
}

bool MTriangle::invertmappingXY(double *p, double *uv, double tol)
{
  double mat[2][2];
  double b[2];
  getMat(mat);
  b[0] = p[0] - getVertex(0)->x();
  b[1] = p[1] - getVertex(0)->y();
  sys2x2(mat, b, uv);

  if(uv[0] >= -tol && 
     uv[1] >= -tol && 
     uv[0] <= 1. + tol && 
     uv[1] <= 1. + tol && 
     1. - uv[0] - uv[1] > -tol) {
    return true;
  }
  return false; 
}


double MTriangle::getSurfaceXY() const
{
  const double x1 =_v[0]->x();
  const double x2 =_v[1]->x();
  const double x3 =_v[2]->x();
  const double y1 =_v[0]->y();
  const double y2 =_v[1]->y();
  const double y3 =_v[2]->y();

  const double v1 [2] = {x2-x1,y2-y1};
  const double v2 [2] = {x3-x1,y3-y1};

  double s = v1[0]*v2[1] - v1[1]*v2[0]; 
  return s*0.5;
  
}

void MTriangle::circumcenterXY(double *res) const
{
  double d, a1, a2, a3;

  const double x1 =_v[0]->x();
  const double x2 =_v[1]->x();
  const double x3 =_v[2]->x();
  const double y1 =_v[0]->y();
  const double y2 =_v[1]->y();
  const double y3 =_v[2]->y();

  d = 2. * (double)(y1 * (x2 - x3) + y2 * (x3 - x1) + y3 * (x1 - x2));
  if(d == 0.0) {
    Msg(GERROR, "Colinear points in circum circle computation");
    res[0] = res[1] = -99999.;
    return ;
  }

  a1 = x1 * x1 + y1 * y1;
  a2 = x2 * x2 + y2 * y2;
  a3 = x3 * x3 + y3 * y3;
  res[0] = (double)((a1 * (y3 - y2) + a2 * (y1 - y3) + a3 * (y2 - y1)) / d);
  res[1] = (double)((a1 * (x2 - x3) + a2 * (x3 - x1) + a3 * (x1 - x2)) / d);

  //  printf("%g %g - %g %g - %g %g cc %g %g\n",x1,y1,x2,y2,x3,y3,res[0],res[1]);

  return ;
}

