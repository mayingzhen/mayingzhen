#pragma once

#include "d3dx9math.h"

/**
 * ����һ����ͷĳ֡�ı任��������ƽ�ơ���ת
 */
class GEKeyframe
{
public:
	GEKeyframe();	///< Ĭ�Ϲ��캯��������һ�������κα任�Ĺؼ�֡
	GEKeyframe(D3DXVECTOR3 scale, D3DXVECTOR3 translate, D3DXQUATERNION rotate, float time);
	virtual ~GEKeyframe();

	/// ��ñ任����
	inline D3DXMATRIX getMatrix()
	{
		if(mNeedRecomputeMatrix)
		{	
			recomputeMatrix();
		}
		return mMatrix;
	}
	/// ���¼���任����
	inline void recomputeMatrix()
	{		
		//Matrix4 m(mRotation);
		//m.SetOffset(mTranslation);
		

		// how to do scale?
		//m.SetScale(mScale);

		//mMatrix=m;
		//mNeedRecomputeMatrix=false;	

		//D3DXMatrixIdentity(&mMatrix);
		D3DXMATRIX rm, sm, tm;
		D3DXMatrixRotationQuaternion(&rm, &mRotation);
		
		D3DXMatrixScaling(&sm, mScale.x, mScale.y, mScale.z);
		
		D3DXMatrixTranslation(&tm, mTranslation.x, mTranslation.y, mTranslation.z);

		mMatrix = rm * sm * tm;

		mNeedRecomputeMatrix = false;	
	}

	/// �������ؼ�֡�ı任����
// 	inline GEKeyframe operator*(GEKeyframe& kf)
// 	{
// 		return GEKeyframe(mScale * kf.mScale, mTranslation * kf.mTranslation, 
// 			mRotation * kf.mRotation, 0.0f);
// 	}

	/// ��÷���
	inline D3DXVECTOR3 getScale() {	return mScale; }
	/// ���ƽ��
	inline D3DXVECTOR3 getTranslation() { return mTranslation; }
	/// �����ת
	inline D3DXQUATERNION getRotation()	{ return mRotation; }
	/// ���ô˹ؼ�֡��ʱ��
	inline void  setTime(float time) { mTime=time; }
	/// ��ô˹ؼ�֡��ʱ��
	inline float getTime() { return mTime; }
	/// ���÷���
	inline void setScale(D3DXVECTOR3 scale)
	{
        mScale=scale;
		mNeedRecomputeMatrix=true;
	}
	/// ����ƽ��
	inline void setTranslation(D3DXVECTOR3 translate)
	{
		mTranslation=translate;
		mNeedRecomputeMatrix=true;
	}
	/// ������ת
	inline void setRotation(D3DXQUATERNION rotate)
	{
		mRotation=rotate;
		mNeedRecomputeMatrix=true;
	}
	/**
	 * ���Լ���kf2�����Բ�ֵ���������Բ�ֵ�����ز�ֵ��ı任��������һ���ؼ�֡�����У�
	 * @param weight kf2��Ȩ��
	 */	
	GEKeyframe interpolate(GEKeyframe& kf2, float weight);	

protected:
	D3DXVECTOR3    mScale;					///< ����
	D3DXVECTOR3    mTranslation;			///< ƽ��
	D3DXQUATERNION mRotation;				///< ��ת
	D3DXMATRIX    mMatrix;					///< �任����
    bool       mNeedRecomputeMatrix;	///< �Ƿ���Ҫ���¾���ı�־
	float	   mTime;					///< �˹ؼ�֡��ʱ��
};
