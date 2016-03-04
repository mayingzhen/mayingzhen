#include "S3EngineCommon.h"
#include "ParticleColourFader2Affector.h"

namespace ma
{

ParticleColourFader2Affector::ParticleColourFader2Affector(void)
{
	mTimeChange = 0;
	mColourAdder0 = mColourAdder1 = Vector4::ZERO;
}

ParticleColourFader2Affector::~ParticleColourFader2Affector(void)
{
}

void ParticleColourFader2Affector::Affect( Real now, Real timediff, LST_PARTICLE& lstParticles )
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

RefPtr<ParticleColourFader2Affector> CreateColourFader2Affector()
{
	return new ParticleColourFader2Affector;
}

}