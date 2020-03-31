#include "GmshConfig.h"
#include "GModel.h"
#include "meshGRegionHxt.h"
#include "meshGFaceHxt.h"
#include "Context.h"
#include "MVertex.h"
#include "GFace.h"
#include "GmshMessage.h"
#include "gmshCrossFields.h"

#if defined(HAVE_HXT)
extern "C" {
#include "hxt_api.h"
#include "remesh/hxt_gmsh_point_gen_main.h"
#include "remesh/hxt_point_gen_options.h"
}


int meshGFaceHxt(GModel *gm)
{

  HXT_CHECK(hxtSetMessageCallback(hxtGmshMsgCallback));

  HXTMesh *mesh;
  HXTContext *context;
  HXT_CHECK(hxtContextCreate(&context));
  HXT_CHECK(hxtMeshCreate(context, &mesh));
  
  std::map<int, std::vector<double> > dataH;
  std::map<int, std::vector<double> > dataDir;
  std::map<int, std::vector<double> > dataDirOrtho;
  computeCrossFieldAndH(gm,dataH,dataDir,dataDirOrtho);

  std::map<MVertex *, int> v2c;
  std::vector<MVertex *> c2v;
  HXT_CHECK(Gmsh2Hxt(gm, mesh, v2c, c2v));

  /// put the cross field and conformal factor into a big vector
  double *data = (double*)malloc(c2v.size()*sizeof(double)*7);
  for (size_t i = 0; i< c2v.size()*7 ; i++)data [i] = 0.0;
  
  std::map<int, std::vector<double> > :: iterator it = dataDir.begin();
  for ( ; it != dataDir.end() ; ++it){
    MElement *e = gm->getMeshElementByTag (it->first);
    std::vector<double> &dir      = it->second;
    std::vector<double> &dirOrtho = dataDirOrtho[it->first];
    SVector3 t1 (dir[0],dir[1],dir[2]);
    SVector3 t2 (dirOrtho[0],dirOrtho[1],dirOrtho[2]);
    SVector3 n = crossprod(t1,t2);
    
    for (int i=0;i< e->getNumVertices();i++){
      MVertex *v = e->getVertex (i);
      if (v2c.find(v)  == v2c.end())Msg::Error ("FILE %s LINE %d Cannot find vertex %lu",__FILE__,__LINE__,v->getNum()); 
      if (v2c[v] >= v2c.size())Msg::Error ("FILE %s LINE %d Bad numbering v2c[%lu] = %lu",__FILE__,__LINE__,v->getNum(),v2c[v]); 
      double *nn = data+7*v2c[v];
      nn[0] += n[0];
      nn[1] += n[1];
      nn[2] += n[2];
      SVector3 t (nn[3],nn[4],nn[5]);
      if (t.norm () < 1.e-12){
	nn[3] = t1.x();
	nn[4] = t1.y();
	nn[5] = t1.z();
      }
      else {
	double x0 = dot (t,t1);
	double x1 = dot (t,-t1);
	double x2 = dot (t,t2);
	double x3 = dot (t,-t2);
	if (x0 > x1 && x0 > x2 && x0 > x3){
	  nn[3]+= t1.x();nn[4]+= t1.y();nn[5]+= t1.z();
	}
	else if (x1 > x0 && x1 > x2 && x1 > x3){
	  nn[3]-= t1.x();nn[4]-= t1.y();nn[5]-= t1.z();
	}
	else if (x2 > x0 && x2 > x1 && x2 > x3){
	  nn[3]+= t2.x();nn[4]+= t2.y();nn[5]+= t2.z();
	}
	else{
	  nn[3]-= t2.x();nn[4]-= t2.y();nn[5]-= t2.z();
	}	  
      }
    }    
  }
  
  for (size_t i = 0; i< c2v.size() ; i++){
    double *n = data+ 7*i;
    
    SVector3 t (n[0],n[1],n[2]); t.normalize();
    n[0] = t.x();n[1] = t.y();n[2] = t.z();
    t = SVector3(n[3],n[4],n[5]); t.normalize();
    n[3] = t.x();n[4] = t.y();n[5] = t.z();
    
    if (dataH.find(c2v[i]->getNum()) != dataH.end()){    
      n[6] = dataH[c2v[i]->getNum()][0];
    }
    else {
      Msg::Warning ("Vertex %lu has no value for H",c2v[i]->getNum());
    }
  }
  
  ///// HERE WE NEED THE CODE TO THE REMESHING STUFF
   
  HXTContext *fcontext;
  HXTMesh *fmesh;
  HXT_CHECK(hxtContextCreate(&fcontext));
  HXT_CHECK(hxtMeshCreate(fcontext, &fmesh));

  // TODO 
  HXTPointGenOptions opt = { .verbosity = 0,
                             .generateLines = 1,
                             .generateSurfaces = 1,
                             .generateVolumes = 0,
                             .remeshSurfaces = 1,
                             .dirType = 1,
                             .areaThreshold = 0,
                             .uniformSize = 0.1};
  
  HXT_CHECK(hxtGmshPointGenMain(mesh,&opt,data,fmesh));
  HXT_CHECK(Hxt2Gmsh(gm, fmesh, v2c, c2v));
  
  HXT_CHECK(hxtMeshDelete(&fmesh));
  HXT_CHECK(hxtContextDelete(&fcontext));
 

  
  
  ///// END OF HERE WE NEED THE CODE TO THE REMESHING STUFF

  free (data);
  HXT_CHECK(hxtMeshDelete(&mesh));
  HXT_CHECK(hxtContextDelete(&context));
  

  return 0;
}


#else

int meshGFaceHxt(GModel *gm)
{
  Msg::Error("Gmsh should be compiled with Hxt to enable this option");
  return -1;
}

#endif