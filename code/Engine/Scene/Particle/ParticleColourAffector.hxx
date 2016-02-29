#include "S3EngineCommon.h"
#include "ParticleColourAffector.h"

namespace ma
{


CParticleColourAffector::CParticleColourAffector()
: mTargetColor(ColourValue::ZERO), CParticleAffector()
{
	mFadeOutTime = 1.f;
}

CParticleColourAffector::~CParticleColourAffector(void)
{
}

void CParticleColourAffector::Affect( Real now, Real timediff, LST_PARTICLE& lstParticles )
{
	if (!m_bEnabled)
		return;
	float d;

	for(LST_PARTICLE::iterator iter = lstParticles.begin();iter != lstParticles.end();++iter)
	{
		SParticle& p = *iter;
		float fTime = now - p.startTime;
		if (fTime <= mFadeOutTime)
		{
			d = fTime / mFadeOutTime;
			p.color.r = Math::Lerp<Real>(p.startColor.r, mTargetColor.r, d);
			p.color.g = Math::Lerp<Real>(p.startColor.g, mTargetColor.g, d);
			p.color.b = Math::Lerp<Real>(p.startColor.b, mTargetColor.b, d);
			p.color.a = Math::Lerp<Real>(p.startColor.a, mTargetColor.a, d);
		}
        else
        {
            p.color = mTargetColor;
        }
	}
}

RefPtr<CParticleColourAffector> CreateColourAffector()
{
	return new CParticleColourAffector();
}
}