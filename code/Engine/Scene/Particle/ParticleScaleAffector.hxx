#include "S3EngineCommon.h"
#include "ParticleScaleAffector.h"

namespace ma
{

ParticleScaleAffector::ParticleScaleAffector()
:mScaleAdder(FloatSize(1.0f, 1.0f))
{
}

ParticleScaleAffector::~ParticleScaleAffector(void)
{
}

void ParticleScaleAffector::Affect( Real now, Real timediff, LST_PARTICLE& lstParticles )
{
	if( !m_bEnabled )
		return;

	FloatSize ds(mScaleAdder.width*timediff, mScaleAdder.height*timediff);
	for(LST_PARTICLE::iterator iter = lstParticles.begin();iter != lstParticles.end();++iter)
	{
		SParticle& p = *iter;
		p.size += ds;
	}
}

RefPtr<ParticleScaleAffector> CreateScaleAffector()
{
	return new ParticleScaleAffector();
}
}