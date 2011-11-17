#pragma once

#include<string>
#include <vector>
using namespace std;

#include"GEKeyframe.h"
class GESkeleton;

/**
 *  
 * ���⣺��ͷ���洢����Ա任������ʼ�任
 */
class GEBone
{
public:
	/**
	 *  @param parent ���Ϊ��1����ʾ�ù�ͷΪ���ڵ� 
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
	inline void setInitialTransform(const D3DXVECTOR3& translate, const D3DXQUATERNION& rotate) ///< scaleĬ��ΪVector3(1,1,1)
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
	
    void update();	// update�Լ����ӽڵ��absolute transformation
	void updateWithInitialTransform();
	
	/**
	 * ȡ�ý��ܸù�ͷӰ��Ķ���任����֡����λ�õľ���.
	 * mInverseInitialTransform * Absolute Transform 
	 */
	D3DXMATRIX getFullTransform();

	/// ���Լ��������ӽڵ��relative transform��ɳ�ʼ�任
	void setToInitialTransform();

	void setToNoTransform();

	/// ���ݵ�ǰ����Ա任�����������任��������mInverseInitialTransform�������ӽڵ㡣
	void computeInverseInitialTransform();

protected:
	/**
	 * ��ͷ��������.
	 * ע�⣺mIndex��mParentIndexΪ��ͬ���ͣ��Ա�mParentIndex�ܱ�ʾ�޸��ڵ�����
	 */
	unsigned short mIndex; 
	string mName;			///< ��ͷ��
	GESkeleton* mSkeleton;	///< ��ͷ�����ĹǼ�
	
	/**
	 * �ù�ͷ�ĸ����������š����Ϊ-1����ʾ�ù�ͷΪ���ڵ㣬�޸���㡣
	 */
	int mParentIndex;	
	vector<unsigned short> mChildrenIndex;	///< �����ӽ���������
	
	D3DXVECTOR3    mInitialScale;			///< ��ʼ�任�еķ���
	D3DXVECTOR3    mInitialTranslation;		///< ��ʼ�任�е�ƽ��
	D3DXQUATERNION mInitialRotation;		///< ��ʼ�任�е���ת
	D3DXVECTOR3    mRelativeScale;			///< ĳʱ����Ա任�еķ���
	D3DXVECTOR3    mRelativeTranslation;	///< ĳʱ����Ա任�е�ƽ��
	D3DXQUATERNION mRelativeRotation;		///< ĳʱ����Ա任�е���ת
	D3DXVECTOR3    mAbsoluteScale;			///< ĳʱ�̼������ľ��Ա任�еķ���
	D3DXVECTOR3    mAbsoluteTranslation;	///< ĳʱ�̼������ľ��Ա任�е�ƽ��
	D3DXQUATERNION mAbsoluteRotation;		///< ĳʱ�̼������ľ��Ա任�е���ת

	D3DXMATRIX    mInverseInitialTransform; ///< ��ʼ�任����任�����ڻ������ձ任���� 

	bool	   mFullTransformNeedUpdate; ///< ���ձ任������Ҫ���µı�־
	D3DXMATRIX	   mFullTransform;			 ///< ���ձ任����

};
