#include "S3EngineCommon.h"
#include "ParticleRotationAffector.h"

namespace ma
{

ParticleRotationAffector::ParticleRotationAffector()
		:mMinStart(0.f), mMaxStart(0.f), mMinSpeed(0.f), mMaxSpeed(0.f)
{
}

ParticleRotationAffector::~ParticleRotationAffector(void)
{
}


void ParticleRotationAffector::Init( SParticle* particlearray, uint32 count )
{
	if( !m_bEnabled )
		return;

	if (particlearray == NULL)
		return;

	for(uint32 i=0;i<count;i++)
	{
		SParticle& p = particlearray[i];
		p.rotation = p.rotationStart = mMinStart + (Randomizer::frand() * (mMaxStart - mMinStart));
		p.rotationSpeed = mMinSpeed + Randomizer::frand()*(mMaxSpeed-mMinSpeed);
	}
}

void ParticleRotationAffector::Affect( Real now, Real timediff, LST_PARTICLE& lstParticles )
{
	if( !m_bEnabled )
		return;

	for(LST_PARTICLE::iterator iter = lstParticles.begin();iter != lstParticles.end();++iter)
	{
		SParticle& p = *iter;
		p.rotation += p.rotationSpeed*timediff;
	}
}

 RefPtr<ParticleRotationAffector> CreateRotationAffector()
{
	return new ParticleRotationAffector();
}
}