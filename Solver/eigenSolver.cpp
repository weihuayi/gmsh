// Gmsh - Copyright (C) 1997-2009 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to <gmsh@geuz.org>.

#include "eigenSolver.h"

#if defined(HAVE_SLEPC)

#include <slepceps.h>

eigenSolver::eigenSolver(dofManager<double, double> *manager, std::string A, 
                         std::string B) : _A(0), _B(0)
{
  if(A.size()){
    _A = dynamic_cast<linearSystemPETSc<double>*>(manager->getLinearSystem(A));
    if(!_A) Msg::Error("Could not find PETSc system '%s'", A.c_str());
  }
  if(B.size()){
    _B = dynamic_cast<linearSystemPETSc<double>*>(manager->getLinearSystem(B));
    if(!_B) Msg::Error("Could not find PETSc system '%s'", B.c_str());
  }
}

void eigenSolver::solve(int numEigenValues, std::string which)
{
  if(!_A) return;
  Mat A = _A->getMatrix();
  Mat B = _B ? _B->getMatrix() : PETSC_NULL;

  _try(MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY));
  _try(MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY));
  PetscInt N, M;
  _try(MatGetSize(A, &N, &M));

  // generalized eigenvalue problem A x - \lambda B x = 0
  EPS eps;
  _try(EPSCreate(PETSC_COMM_WORLD, &eps));
  _try(EPSSetOperators(eps, A, B));
  bool hermitian = false; // FIXME
  if(hermitian)
    _try(EPSSetProblemType(eps, _B ? EPS_GHEP : EPS_HEP));
  else
    _try(EPSSetProblemType(eps, _B ? EPS_GNHEP : EPS_NHEP));

  // set some default options
  _try(EPSSetDimensions(eps, 5, PETSC_DECIDE, PETSC_DECIDE));

  // override these options at runtime, petsc-style
  _try(EPSSetFromOptions(eps));

  // force options specified directly as arguments
  if(numEigenValues)
    _try(EPSSetDimensions(eps, numEigenValues, PETSC_DECIDE, PETSC_DECIDE));
  if(which == "smallest")
    _try(EPSSetWhichEigenpairs(eps, EPS_SMALLEST_MAGNITUDE));
  else if(which == "largest")
    _try(EPSSetWhichEigenpairs(eps, EPS_LARGEST_MAGNITUDE));

  // print info
  const EPSType type;
  _try(EPSGetType(eps, &type));
  Msg::Info("SLEPc solution method: %s", type);
  PetscInt nev;
  _try(EPSGetDimensions(eps, &nev, PETSC_NULL, PETSC_NULL));
  Msg::Info("SLEPc number of requested eigenvalues: %d", nev);
  PetscReal tol;
  PetscInt maxit;
  _try(EPSGetTolerances(eps, &tol, &maxit));
  Msg::Info("SLEPc stopping condition: tol=%g, maxit=%d", tol, maxit);

  // solve
  Msg::Info("SLEPc solving...");
  _try(EPSSolve(eps));

  // check convergence
  int its;
  _try(EPSGetIterationNumber(eps, &its));
  EPSConvergedReason reason;
  _try(EPSGetConvergedReason(eps, &reason));
  if(reason == EPS_CONVERGED_TOL)
    Msg::Info("SLEPc converged in %d iterations", its);
  else if(reason == EPS_DIVERGED_ITS)
    Msg::Error("SLEPc diverged after %d iterations", its);
  else if(reason == EPS_DIVERGED_BREAKDOWN)
    Msg::Error("SLEPc generic breakdown in method");
  else if(reason == EPS_DIVERGED_NONSYMMETRIC)
    Msg::Error("The operator is nonsymmetric");
  
  // get number of converged approximate eigenpairs
  PetscInt nconv;
  _try(EPSGetConverged(eps, &nconv));
  Msg::Info("SLEPc number of converged eigenpairs: %d", nconv);

  // ignore additional eigenvalues if we get more than what we asked
  if(nconv > nev) nconv = nev;

  Vec xr, xi;
  _try(MatGetVecs(A, PETSC_NULL, &xr));
  _try(MatGetVecs(A, PETSC_NULL, &xi));
  Msg::Info("         Re[EigenValue]          Im[EigenValue]"
            "          Relative error");
  for (int i = 0; i < nconv; i++){
    PetscScalar kr, ki;
    _try(EPSGetEigenpair(eps, i, &kr, &ki, xr, xi));
    PetscReal error;
    _try(EPSComputeRelativeError(eps, i, &error));
#if defined(PETSC_USE_COMPLEX)
    PetscReal re = PetscRealPart(kr);
    PetscReal im = PetscImaginaryPart(kr);
#else
    PetscReal re = kr;
    PetscReal im = ki;
#endif
    Msg::Info("EIG %03d %s%.16e %s%.16e  %3.6e", 
              i, (re < 0) ? "" : " ", re, (im < 0) ? "" : " ", im, error);

    // store eigenvalues and eigenvectors
    _eigenValues.push_back(std::complex<double>(re, im));
    PetscScalar *tmpr, *tmpi;
    _try(VecGetArray(xr, &tmpr));
    _try(VecGetArray(xi, &tmpi));
    std::vector<std::complex<double> > ev(N);
    for(int i = 0; i < N; i++){
#if defined(PETSC_USE_COMPLEX)
      ev[i] = tmpr[i];
#else
      ev[i] = std::complex<double>(tmpr[i], tmpi[i]);
#endif
    }
    _eigenVectors.push_back(ev);
  }
  
  // cleanup
  _try(EPSDestroy(eps));
  _try(VecDestroy(xr));
  _try(VecDestroy(xi));
  _try(SlepcFinalize());
  Msg::Info("SLEPc done");
}

#endif
