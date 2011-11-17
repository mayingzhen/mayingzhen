#include "GEKeyframe.h"

GEKeyframe::GEKeyframe(void)
{
	mNeedRecomputeMatrix=false;
    mScale=D3DXVECTOR3(1.f,1.f,1.f);
	mTranslation=D3DXVECTOR3(0.f,0.f,0.f);
	mRotation=D3DXQUATERNION();	// default constructor
	//mMatrix=Matrix4();		// default constructor, identity matrix
	mTime=0.f;
}

GEKeyframe::GEKeyframe(D3DXVECTOR3 scale, D3DXVECTOR3 translate, D3DXQUATERNION rotate, float time)		
{	
    mScale=scale;
	mTranslation=translate;
	mRotation=rotate;
	mNeedRecomputeMatrix=true;
	recomputeMatrix();
	mTime=time;
}


GEKeyframe::~GEKeyframe(void)
{
}


GEKeyframe GEKeyframe::interpolate(GEKeyframe& kf2, float weight)
{
	D3DXVECTOR3 s,t;
	D3DXQUATERNION r;
	
	//s=mScale.Lerp(kf2.getScale(), weight);
	float omt = 1.f - weight;	
	s.x = omt * mScale.x + weight * kf2.getScale().x;
	s.y = omt * mScale.y + weight * kf2.getScale().y;
	s.z = omt * mScale.z + weight * kf2.getScale().z;

	//t=mTranslation.Lerp(kf2.getTranslation(), weight);
	t.x = omt * mTranslation.x + weight * kf2.getTranslation().x;
	t.y = omt * mTranslation.y + weight * kf2.getTranslation().y;
	t.z = omt * mTranslation.z + weight * kf2.getTranslation().z;
	
	//r=D3DXQUATERNION::nlerp(weight, mRotation, kf2.getRotation(),true); // weight是kf2的权重
	D3DXQuaternionSlerp(&r, &mRotation, &kf2.getRotation(), weight ); 

    return GEKeyframe(s, t, r, 0.0f);
}



