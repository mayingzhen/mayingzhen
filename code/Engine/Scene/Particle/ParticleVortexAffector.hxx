#include "S3EngineCommon.h"
#include "ParticleVortexAffector.h"

namespace ma
{

CParticleVortexAffector::CParticleVortexAffector()
: mRotateSpeed(1.f), mRotateVector(Vector3::UNIT_Z)
{
}

CParticleVortexAffector::~CParticleVortexAffector(void)
{
}

void CParticleVortexAffector::Affect( Real now, Real timediff, LST_PARTICLE& lstParticles)
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

RefPtr<CParticleVortexAffector> CreateVortexAffector()
{
	return new CParticleVortexAffector();
}
}