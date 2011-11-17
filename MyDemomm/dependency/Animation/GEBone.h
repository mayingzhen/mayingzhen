#pragma once

#include<string>
#include <vector>
using namespace std;

#include"GEKeyframe.h"
class GESkeleton;

/**
 *  
 * 主意：骨头所存储的相对变换包含初始变换
 */
class GEBone
{
public:
	/**
	 *  @param parent 如果为－1，表示该骨头为根节点 
	 */
	inline GEBone(unsigned short index, const string& name, GESkeleton* creator, int parent=-1)
		: mIndex(index), mName(name), mSkeleton(creator), mParentIndex(parent),	mFullTransformNeedUpdate(true)
	{  }
	virtual ~GEBone(void);

	inline void setIndex(unsigned short index)	{ mIndex=index; }
	inline unsigned short getIndex() const		{ return mIndex; }	

	inline void  setName(const string& name)	{ mName=name; }
	inline const string& getName() const		{ return mName; }

    void  setParent(int index) ;
	inline int getParent() const				{ return mParentIndex; }

	inline void  addChild(unsigned short index) { mChildrenIndex.push_back(index); }
	inline const vector<unsigned short>& getChildren() const { return mChildrenIndex; }

	void   setInitialTransform(const D3DXVECTOR3& scale, const D3DXVECTOR3& translate, const D3DXQUATERNION& rotate);
	inline void setInitialTransform(const D3DXVECTOR3& translate, const D3DXQUATERNION& rotate) ///< scale默认为Vector3(1,1,1)
	{ this->setInitialTransform( D3DXVECTOR3(1,1,1), translate, rotate); }

	void setRelativeTransform(const D3DXVECTOR3& scale, const D3DXVECTOR3& translate, const D3DXQUATERNION& rotate);
	void setRelativeTransform(GEKeyframe& kf);

	inline const D3DXVECTOR3&		getInitialScale()		const	{ return mInitialScale;			}	
	inline const D3DXVECTOR3&		getInitialTranslation()	const	{ return mInitialTranslation;	}
	inline const D3DXQUATERNION&	getInitialRotation()	const	{ return mInitialRotation;		}
	inline const D3DXVECTOR3&		getRelativeScale()		const	{ return mRelativeScale;		}	
	inline const D3DXVECTOR3&		getRelativeTranslation()const	{ return mRelativeTranslation;	}
	inline const D3DXQUATERNION&	getRelativeRotation()	const	{ return mRelativeRotation;		}
	inline const D3DXVECTOR3&		getAbsoluteScale()		const	{ return mAbsoluteScale;		}	
	inline const D3DXVECTOR3&		getAbsoluteTranslation()const	{ return mAbsoluteTranslation;	}
	inline const D3DXQUATERNION&	getAbsoluteRotation()	const	{ return mAbsoluteRotation;		}
	
    void update();	// update自己和子节点的absolute transformation
	void updateWithInitialTransform();
	
	/**
	 * 取得将受该骨头影响的顶点变换至该帧最终位置的矩阵.
	 * mInverseInitialTransform * Absolute Transform 
	 */
	D3DXMATRIX getFullTransform();

	/// 将自己和所有子节点的relative transform设成初始变换
	void setToInitialTransform();

	void setToNoTransform();

	/// 根据当前的相对变换计算出绝对逆变换，并赋给mInverseInitialTransform；遍历子节点。
	void computeInverseInitialTransform();

protected:
	/**
	 * 骨头的索引号.
	 * 注意：mIndex和mParentIndex为不同类型，以便mParentIndex能表示无父节点的情况
	 */
	unsigned short mIndex; 
	string mName;			///< 骨头名
	GESkeleton* mSkeleton;	///< 骨头所属的骨架
	
	/**
	 * 该骨头的父结点的索引号。如果为-1，表示该骨头为根节点，无父结点。
	 */
	int mParentIndex;	
	vector<unsigned short> mChildrenIndex;	///< 所有子结点的索引号
	
	D3DXVECTOR3    mInitialScale;			///< 初始变换中的放缩
	D3DXVECTOR3    mInitialTranslation;		///< 初始变换中的平移
	D3DXQUATERNION mInitialRotation;		///< 初始变换中的旋转
	D3DXVECTOR3    mRelativeScale;			///< 某时刻相对变换中的放缩
	D3DXVECTOR3    mRelativeTranslation;	///< 某时刻相对变换中的平移
	D3DXQUATERNION mRelativeRotation;		///< 某时刻相对变换中的旋转
	D3DXVECTOR3    mAbsoluteScale;			///< 某时刻计算所的绝对变换中的放缩
	D3DXVECTOR3    mAbsoluteTranslation;	///< 某时刻计算所的绝对变换中的平移
	D3DXQUATERNION mAbsoluteRotation;		///< 某时刻计算所的绝对变换中的旋转

	D3DXMATRIX    mInverseInitialTransform; ///< 初始变换的逆变换，用于或者最终变换矩阵 

	bool	   mFullTransformNeedUpdate; ///< 最终变换矩阵显要更新的标志
	D3DXMATRIX	   mFullTransform;			 ///< 最终变换矩阵

};
