#include "S3EngineCommon.h"
#include "ParticleVortexAffector.h"

namespace ma
{

ParticleVortexAffector::ParticleVortexAffector()
: mRotateSpeed(1.f), mRotateVector(Vector3::UNIT_Z)
{
}

ParticleVortexAffector::~ParticleVortexAffector(void)
{
}

void ParticleVortexAffector::Affect( Real now, Real timediff, LST_PARTICLE& lstParticles)
{
	if( !m_bEnabled )
		return;

    Quaternion qRotate(Radian(mRotateSpeed*timediff), mRotateVector);
	for(LST_PARTICLE::iterator iter = lstParticles.begin();iter != lstParticles.end();++iter)
	{
		SParticle& p = *iter;
        p.vector = qRotate*p.vector;
	}
}

RefPtr<ParticleVortexAffector> CreateVortexAffector()
{
	return new ParticleVortexAffector();
}
}