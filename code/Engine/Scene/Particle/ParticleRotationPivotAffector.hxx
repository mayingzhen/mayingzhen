#include "S3EngineCommon.h"
#include "ParticleRotationPivotAffector.h"

namespace ma
{

	ParticleRotationPivotAffector::ParticleRotationPivotAffector()
		:mPivotPoint(Vector3::ZERO), mFinalPivotPoint(Vector3::ZERO), mSpeed(Vector3::UNIT_SCALE)
	{
		m_matParent = Matrix4::IDENTITY;
	}

	ParticleRotationPivotAffector::~ParticleRotationPivotAffector(void)
	{
	}

    void ParticleRotationPivotAffector::Reset()
    {
    }

	void ParticleRotationPivotAffector::Affect( Real now, Real timediff, LST_PARTICLE& lstParticles )
	{
		if( !m_bEnabled )
			return;

		for(LST_PARTICLE::iterator iter = lstParticles.begin();iter != lstParticles.end();++iter)
		{
			SParticle& p = *iter;
			if( mSpeed.x != 0.0f )
				p.pos.rotateYZBy( Radian(Degree(timediff * mSpeed.x)), mFinalPivotPoint );

			if( mSpeed.y != 0.0f )
				p.pos.rotateXZBy( Radian(Degree(timediff * mSpeed.y)), mFinalPivotPoint );

			if( mSpeed.z != 0.0f )
				p.pos.rotateXYBy( Radian(Degree(timediff * mSpeed.z)), mFinalPivotPoint );
		}
	}

	RefPtr<ParticleRotationPivotAffector> CreateRotationPivotAffector()
	{
		return new ParticleRotationPivotAffector();
	}
}