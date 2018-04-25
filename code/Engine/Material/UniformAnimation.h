#pragma once

namespace ma
{
	class UniformAnimation : public Serializable, public Animatable
	{
	public:
		UniformAnimation();
		~UniformAnimation();

		void AddKeyFrame(int nKeyFrame, const Any& any);
		uint32_t GetNumKeyFrames() const{return (uint32_t)m_mapKeyFrame.size();}
		std::pair<int, const Any*> GetKeyFrameByIndex(int n) const;

		void BuildFrames();
		int GetNumFrames();
		const Any& GetValueByIndex(int nFrame) {return m_vecAny[nFrame];}
		const Any& GetValue();

		bool IsEqual(const UniformAnimation* p) const;
	private:
		typedef map<int, Any> MAP_KEYFRAME;
		MAP_KEYFRAME m_mapKeyFrame;

		typedef vector<Any> VEC_ANY;
		VEC_ANY m_vecAny;
	};

	RefPtr<UniformAnimation> CreateUniformAnimation();

}
