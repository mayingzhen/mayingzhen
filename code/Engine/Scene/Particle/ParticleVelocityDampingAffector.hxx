#include "S3EngineCommon.h"
#include "ParticleVelocityDampingAffector.h"

namespace ma
{

CParticleVelocityDampingAffector::CParticleVelocityDampingAffector(void)
: m_vDampingSpeed(Vector3::ZERO)
{
}

CParticleVelocityDampingAffector::~CParticleVelocityDampingAffector(void)
{
}

void CParticleVelocityDampingAffector::Affect( Real now, Real timediff, LST_PARTICLE& lstParticles )
{
    if( !m_bEnabled )
        return;

    for(LST_PARTICLE::iterator iter = lstParticles.begin();iter != lstParticles.end();++iter)
    {
        SParticle& p = *iter;
        if (p.vector.x >= 0)
            p.vector.x = __max(0, Math::Abs(p.vector.x) - m_vDampingSpeed.x*timediff);
        else
            p.vector.x = -__max(0, Math::Abs(p.vector.x) - m_vDampingSpeed.x*timediff);
        
        if (p.vector.y >= 0)
            p.vector.y = __max(0, Math::Abs(p.vector.y) - m_vDampingSpeed.y*timediff);
        else
            p.vector.y = -__max(0, Math::Abs(p.vector.y) - m_vDampingSpeed.y*timediff);

        if (p.vector.z >= 0)
            p.vector.z = __max(0, Math::Abs(p.vector.z) - m_vDampingSpeed.z*timediff);
        else
            p.vector.z = -__max(0, Math::Abs(p.vector.z) - m_vDampingSpeed.z*timediff);
    }
}

RefPtr<CParticleVelocityDampingAffector> CreateVelocityDampingAffector()
{
    return new CParticleVelocityDampingAffector;
}

}