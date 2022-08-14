#include "OcclusionMask.h"

OcclusionMask::OcclusionMask(int width, int height)
{
   // pMoc = MaskedOcclusionCulling::Create(); 
//	pMoc->SetResolution(width, height); 
//	pMoc->ClearBuffer();
//	pMoc->SetNearClipPlane(nearClipDist);
}

OcclusionMask::~OcclusionMask()
{

}

void OcclusionMask::render()
{
    //for(auto obj : GumManagers::Objects->InstObjs)
    //{
    //    unsigned int nTris = obj.second->getProperties(0)->pMesh->indices.size() / 3;

        //pMoc->RenderTriangles((const float*)&obj.second->getProperties(0)->model->positions[0], &obj.second->getProperties(0)->model->indices[0], nTris);
    //}
}

void OcclusionMask::clean()
{
  //  MaskedOcclusionCulling::Destroy(pMoc);
}
