#include "GESubmesh.h"

//#include "GESkeleton.h"
//#include "GEMesh.h"
//#include "GEMorphAnimation.h"

#include "public.h"
#include "GameApp.h"

GESubmesh::~GESubmesh()
{
	SAFE_RELEASE(mVB);		
	SAFE_RELEASE(mIB);
	SAFE_DELETE(mBoneAssignment);
}

void GESubmesh::Render()		
{		
// 	hr =g_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
// 		0, 0, m_pTempGridArray.size() * 4, 0, m_pTempGridArray.size() * 2 );
// 
// 	HRESULT r = g_pD3DDevice->DrawIndexedPrimitive(*mIB);

}

void GESubmesh::RestoreDeviceObjects()
{
	//if(mVB) mVB->RestoreDeviceObjects();
	//if(mIB) mIB->RestoreDeviceObjects();
}

void GESubmesh::InvalidateDeviceObjects()
{
	//if(mVB) mVB->InvalidateDeviceObjects();
	//if(mIB) mIB->InvalidateDeviceObjects();
}

void GESubmesh::DeleteDeviceObjects()
{
	//if(mVB) mVB->DeleteDeviceObjects();
	//if(mIB) mIB->DeleteDeviceObjects();	
}

void GESubmesh::updateFromSkeleton(GESkeleton* skeleton)
{ 
	//if(mVB) mVB->updateFromSkeleton(skeleton, mBoneAssignment);
}

void GESubmesh::updateFromMorphAnimation(GEMorphAnimationKey* currentPose)
{
	//if(mVB) mVB->updateFromMorphAnimation(currentPose);
}


