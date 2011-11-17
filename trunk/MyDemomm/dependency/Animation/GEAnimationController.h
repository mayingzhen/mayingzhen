#pragma once


#include<string>
using namespace std;

class GESkeleton;
class GEBone;
class GEAnimationSet;
class GESkeleton;
class GEAnimation;

/**
 * ��װ�붯����ص����ݺͲ�����.
 * һ��GECharacter�漰�����Ĳ������������Ķ�����������ɡ�����������ӵ�ж���������Щ������
 * ʹ�õĹǼܣ�����װ���ö�������ز�����
 */
class GEAnimationController
{
public:
	GEAnimationController(void);
	virtual ~GEAnimationController(void);

	/**
	 * ���õ�ǰ���ŵĶ���.
	 * @param animationName Ҫ���ŵĶ�����
	 * @param beginTime �Ӹö������е�beginTimeʱ�俪ʼ����
	 */
	bool SetActiveAnimation(const string& animationName, float beginTime=0.f);
	/**
	 * ��õ�ǰ���� 
	 */
	inline GEAnimation* GetActiveAnimation() { return mActiveAnimation; }
	/**
	 * ���õ�ǰ������ʱ�����.
	 * ��ȡ�ؼ�֡��ʱ�򽫵�ǰ�����ĵ�ǰʱ��������ʱ�����������ǰ��ؼ�֡��
	 */
	inline void SetTimeScale(float scale=1.0f) { mActiveTimeScale=scale; }

	/**
	 * ���õ�ǰ������ʱ�� 
	 */
	inline void SetTime(float time)
	{ mActiveAnimationTime=time; }
	/** 
	 * �õ���ǰ������ʱ�� 
	 */
	inline float GetActiveAnimationTime() 
	{ return mActiveAnimationTime; }

	/**
	 * ����������������������� 
	 */
	void BindAnimationSet(GEAnimationSet* set)
	{ mAnimationSet=set; }
	/**
	 * ���Ǽ���������������� 
	 */
	inline void BindSkeleton(GESkeleton* skn)
	{ mSkeleton=skn; }

	/**
	 * ��ָ����ʱ����ӵ�ǰ������Ϲ��ɵ���һ�ζ�����.
	 * �����δblend�굱ǰ������Ŀ�궯�����Ѿ���ĩ�ˣ���ô��ʹ�������һ֡����blend��
	 * @param blendTime Ҫ���೤ʱ��ӵ�ǰ�������ɵ�Ŀ�궯��
	 * @param timeScale ����Ŀ�궯����ʱ������
	 */
	bool BlendTo(const string& targetAnimationName, float blendTime, float timeScale=1.0f);
	/**
	 * �����ڵ���ǰ�������������ʱ������ɻ��
	 * @param targetAnimationName ��ϵ�Ŀ�궯��
	 * @param timeScale ����Ŀ�궯����ʱ�����
	 */		
	bool BlendToEnd(const string& targetAnimationName, float timeScale=1.0f);

	/**
	* �����ڵ���ǰ����ƽ�ȹ���
	* @param targetAnimationName ��ϵ�Ŀ�궯��
	* @param translateTime �任����ʱ��Ŀ�궯���ĵ�ǰ֡ʱ�̣�ԭʼ֡����������ʱ�����ţ�
	* @param timeScale ����Ŀ�궯����ʱ�����
	* @note: ��� translateTime > Ŀ�궯�����ܳ��ȣ��� translateTime = Ŀ�궯�����ܳ���
	*/		
	bool TranslateTo(const string& targetAnimationName, float translateTime ,float timeScale = 1.0f);


	void UpdateSkeleton(); ///< ���ݵ�ǰ�����͵�ǰʱ����¹Ǽ������й�ͷ���������任
	/**
	 * ���ݵ�ǰ�����͵�ǰʱ����¸ù�ͷ���������ӽڵ���������任
	 * @param pbone Ҫ������̬�Ĺ�ͷ��ָ��
	 */
	void UpdateRelativeTransform(GEBone* pbone); 
	/**
	* ���ݵ�ǰ�����͵�ǰʱ����¸ù�ͷ���������ӽڵ���������任
	* @param boneIndex Ҫ������̬�Ĺ�ͷ��������
	*/
	void UpdateRelativeTransform(unsigned short boneIndex);
    
