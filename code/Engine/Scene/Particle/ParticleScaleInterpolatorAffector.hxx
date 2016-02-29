#include "S3EngineCommon.h"
#include "ParticleScaleInterpolatorAffector.h"

namespace ma
{

CParticleScaleInterpolatorAffector::CParticleScaleInterpolatorAffector(void)
{
    m_nCycles = 1;
	for (int i = 0;i< MAX_STAGES;++i)
	{
		mScaleAdj[i]	= Vector2::UNIT_SCALE;
		mTimeAdj[i]		= 1.0f;
	}
}

CParticleScaleInterpolatorAffector::~CParticleScaleInterpolatorAffector(void)
{
}

void CParticleScaleInterpolatorAffector::Affect( Real now, Real timediff, LST_PARTICLE& lstParticles )
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
			p.size.width = p.startSize.width*mScaleAdj[0].x;
			p.size.height = p.startSize.height*mScaleAdj[0].y;
		}
		else if (fPercent >= mTimeAdj[MAX_STAGES - 1])
		{
			p.size.width = p.startSize.width*mScaleAdj[MAX_STAGES - 1].x;
			p.size.height = p.startSize.height*mScaleAdj[MAX_STAGES - 1].y;
		}
		else
		{
			for (int i = 0;i< MAX_STAGES-1;++i)
			{
				if (fPercent >= mTimeAdj[i] && fPercent < mTimeAdj[i+1])
				{
					fPercent -= mTimeAdj[i];
					fPercent /= (mTimeAdj[i+1] - mTimeAdj[i]);
					Vector2 vScale = (mScaleAdj[i+1] * fPercent) + (mScaleAdj[i]*(1.f-fPercent));
					p.size.width = p.startSize.width*vScale.x;
					p.size.height = p.startSize.height*vScale.y;
					break;
				}
			}
		}
	}
}

RefPtr<CParticleScaleInterpolatorAffector> CreateScaleInterpolatorAffector()
{
	return new CParticleScaleInterpolatorAffector();
}

}