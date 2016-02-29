#include "S3EngineCommon.h"
#include "ParticlePointAttractionAffector.h"

namespace ma
{

CParticlePointAttractionAffector::CParticlePointAttractionAffector(void)
:mPoint(Vector3::ZERO), mFinalPoint(Vector3::ZERO), mSpeed(1.f), mAffectX(true), mAffectY(true), mAffectZ(true),
mAttract(true), m_matParent(Matrix4::IDENTITY), m_vParentScale(Vector3::UNIT_SCALE)
{
}

CParticlePointAttractionAffector::~CParticlePointAttractionAffector(void)
{
}


void CParticlePointAttractionAffector::Affect( Real now, Real timediff, LST_PARTICLE& lstParticles)
{
    if( !m_bEnabled )
        return;

    for(LST_PARTICLE::iterator iter = lstParticles.begin();iter != lstParticles.end();++iter)
    {
        SParticle& p = *iter;
        Vector3 direction = (mFinalPoint - p.pos);
        direction.normalise();
        direction *= mSpeed * timediff;

        if( !mAttract )
            direction *= -1.0f;

        if( mAffectX )
        {
            p.pos.x += direction.x*m_vParentScale.x;
        }

        if( mAffectY )
        {
            p.pos.y += direction.y*m_vParentScale.y;
        }

        if( mAffectZ )
        {
            p.pos.z += direction.z*m_vParentScale.z;
        }
    }
}

RefPtr<CParticlePointAttractionAffector> CreatePointAttractionAffector()
{
    return new CParticlePointAttractionAffector();
}

}