	/// ��ǰ�������������������ѭ�����ŵĻ����������Ϊfalse
	bool IsAnimationEnded();

	/**
	 * ���õ�ǰ�����Ƿ�Ҫѭ��
	 * @param l Ϊ��ʱѭ������֮���ŵ�������ͷ��ֹͣ
	 */
	inline void SetLoop(bool l) { mLoop=l; }

	/** ��ǰ�����Ƿ�ѭ������ */
	inline bool IsLoop() { return mLoop; }

	/** �����Ƿ񶯻������� */
	bool Update(float deltaTime);

	inline GESkeleton* GetSkeleton() const {	return mSkeleton;	}

	void SetActiveAnimationNull();

	inline string GEAnimationController::GetActiveAnimationName();

protected:
	bool  mLoop;						///< ��ǰ�Ķ����Ƿ�ѭ������

	bool  mBeingBlending;				///< �Ƿ�����blending
	float mBlendInterval;				///< blendҪ�����೤ʱ��
	float mBlendingTime;				///< �Ѿ���ʼblend�����
	
	bool  mBeingTranslating;			///< �Ƿ�����Translating
	float mTranslateInterval;			///< TranslatingҪ�����೤ʱ��
	float mTranslatingTime;				///< �Ѿ���ʼTranslate�����

	float mActiveAnimationTime;			///< ��ǰ������ʱ��
	float mActiveTimeScale;				///< ��ǰ������ʱ������
	string mActiveAnimationName;		///< ��ǰ���������ƣ�NOTE�����ö�������ʱ�����ƻ��Ǳ����ġ�

	float mBlendedAnimationTime;		///< Ҫblend��ȥ�Ķ����ĵ�ǰʱ��
	float mBlendedTimeScale;			///< blend��Ŀ�궯����ʱ������

	float mActiveAnimationLength;		///< ��ǰ����δ��mActiveTimeScale�������ĳ���
	float mBlendedAnimationLength;		///< blend��Ŀ�궯��δ��mActiveTimeScale�������ĳ���

	float mTranslatedAnimationTime;		///< TranslateĿ�궯����Ŀ��֡ʱ��
	float mTranslatedAnimationLength;	///< TranslateĿ�궯�����ܳ���
	float mTranslatedTimeScale;			///< Translate��Ŀ�궯����ʱ������


	GEAnimation* mActiveAnimation;		///< ��ǰ����
	GEAnimation* mBlendedAnimation;		///< blend��Ŀ�궯��
	GEAnimation* mTranslatedAnimation;	///< Translate��Ŀ�궯��


	GEAnimationSet* mAnimationSet;		///< �ö���������ӵ�еĶ�����
	GESkeleton*     mSkeleton;			///< �ö����������ж�����ʹ�õĹǼ�

private:
	/**
	* �������ŵ�ʱ������һ�������.
	* ÿ֡��Ҫ�������������������һ֡����֡��ʱ������
	* @param delta ���ӵ�ʱ����
	* @return bool �Ƿ񶯻�������
	*/
	bool AddTime(float delta);	

};

inline void GEAnimationController::SetActiveAnimationNull()
{
	mActiveAnimation = NULL;
}


/** ���ص�ǰ����������
 *	@note: ��ǰ�޶���ʱ��������һ������������
 */
inline string GEAnimationController::GetActiveAnimationName()
{
	return mActiveAnimationName;
}

inline bool GEAnimationController::IsAnimationEnded()
{
	if(!mBeingBlending)	// ����û����blending
	{
		if(mLoop)
			return false;
		else
			return (mActiveAnimationTime>=mActiveAnimationLength)?true:false;
	}
	else // ����blending�Ļ�����Ϊδ������
	{
		return false;
	}
}