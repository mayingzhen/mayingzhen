#include "S3EngineCommon.h"
#include "ParticleColourFaderAffector.h"

namespace ma
{

ParticleColourFaderAffector::ParticleColourFaderAffector()
:mColourAdder(Vector4::ZERO)
{
}

ParticleColourFaderAffector::~ParticleColourFaderAffector(void)
{
}

void ParticleColourFaderAffector::Affect( Real now, Real timediff, LST_PARTICLE& lstParticles )
{
	if( !m_bEnabled )
		return;

	Vector4 delta = mColourAdder*timediff;
	for(LST_PARTICLE::iterator iter = lstParticles.begin();iter != lstParticles.end();++iter)
	{
		SParticle& p = *iter;
		p.color = p.color+ (ColourValue&)delta;
		p.color.saturate();
	}
}

RefPtr<ParticleColourFaderAffector> CreateColourFaderAffector()
{
	return new ParticleColourFaderAffector();
}
}