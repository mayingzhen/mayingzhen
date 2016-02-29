#include "S3EngineCommon.h"
#include "ParticleColourFader2Affector.h"

namespace ma
{

CParticleColourFader2Affector::CParticleColourFader2Affector(void)
{
	mTimeChange = 0;
	mColourAdder0 = mColourAdder1 = Vector4::ZERO;
}

CParticleColourFader2Affector::~CParticleColourFader2Affector(void)
{
}

void CParticleColourFader2Affector::Affect( Real now, Real timediff, LST_PARTICLE& lstParticles )
{
	if( !m_bEnabled )
		return;

	Vector4 delta0 = mColourAdder0*timediff;
	Vector4 delta1 = mColourAdder1*timediff;
	for(LST_PARTICLE::iterator iter = lstParticles.begin();iter != lstParticles.end();++iter)
	{
		SParticle& p = *iter;
		if (now - p.startTime <= mTimeChange)
		{
			p.color = p.color + (ColourValue&)delta0;
			p.color.saturate();
		}
		else
		{
			p.color = p.color + (ColourValue&)delta1;
			p.color.saturate();
		}
		
	}
}

RefPtr<CParticleColourFader2Affector> CreateColourFader2Affector()
{
	return new CParticleColourFader2Affector;
}

}