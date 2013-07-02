#include "BasisLagrange.h"

using namespace std;

BasisLagrange::BasisLagrange(void){
  scalar = true;

  preEvaluated     = false;
  preEvaluatedGrad = false;

  preEvaluatedFunction     = NULL;
  preEvaluatedGradFunction = NULL;
}

BasisLagrange::~BasisLagrange(void){
  if(preEvaluated)
    delete preEvaluatedFunction;

  if(preEvaluatedGrad)
    delete preEvaluatedGradFunction;
}

unsigned int BasisLagrange::
getNOrientation(void) const{
  return 1;
}

unsigned int BasisLagrange::
getOrientation(const MElement& element) const{
  return 0;
}

static bool
sortPredicate(const std::pair<size_t, size_t>& a,
              const std::pair<size_t, size_t>& b){
  return a.second < b.second;
}

static vector<int> reducedNodeId(const MElement& element){
  const size_t nVertex = element.getNumPrimaryVertices();
  vector<pair<size_t, size_t> > vertexGlobalId(nVertex);

  for(size_t i = 0; i < nVertex; i++){
    vertexGlobalId[i].first  = i;
    vertexGlobalId[i].second = element.getVertex(i)->getNum();
  }

  std::sort(vertexGlobalId.begin(), vertexGlobalId.end(), sortPredicate);

  vector<int> vertexReducedId(nVertex);

  for(size_t i = 0; i < nVertex; i++)
    vertexReducedId[vertexGlobalId[i].first] = i;

  return vertexReducedId;
}

static size_t matchClosure(vector<int>& reduced,
                           nodalBasis::clCont& closures){

  const size_t nNode = reduced.size();
  const size_t nPerm = closures.size();

  size_t i = 0;
  bool   match = false;

  while(i < nPerm && !match){
    match = true;

    for(size_t j = 0; j < nNode && match; j++)
      if(reduced[j] != closures[i][j])
         match = false;

    if(!match)
      i++;
  }

  return i;
}

vector<size_t> BasisLagrange::
getFunctionOrdering(const MElement& element) const{
  vector<int> rNodeId = reducedNodeId(element);
  const size_t closureId = matchClosure(rNodeId, lBasis->fullClosures);

  vector<int>& closure = lBasis->fullClosures[closureId];

  vector<size_t> myClosure(closure.size());

  for(size_t i = 0; i < closure.size(); i++)
    myClosure[i] = closure[i];

  return myClosure;
}

void BasisLagrange::
getFunctions(fullMatrix<double>& retValues,
             const MElement& element,
             double u, double v, double w) const{

  // Fill Matrix //
  fullMatrix<double> tmp;
  fullMatrix<double> point(1, 3);
  point(0, 0) = u;
  point(0, 1) = v;
  point(0, 2) = w;

  lBasis->f(point, tmp);

  // Transpose 'tmp': otherwise not coherent with df !!
  retValues = tmp.transpose();
}

void BasisLagrange::
getFunctions(fullMatrix<double>& retValues,
             unsigned int orientation,
             double u, double v, double w) const{

  // Fill Matrix //
  fullMatrix<double> tmp;
  fullMatrix<double> point(1, 3);
  point(0, 0) = u;
  point(0, 1) = v;
  point(0, 2) = w;

  lBasis->f(point, tmp);

  // Transpose 'tmp': otherwise not coherent with df !!
  retValues = tmp.transpose();
}

void BasisLagrange::preEvaluateFunctions(const fullMatrix<double>& point) const{
  // Delete if older //
  if(preEvaluated)
    delete preEvaluatedFunction;

  // Fill Matrix //
  fullMatrix<double> tmp;
  lBasis->f(point, tmp);

  // Transpose 'tmp': otherwise not coherent with df !!
  preEvaluatedFunction = new fullMatrix<double>(tmp.transpose());

  // PreEvaluated //
  preEvaluated = true;
}

void BasisLagrange::
preEvaluateDerivatives(const fullMatrix<double>& point) const{
  // Delete if older //
  if(preEvaluatedGrad)
    delete preEvaluatedGradFunction;

  // Alloc //
  preEvaluatedGradFunction = new fullMatrix<double>;

  // Fill Matrix //
  lBasis->df(point, *preEvaluatedGradFunction);

  // PreEvaluated //
  preEvaluatedGrad = true;
}

const fullMatrix<double>&
BasisLagrange::getPreEvaluatedFunctions(const MElement& element) const{
  return *preEvaluatedFunction;
}

const fullMatrix<double>&
BasisLagrange::getPreEvaluatedDerivatives(const MElement& element) const{
  return *preEvaluatedGradFunction;
}

const fullMatrix<double>&
BasisLagrange::getPreEvaluatedFunctions(unsigned int orientation) const{
  return *preEvaluatedFunction;
}

const fullMatrix<double>&
BasisLagrange::getPreEvaluatedDerivatives(unsigned int orientation) const{
  return *preEvaluatedGradFunction;
}

vector<double> BasisLagrange::
project(const MElement& element,
        const vector<double>& coef,
        const FunctionSpaceScalar& fSpace){

  // Init New Coefs //
  const unsigned int size = lPoint->size1();
  const unsigned int dim  = lPoint->size2();

  vector<double> newCoef(size);

  // Interpolation at Lagrange Points //
  for(unsigned int i = 0; i < size; i++){
    fullVector<double> uvw(3);

    if(dim > 0)
      uvw(0) = (*lPoint)(i, 0);
    else
      uvw(0) = 0;

    if(dim > 1)
      uvw(1) = (*lPoint)(i, 1);
    else
      uvw(1) = 0;

    if(dim > 2)
      uvw(2) = (*lPoint)(i, 2);
    else
      uvw(2) = 0;

    newCoef[i] = fSpace.interpolateInRefSpace(element,
                                              coef,
                                              uvw);
  }

  // Return ;
  return newCoef;
}

vector<fullVector<double> > BasisLagrange::
project(const MElement& element,
        const vector<double>& coef,
        const FunctionSpaceVector& fSpace){

  // Init New Coefs //
  const unsigned int size = lPoint->size1();
  vector<fullVector<double> > newCoef(size);

  // Interpolation at Lagrange Points //
  for(unsigned int i = 0; i < size; i++){
    fullVector<double> uvw(3);

    if(dim > 0)
      uvw(0) = (*lPoint)(i, 0);
    else
      uvw(0) = 0;

    if(dim > 1)
      uvw(1) = (*lPoint)(i, 1);
    else
      uvw(1) = 0;

    if(dim > 2)
      uvw(2) = (*lPoint)(i, 2);
    else
      uvw(2) = 0;

    newCoef[i] = fSpace.interpolateInRefSpace(element,
                                              coef,
                                              uvw);
  }

  // Return ;
  return newCoef;
}
