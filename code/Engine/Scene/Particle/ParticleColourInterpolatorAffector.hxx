#include "S3EngineCommon.h"
#include "ParticleColourInterpolatorAffector.h"

namespace ma
{

CParticleColourInterpolatorAffector::CParticleColourInterpolatorAffector(void)
{
    m_nCycles = 1;
	for (int i = 0;i< MAX_STAGES;++i)
	{
		mColourAdj[i]	= ColourValue(1.f, 1.f, 1.f, 1.0f);
		mTimeAdj[i]		= 1.0f;
	}
}

CParticleColourInterpolatorAffector::~CParticleColourInterpolatorAffector(void)
{
}

void CParticleColourInterpolatorAffector::Affect( Real now, Real timediff, LST_PARTICLE& lstParticles )
{
	if( !m_bEnabled )
		return;

	for(LST_PARTICLE::iterator iter = lstParticles.begin();iter != lstParticles.end();++iter)
	{
		SParticle& p = *iter;

        Real fPercent;
        if (m_nCycles <= 1)
            fPercent = (now - p.startTime) / (p.endTime - p.startTime); 
        else
        {
            float fEveryCycleTime = (p.endTime - p.startTime)/m_nCycles;
            fPercent = fmod(now - p.startTime, fEveryCycleTime)/fEveryCycleTime;
        }
		
		if (fPercent <= mTimeAdj[0])
		{
			p.color = mColourAdj[0];
		}
		else if (fPercent >= mTimeAdj[MAX_STAGES - 1])
		{
			p.color = mColourAdj[MAX_STAGES - 1];
		}
		else
		{
			for (int i = 0;i< MAX_STAGES-1;++i)
			{
				if (fPercent >= mTimeAdj[i] && fPercent < mTimeAdj[i+1])
				{
					fPercent -= mTimeAdj[i];
					fPercent /= (mTimeAdj[i+1] - mTimeAdj[i]);
					p.color.r = (mColourAdj[i+1].r * fPercent) + (mColourAdj[i].r*(1.f-fPercent));
					p.color.g = (mColourAdj[i+1].g * fPercent) + (mColourAdj[i].g*(1.f-fPercent));
					p.color.b = (mColourAdj[i+1].b * fPercent) + (mColourAdj[i].b*(1.f-fPercent));
					p.color.a = (mColourAdj[i+1].a * fPercent) + (mColourAdj[i].a*(1.f-fPercent));
					break;
				}
			}
		}
	}
}

RefPtr<CParticleColourInterpolatorAffector> CreateColourInterpolatorAffector()
{
	return new CParticleColourInterpolatorAffector();
}
}