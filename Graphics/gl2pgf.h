// Gmsh - Copyright (C) 1997-2019 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// issues on https://gitlab.onelab.info/gmsh/gmsh/issues.

#ifndef GL2PGF_H
#define GL2PGF_H

#include <string>
#include "PixelBuffer.h"

int print_pgf(const std::string &name, const int num, const int cnt,
              PixelBuffer *buffer, double *eulerAngles, int *viewport,
              double *proj, double *model);

#endif
