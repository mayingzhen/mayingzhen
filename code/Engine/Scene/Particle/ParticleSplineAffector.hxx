#include "S3EngineCommon.h"
#include "ParticleSplineAffector.h"

namespace ma
{

ParticleSplineAffector::ParticleSplineAffector(void)
{
}

ParticleSplineAffector::~ParticleSplineAffector(void)
{
}

void ParticleSplineAffector::Affect( Real now, Real timediff, LST_PARTICLE& lstParticles )
{
	if( !m_bEnabled )
		return;

	if( mSpline.getNumPoints() == 0 )
		return;

	for(LST_PARTICLE::iterator iter = lstParticles.begin();iter != lstParticles.end();++iter)
	{
		SParticle& p = *iter;
		if (now > p.endTime)
		{
			continue;
		}

		Real timeFraction = (now - p.startTime - timediff) / (p.endTime - p.startTime);
		Real timeFractionPlusDelta = (now - p.startTime)/(p.endTime - p.startTime);
		timeFraction = Math::Clamp<Real>(timeFraction, 0.f, 1.f);
		timeFractionPlusDelta = Math::Clamp<Real>(timeFractionPlusDelta, 0.f, 1.f);
		p.pos += mSpline.interpolate(timeFractionPlusDelta) - mSpline.interpolate(timeFraction);
	}
}

RefPtr<ParticleSplineAffector> CreateSplineAffector()
{
	return new ParticleSplineAffector;
}

}