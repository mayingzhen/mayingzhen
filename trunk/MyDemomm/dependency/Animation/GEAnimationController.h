#pragma once


#include<string>
using namespace std;

class GESkeleton;
class GEBone;
class GEAnimationSet;
class GESkeleton;
class GEAnimation;

/**
 * 封装与动画相关的数据和操作。.
 * 一个GECharacter涉及动画的操作都经过它的动画控制器完成。动画控制器拥有动画集和这些动画所
 * 使用的骨架，并封装设置动画的相关操作。
 */
class GEAnimationController
{
public:
	GEAnimationController(void);
	virtual ~GEAnimationController(void);

	/**
	 * 设置当前播放的动画.
	 * @param animationName 要播放的动画名
	 * @param beginTime 从该动画序列的beginTime时间开始播放
	 */
	bool SetActiveAnimation(const string& animationName, float beginTime=0.f);
	/**
	 * 获得当前动画 
	 */
	inline GEAnimation* GetActiveAnimation() { return mActiveAnimation; }
	/**
	 * 设置当前动画的时间放缩.
	 * 获取关键帧的时候将当前动画的当前时间乘上这个时间放缩后再找前后关键帧。
	 */
	inline void SetTimeScale(float scale=1.0f) { mActiveTimeScale=scale; }

	/**
	 * 设置当前动画的时间 
	 */
	inline void SetTime(float time)
	{ mActiveAnimationTime=time; }
	/** 
	 * 得到当前动画的时间 
	 */
	inline float GetActiveAnimationTime() 
	{ return mActiveAnimationTime; }

	/**
	 * 将动画集捆绑到这个控制器上 
	 */
	void BindAnimationSet(GEAnimationSet* set)
	{ mAnimationSet=set; }
	/**
	 * 当骨架捆绑到这个控制器上 
	 */
	inline void BindSkeleton(GESkeleton* skn)
	{ mSkeleton=skn; }

	/**
	 * 在指定的时间里从当前动画混合过渡到另一段动画。.
	 * 如果还未blend完当前动画或目标动画就已经到末端，那么将使用其最后一帧来作blend。
	 * @param blendTime 要花多长时间从当前动画过渡到目标动画
	 * @param timeScale 用于目标动画的时间缩放
	 */
	bool BlendTo(const string& targetAnimationName, float blendTime, float timeScale=1.0f);
	/**
	 * 从现在到当前动画结束的这段时间内完成混合
	 * @param targetAnimationName 混合的目标动画
	 * @param timeScale 用于目标动画的时间放缩
	 */		
	bool BlendToEnd(const string& targetAnimationName, float timeScale=1.0f);

	/**
	* 从现在到当前动画平稳过渡
	* @param targetAnimationName 混合的目标动画
	* @param translateTime 变换结束时，目标动画的当前帧时刻（原始帧，即不考虑时间缩放）
	* @param timeScale 用于目标动画的时间放缩
	* @note: 如果 translateTime > 目标动画的总长度，则 translateTime = 目标动画的总长度
	*/		
	bool TranslateTo(const string& targetAnimationName, float translateTime ,float timeScale = 1.0f);


	void UpdateSkeleton(); ///< 根据当前动画和当前时间更新骨架中所有骨头的相对坐标变换
	/**
	 * 根据当前动画和当前时间更新该骨头及其所有子节点的相对坐标变换
	 * @param pbone 要更新姿态的骨头的指针
	 */
	void UpdateRelativeTransform(GEBone* pbone); 
	/**
	* 根据当前动画和当前时间更新该骨头及其所有子节点的相对坐标变换
	* @param boneIndex 要更新姿态的骨头的索引号
	*/
	void UpdateRelativeTransform(unsigned short boneIndex);
    
	/// 当前动画播放完了吗？如果是循环播放的话这个函数恒为false
	bool IsAnimationEnded();

	/**
	 * 设置但前动画是否要循环
	 * @param l 为真时循环，反之播放到动画尽头即停止
	 */
	inline void SetLoop(bool l) { mLoop=l; }

	/** 当前动画是否循环播放 */
	inline bool IsLoop() { return mLoop; }

	/** 返回是否动画将结束 */
	bool Update(float deltaTime);

	inline GESkeleton* GetSkeleton() const {	return mSkeleton;	}

	void SetActiveAnimationNull();

	inline string GEAnimationController::GetActiveAnimationName();

protected:
	bool  mLoop;						///< 当前的动画是否循环播放

	bool  mBeingBlending;				///< 是否正在blending
	float mBlendInterval;				///< blend要经过多长时间
	float mBlendingTime;				///< 已经开始blend多久了
	
	bool  mBeingTranslating;			///< 是否正在Translating
	float mTranslateInterval;			///< Translating要经过多长时间
	float mTranslatingTime;				///< 已经开始Translate多久了

	float mActiveAnimationTime;			///< 当前动画的时间
	float mActiveTimeScale;				///< 当前动画的时间缩放
	string mActiveAnimationName;		///< 当前动画的名称，NOTE：当该动画结束时，名称还是保留的。

	float mBlendedAnimationTime;		///< 要blend过去的动画的当前时间
	float mBlendedTimeScale;			///< blend的目标动画的时间缩放

	float mActiveAnimationLength;		///< 当前动画未用mActiveTimeScale放缩过的长度
	float mBlendedAnimationLength;		///< blend的目标动画未用mActiveTimeScale放缩过的长度

	float mTranslatedAnimationTime;		///< Translate目标动画的目标帧时间
	float mTranslatedAnimationLength;	///< Translate目标动画的总长度
	float mTranslatedTimeScale;			///< Translate的目标动画的时间缩放


	GEAnimation* mActiveAnimation;		///< 当前动画
	GEAnimation* mBlendedAnimation;		///< blend的目标动画
	GEAnimation* mTranslatedAnimation;	///< Translate的目标动画


	GEAnimationSet* mAnimationSet;		///< 该动画控制器拥有的动画集
	GESkeleton*     mSkeleton;			///< 该动画控制器中动画所使用的骨架

private:
	/**
	* 动画播放的时间增加一个间隔。.
	* 每帧都要调用这个函数，增加上一帧到这帧的时间间隔。
	* @param delta 增加的时间间隔
	* @return bool 是否动画将结束
	*/
	bool AddTime(float delta);	

};

inline void GEAnimationController::SetActiveAnimationNull()
{
	mActiveAnimation = NULL;
}


/** 返回当前动画的名字
 *	@note: 当前无动画时，返回上一个动画的名字
 */
inline string GEAnimationController::GetActiveAnimationName()
{
	return mActiveAnimationName;
}

inline bool GEAnimationController::IsAnimationEnded()
{
	if(!mBeingBlending)	// 现在没有作blending
	{
		if(mLoop)
			return false;
		else
			return (mActiveAnimationTime>=mActiveAnimationLength)?true:false;
	}
	else // 还在blending的话就认为未播放完
	{
		return false;
	}
}