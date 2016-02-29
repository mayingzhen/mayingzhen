#include "S3EngineCommon.h"
#include "ParticleLinearForceAffector.h"

namespace ma
{

CParticleLinearForceAffector::CParticleLinearForceAffector()
{
	mFinalForceVector = mForceVector = Vector3::UNIT_Z;
	mForceApplication = FA_AVERAGE;
    m_mat3 = Matrix3::IDENTITY;
    mStartTime = 0.f;
    mDuration = 999999.f;
}

CParticleLinearForceAffector::~CParticleLinearForceAffector(void)
{
}

void CParticleLinearForceAffector::Affect( Real now, Real timediff, LST_PARTICLE& lstParticles )
{
	if (!m_bEnabled)
		return;

    if (now < mStartTime || now > mStartTime+mDuration)
        return;

	for(LST_PARTICLE::iterator iter = lstParticles.begin();iter != lstParticles.end();++iter)
	{
		SParticle& p = *iter;
		if (mForceApplication == FA_ADD)
		{
			p.vector += mFinalForceVector * timediff;
		}
		else // FA_AVERAGE
		{
			float fPercent = (now - p.startTime)/(p.endTime - p.startTime);
			p.vector = Math::Lerp<Vector3>(p.startVector, mFinalForceVector, fPercent);
		}
	}
}

 RefPtr<CParticleLinearForceAffector> CreateLinearForceAffector()
{
	return new CParticleLinearForceAffector();
}
}