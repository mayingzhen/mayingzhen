#include "GEBone.h"
#include "GESkeleton.h"
#include "public.h"

GEBone::~GEBone(void)
{
}

void GEBone::setParent(int index) 
{ 
	mParentIndex=index; 
	// ���Լ���Ϊ���ڵ�Ķ��ӣ�
	GEBone* parent=mSkeleton->getBone(index);
	parent->addChild(mIndex);
}

void GEBone::setInitialTransform(const D3DXVECTOR3& scale, const D3DXVECTOR3& translate, const D3DXQUATERNION& rotate)
{
	mFullTransformNeedUpdate=true;
	mInitialScale=scale; 
	mInitialTranslation=translate; 
	mInitialRotation=rotate; 
}

void GEBone::update()
{
	// peter: �����updateЧ���е�ͣ���Ϊû�п��ǵ�bone��֡������µ����
	mFullTransformNeedUpdate=true;
	D3DXQUATERNION pRotate;
	D3DXVECTOR3    pScale;
	D3DXVECTOR3    pTranslate;

	if(mParentIndex>=0)
	{
		// ��Ϊupdate����������������������ڴ˼ٶ����ڵ������update��
		GEBone* parent=mSkeleton->getBone(static_cast<int>(mParentIndex));
		pRotate=parent->getAbsoluteRotation();
		pScale=parent->getAbsoluteScale();
		pTranslate=parent->getAbsoluteTranslation();
	}
	else
	{
		pRotate = D3DXQUATERNION(1.0,0.0,0.0,0.0); //::IDENTITY;
		pScale = D3DXVECTOR3(1,1,1); //::UNIT_SCALE;
		pTranslate = D3DXVECTOR3(0,0,0);//::ZERO;
	}


	mAbsoluteRotation = pRotate * mRelativeRotation;
	mAbsoluteScale = D3DXVECTOR3(mRelativeScale.x * pScale.x, 
		mRelativeScale.y * pScale.y, 
		mRelativeScale.z * pScale.z);//(mRelativeScale) * pScale;

	//mAbsoluteTranslation = pRotate * ((mRelativeTranslation) * pScale) + pTranslate;	

	vector<unsigned short>::const_iterator it=mChildrenIndex.begin();
	while(it!=mChildrenIndex.end())
	{
        mSkeleton->getBone(*it)->update();
		++it;
	}
}

void GEBone::updateWithInitialTransform()
{
	D3DXQUATERNION pRotate;
	D3DXVECTOR3    pScale;
	D3DXVECTOR3    pTranslate;

	if(mParentIndex>=0)
	{
		// ��Ϊupdate����������������������ڴ˼ٶ����ڵ������update��
		GEBone* parent=mSkeleton->getBone(static_cast<int>(mParentIndex));
		pRotate=parent->getAbsoluteRotation();
		pScale=parent->getAbsoluteScale();
		pTranslate=parent->getAbsoluteTranslation();
	}
	else
	{
// 		pRotate=D3DXQUATERNION::IDENTITY;
// 		pScale=D3DXVECTOR3::UNIT_SCALE;
// 		pTranslate=D3DXVECTOR3::ZERO;
		pRotate = D3DXQUATERNION(1.0,0.0,0.0,0.0); //::IDENTITY;
		pScale = D3DXVECTOR3(1,1,1); //::UNIT_SCALE;
		pTranslate = D3DXVECTOR3(0,0,0);//::ZERO;
	}

	mAbsoluteRotation = pRotate * mInitialRotation;
	//mAbsoluteScale = mInitialScale * pScale;
	//mAbsoluteTranslation = pRotate * ( mInitialTranslation  * pScale) + pTranslate;

	vector<unsigned short>::const_iterator it=mChildrenIndex.begin();
	while(it!=mChildrenIndex.end())
	{
		mSkeleton->getBone(*it)->updateWithInitialTransform();
		++it;
	}
}
 

