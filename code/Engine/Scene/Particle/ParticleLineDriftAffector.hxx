#include "S3EngineCommon.h"
#include "ParticleLineDriftAffector.h"

namespace ma
{

ParticleLineDriftAffector::ParticleLineDriftAffector(void)
{
	mMaxDeviation = 1.f;
	mEnd = Vector3::ZERO;
	mTimeStep = 0.1f;
	mDrift = 0.f;

	mOneMinusDrift = 1 - mDrift;
	mUpdate = true;
	mFirst = true;
	mTimeSinceLastUpdate = 0.f;
}

ParticleLineDriftAffector::~ParticleLineDriftAffector(void)
{
}

void ParticleLineDriftAffector::Reset()
{
    mUpdate = true;
    mFirst = true;
    mTimeSinceLastUpdate = 0.f;
}

void ParticleLineDriftAffector::Affect( Real now, Real timediff, LST_PARTICLE& lstParticles )
{
	if( !m_bEnabled )
		return;

	if (lstParticles.empty())
	{
		return;
	}

	mTimeSinceLastUpdate += timediff;
	while (mTimeSinceLastUpdate > mTimeStep)
	{
		mTimeSinceLastUpdate -= mTimeStep;
		mUpdate = true;
	}

	mFirst = true;
	for(LST_PARTICLE::iterator iter = lstParticles.begin();iter != lstParticles.end();++iter)
	{
		SParticle& p = *iter;

		if (mUpdate && Math::UnitRandom() > 0.5 && !mFirst)
		{
			// Generate a random vector perpendicular on the line
			Vector3 perpendicular = mEnd.crossProduct(Vector3(Math::RangeRandom(-1, 1), 
				Math::RangeRandom(-1, 1), 
				Math::RangeRandom(-1, 1)));
			perpendicular.normalise();

			// Determine a random point near the line.
			Vector3 targetPosition = p.startPos + perpendicular*(mMaxDeviation * Math::UnitRandom());

			// Set the new position.
			p.pos = targetPosition*mDrift + p.pos*mOneMinusDrift;
		}
		mFirst = false;
	}

	mUpdate = false;
}

RefPtr<ParticleLineDriftAffector> CreateLineDriftAffector()
{
	return new ParticleLineDriftAffector;
}

}