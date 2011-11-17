#pragma once

#include "d3dx9math.h"

/**
 * 包含一个骨头某帧的变换：放缩、平移、旋转
 */
class GEKeyframe
{
public:
	GEKeyframe();	///< 默认构造函数，创建一个不作任何变换的关键帧
	GEKeyframe(D3DXVECTOR3 scale, D3DXVECTOR3 translate, D3DXQUATERNION rotate, float time);
	virtual ~GEKeyframe();

	/// 获得变换矩阵
	inline D3DXMATRIX getMatrix()
	{
		if(mNeedRecomputeMatrix)
		{	
			recomputeMatrix();
		}
		return mMatrix;
	}
	/// 重新计算变换矩阵
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

	/// 将两个关键帧的变换叠加
// 	inline GEKeyframe operator*(GEKeyframe& kf)
// 	{
// 		return GEKeyframe(mScale * kf.mScale, mTranslation * kf.mTranslation, 
// 			mRotation * kf.mRotation, 0.0f);
// 	}

	/// 获得放缩
	inline D3DXVECTOR3 getScale() {	return mScale; }
	/// 获得平移
	inline D3DXVECTOR3 getTranslation() { return mTranslation; }
	/// 获得旋转
	inline D3DXQUATERNION getRotation()	{ return mRotation; }
	/// 设置此关键帧的时间
	inline void  setTime(float time) { mTime=time; }
	/// 获得此关键帧的时间
	inline float getTime() { return mTime; }
	/// 设置放缩
	inline void setScale(D3DXVECTOR3 scale)
	{
        mScale=scale;
		mNeedRecomputeMatrix=true;
	}
	/// 设置平移
	inline void setTranslation(D3DXVECTOR3 translate)
	{
		mTranslation=translate;
		mNeedRecomputeMatrix=true;
	}
	/// 设置旋转
	inline void setRotation(D3DXQUATERNION rotate)
	{
		mRotation=rotate;
		mNeedRecomputeMatrix=true;
	}
	/**
	 * 把自己和kf2作线性插值和球形线性插值，返回插值后的变换（保存在一个关键帧对象中）
	 * @param weight kf2的权重
	 */	
	GEKeyframe interpolate(GEKeyframe& kf2, float weight);	

protected:
	D3DXVECTOR3    mScale;					///< 放缩
	D3DXVECTOR3    mTranslation;			///< 平移
	D3DXQUATERNION mRotation;				///< 旋转
	D3DXMATRIX    mMatrix;					///< 变换矩阵
    bool       mNeedRecomputeMatrix;	///< 是否需要更新矩阵的标志
	float	   mTime;					///< 此关键帧的时间
};