D3DXMATRIX GEBone::getFullTransform() 
{
	if(!mFullTransformNeedUpdate) 
		return mFullTransform;

	// ˳�򣺷�������ת��ƽ��
	D3DXMATRIX fullTrans;//=Matrix4::IDENTITY;
	//Matrix3 rot3x3, scale3x3;
	//Matrix4 finalMatrix;
	D3DXMATRIX rot3x3, scale3x3, transMat;
	//mAbsoluteRotation.ToRotationMatrix(rot3x3);
	D3DXMatrixRotationQuaternion(&rot3x3, &mAbsoluteRotation); 

	//rot3x3 = rot3x3.Transpose();
	D3DXMatrixTranspose(&rot3x3, &rot3x3);

	//scale3x3 = Matrix3::ZERO;
	//scale3x3[0][0] = mAbsoluteScale.x;
	//scale3x3[1][1] = mAbsoluteScale.y;
	//scale3x3[2][2] = mAbsoluteScale.z;
	D3DXMatrixScaling(&scale3x3, mAbsoluteScale.x, mAbsoluteScale.y, mAbsoluteScale.z);

	D3DXMatrixTranslation(&transMat, mAbsoluteTranslation.x, mAbsoluteTranslation.y, mAbsoluteTranslation.z);

	fullTrans = rot3x3 * scale3x3 * transMat;
	//fullTrans.SetOffset(mAbsoluteTranslation);

	D3DXMATRIX finalMatrix = mInverseInitialTransform * fullTrans;

    mFullTransformNeedUpdate = false;
	mFullTransform = finalMatrix;

	return finalMatrix;
}


void GEBone::setToInitialTransform()
{
	mFullTransformNeedUpdate = true;
	this->mRelativeScale       = this->mInitialScale;
	this->mRelativeRotation	   = this->mInitialRotation;
	this->mRelativeTranslation = this->mInitialTranslation;

	for(size_t i=0; i<mChildrenIndex.size(); ++i)
	{
		GEBone* p=mSkeleton->getBone(mChildrenIndex[i]);
		assert(p && "�Ҳ������ӣ�");
		p->setToInitialTransform();
	}
}

void GEBone::setToNoTransform()
{
	mFullTransformNeedUpdate=true;
	mRelativeScale = D3DXVECTOR3(1, 1, 1);
	mRelativeRotation = D3DXQUATERNION(1.0,0.0,0.0,0.0);
	mRelativeTranslation = D3DXVECTOR3(0, 0, 0);

	for(size_t i=0; i<mChildrenIndex.size(); ++i)
	{
		GEBone* p=mSkeleton->getBone(mChildrenIndex[i]);
		assert(p != NULL && "�Ҳ������ӣ�");
		p->setToNoTransform();
	}
}

void GEBone::computeInverseInitialTransform()
{
	D3DXVECTOR3 scale = this->mAbsoluteScale;
	D3DXQUATERNION rotate = this->mAbsoluteRotation;
	D3DXVECTOR3 translate = this->mAbsoluteTranslation;

	scale.x = 1 / scale.x;
	scale.y = 1 / scale.y;
	scale.z = 1 / scale.z;
	translate *= -1;
	//rotate = rotate.Inverse();
	D3DXQuaternionInverse(&rotate, &rotate);

	translate.x *= scale.x;
	translate.y *= scale.y;
	translate.z *= scale.z;

	D3DXMATRIX rm;
	//rotate.ToRotationMatrix(rm);
	//rm = rm.Transpose();
	D3DXMatrixRotationQuaternion(&rm, &rotate);
	D3DXMatrixTranspose(&rm, &rm);

	//translate = rotate * translate;
	D3DXVec3TransformCoord(&translate, &translate, &rm);

	//D3DXMATRIX sm(scale.x,	0,	  0, 
  	//				 0,		scale.y,	  0,
	//				 0,		  0,scale.z);
	D3DXMATRIX sm;
	D3DXMatrixScaling(&sm, scale.x, scale.y, scale.z);
	//Matrix4 tm=Matrix4::GetOffset(translate);
	D3DXMATRIX tm;
	D3DXMatrixTranslation(&tm, translate.x, translate.y, translate.z);
	// ˳��ƽ�ơ���ת������
	//Matrix4 tmp(rm*sm);
	D3DXMATRIX tmp = rm * sm * tm;

    //tmp.SetOffset(translate);
	mInverseInitialTransform = tmp;
    
	for(size_t i=0; i<mChildrenIndex.size(); ++i)
	{
		GEBone* p=mSkeleton->getBone(mChildrenIndex[i]);
		assert(p && "�Ҳ������ӣ�");
		p->computeInverseInitialTransform();
	}
}


void GEBone::setRelativeTransform(GEKeyframe& kf)
{
	D3DXQUATERNION rotate=kf.getRotation();

	//mRelativeScale       = kf.getScale() * mInitialScale;
	//mRelativeTranslation = kf.getTranslation() + mInitialTranslation;
	//mRelativeRotation    = mInitialRotation * kf.getRotation(); 
}

void GEBone::setRelativeTransform(const D3DXVECTOR3& scale, const D3DXVECTOR3& translate, const D3DXQUATERNION& rotate)
{
	//mRelativeScale       = scale * mInitialScale;
	//mRelativeTranslation = translate + mInitialTranslation;
	//mRelativeRotation    = mInitialRotation * rotate;
}
