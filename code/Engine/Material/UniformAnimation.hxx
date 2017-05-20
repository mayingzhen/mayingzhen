#include "UniformAnimation.h"

namespace ma
{
	UniformAnimation::UniformAnimation()
	{
		m_state = State_Playing;
	}

	UniformAnimation::~UniformAnimation()
	{
	}

	void UniformAnimation::AddKeyFrame(int nKeyFrame, const Any& any)
	{
		m_mapKeyFrame[nKeyFrame] = any;
	}

	// ---------------------------------------------------------------------
	// UniformAnimation
	// ---------------------------------------------------------------------
	std::pair<int, const Any*> UniformAnimation::GetKeyFrameByIndex(int n) const
	{
		int m = 0;
		for (MAP_KEYFRAME::const_iterator iter = m_mapKeyFrame.begin();iter != m_mapKeyFrame.end();++iter)
		{
			if (m++ == n)
			{
				return std::pair<int, const Any*>(iter->first, &iter->second);
			}
		}

		ASSERT(false);
		return std::pair<int, const Any*>(0, NULL);
	}

	void UniformAnimation::BuildFrames()
	{
		ASSERT(!m_mapKeyFrame.empty());
		ASSERT(m_mapKeyFrame.find(0) != m_mapKeyFrame.end());
		int nNumFrames = m_mapKeyFrame.rbegin()->first+1;
		m_vecAny.resize(nNumFrames);

		int nPreIndex = 0;
		Any preFrame;
		for (MAP_KEYFRAME::const_iterator iter = m_mapKeyFrame.begin();iter != m_mapKeyFrame.end();++iter)
		{
			int nIndex = iter->first;
			const Any& frame = iter->second;
			if (nIndex == 0)
			{
				m_vecAny[0] = frame;
			}
			else
			{
				const std::type_info& type = preFrame.getType();
				if (type == typeid(float))
				{
					float pre = any_cast<float>(preFrame);
					float cur = any_cast<float>(frame);
					for (int i = nPreIndex+1;i<= nIndex;++i)
					{
						float fPercent = (i-nPreIndex)/float(nIndex - nPreIndex);
						m_vecAny[i] = Any(Math::Lerp<float>(pre, cur, fPercent));
					}
				}
				else if (type == typeid(Vector2))
				{
					Vector2 pre = any_cast<Vector2>(preFrame);
					Vector2 cur = any_cast<Vector2>(frame);
					for (int i = nPreIndex+1;i<= nIndex;++i)
					{
						float fPercent = (i-nPreIndex)/float(nIndex - nPreIndex);
						m_vecAny[i] = Any(Math::Lerp<Vector2>(pre, cur, fPercent));
					}
				}
				else if (type == typeid(Vector3))
				{
					Vector3 pre = any_cast<Vector3>(preFrame);
					Vector3 cur = any_cast<Vector3>(frame);
					for (int i = nPreIndex+1;i<= nIndex;++i)
					{
						float fPercent = (i-nPreIndex)/float(nIndex - nPreIndex);
						m_vecAny[i] = Any(Math::Lerp<Vector3>(pre, cur, fPercent));
					}
				}
				else if (type == typeid(Vector4))
				{
					Vector4 pre = any_cast<Vector4>(preFrame);
					Vector4 cur = any_cast<Vector4>(frame);
					for (int i = nPreIndex+1;i<= nIndex;++i)
					{
						float fPercent = (i-nPreIndex)/float(nIndex - nPreIndex);
						m_vecAny[i] = Any(Math::Lerp<Vector4>(pre, cur, fPercent));
					}
				}
				else if (type == typeid(ColourValue))
				{
					ColourValue pre = any_cast<ColourValue>(preFrame);
					ColourValue cur = any_cast<ColourValue>(frame);
					for (int i = nPreIndex+1;i<= nIndex;++i)
					{
						float fPercent = (i-nPreIndex)/float(nIndex - nPreIndex);
						m_vecAny[i] = Any(Math::Lerp<ColourValue>(pre, cur, fPercent));
					}
				}
				else
				{
					ASSERT(false);
				}
			}

			preFrame = frame;
			nPreIndex = nIndex;
		}
	}

	int UniformAnimation::GetNumFrames()
	{
		ASSERT(!m_mapKeyFrame.empty());
		ASSERT(m_mapKeyFrame.find(0) != m_mapKeyFrame.end());
		int nNumFrames = m_mapKeyFrame.rbegin()->first+1;
		return nNumFrames;
	}


	const Any& UniformAnimation::GetValue() 
	{
		int nFrame =  (int)( CalcLocalTime() / GetInterval() ) % GetNumFrames();

		return GetValueByIndex(nFrame);
	}

	bool UniformAnimation::IsEqual(const UniformAnimation* p) const
	{
		if (m_mapKeyFrame.size() != p->m_mapKeyFrame.size())
		{
			return false;
		}

		for (MAP_KEYFRAME::const_iterator iter = m_mapKeyFrame.begin(), iter1 = p->m_mapKeyFrame.begin();
			iter != m_mapKeyFrame.end() && iter1 != p->m_mapKeyFrame.end();++iter, ++iter1)
		{
			if(iter->first != iter->first)
				return false;

			const Any& v0 = iter->second;
			const Any& v1 = iter1->second;
			if (v0.getType() != v1.getType())
				return false;

			const std::type_info& type = v0.getType();
			if (type == typeid(float))
			{
				if(any_cast<float>(v0) != any_cast<float>(v1))
					return false;
			}
			else if (type == typeid(Vector2))
			{
				if(any_cast<Vector2>(v0) != any_cast<Vector2>(v1))
					return false;
			}
			else if (type == typeid(Vector3))
			{
				if(any_cast<Vector3>(v0) != any_cast<Vector3>(v1))
					return false;
			}
			else if (type == typeid(Vector4))
			{
				if(any_cast<Vector4>(v0) != any_cast<Vector4>(v1))
					return false;
			}
			else if (type == typeid(ColourValue))
			{
				if(any_cast<ColourValue>(v0) != any_cast<ColourValue>(v1))
					return false;
			}
			else
			{
				ASSERT(false);
				return false;
			}
		}

		return true;
	}

	RefPtr<UniformAnimation> CreateUniformAnimation()
	{
		return new UniformAnimation;
	}
}