#include "S3EngineCommon.h"
#include "ParticleDeflectorPlaneAffector.h"

namespace ma
{

CParticleDeflectorPlaneAffector::CParticleDeflectorPlaneAffector(void)
{
	// defaults
	m_vFinalPlanePoint = mPlanePoint = Vector3::ZERO;
	mPlaneNormal = Vector3::UNIT_Z;
	mBounce = 1.0;
    m_matParent = Matrix4::IDENTITY;
}

CParticleDeflectorPlaneAffector::~CParticleDeflectorPlaneAffector(void)
{
}

void CParticleDeflectorPlaneAffector::Affect( Real now, Real timediff, LST_PARTICLE& lstParticles )
{
	if( !m_bEnabled )
		return;

	// precalculate distance of plane from origin
	Real planeDistance = - mPlaneNormal.dotProduct(m_vFinalPlanePoint) / Math::Sqrt(mPlaneNormal.dotProduct(mPlaneNormal));
	Vector3 directionPart;

	for(LST_PARTICLE::iterator iter = lstParticles.begin();iter != lstParticles.end();++iter)
	{
		SParticle& p = *iter;
		Vector3 direction(p.vector * timediff);
		if (mPlaneNormal.dotProduct(p.pos + direction) + planeDistance <= 0.0)
		{
			Real a = mPlaneNormal.dotProduct(p.pos) + planeDistance;
			if (a > 0.0)
			{
				// for intersection point
				directionPart = direction * (- a / direction.dotProduct( mPlaneNormal ));
				// set new position
				p.pos = (p.pos + ( directionPart )) + (((directionPart) - direction) * mBounce);

				// reflect direction vector
				p.vector = (p.vector - (2.f * p.vector.dotProduct( mPlaneNormal ) * mPlaneNormal)) * mBounce;
			}
		}
	}

}

RefPtr<CParticleDeflectorPlaneAffector> CreateDeflectorPlaneAffector()
{
	return new CParticleDeflectorPlaneAffector;
}

}