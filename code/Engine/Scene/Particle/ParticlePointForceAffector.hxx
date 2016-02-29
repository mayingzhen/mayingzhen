#include "S3EngineCommon.h"
#include "ParticlePointForceAffector.h"

namespace ma
{

CParticlePointForceAffector::CParticlePointForceAffector()
:mPoint(Vector3::ZERO), mFinalPoint(Vector3::ZERO), mSpeed(1.f), mAffectX(true), mAffectY(true), mAffectZ(true),
mAttract(true), m_matParent(Matrix4::IDENTITY), m_vParentScale(Vector3::UNIT_SCALE)
{
	mForceApplication = FA_AVERAGE;
    mStartTime = 0.f;
    mDuration = 999999.f;
}

CParticlePointForceAffector::~CParticlePointForceAffector(void)
{
}

void CParticlePointForceAffector::Affect( Real now, Real timediff, LST_PARTICLE& lstParticles )
{
	if (!m_bEnabled)
		return;

    if (now < mStartTime || now > mStartTime+mDuration)
        return;

	for(LST_PARTICLE::iterator iter = lstParticles.begin();iter != lstParticles.end();++iter)
	{
		SParticle& p = *iter;
        Vector3 direction = (mFinalPoint - p.pos);
        direction.normalise();
        direction *= mSpeed;

        if( !mAttract )
            direction *= -1.0f;

        Vector3 vForce = Vector3::ZERO;
        if( mAffectX )
        {
            vForce.x += direction.x*m_vParentScale.x;
        }

        if( mAffectY )
        {
            vForce.y += direction.y*m_vParentScale.y;
        }

        if( mAffectZ )
        {
            vForce.z += direction.z*m_vParentScale.z;
        }

		if (mForceApplication == FA_ADD)
		{
			p.vector += vForce * timediff;
		}
		else // FA_AVERAGE
		{
			float fPercent = (now - p.startTime)/(p.endTime - p.startTime);
			p.vector = Math::Lerp<Vector3>(p.startVector, vForce, fPercent);
		}
	}
}

 RefPtr<CParticlePointForceAffector> CreatePointForceAffector()
{
	return new CParticlePointForceAffector();
}
}