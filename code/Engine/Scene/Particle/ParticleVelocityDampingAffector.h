#pragma once
#include "ParticleAffector.h"

namespace ma
{

class ParticleVelocityDampingAffector : public ParticleAffector
{
public:
    ParticleVelocityDampingAffector(void);
    ~ParticleVelocityDampingAffector(void);

    // ---------------------------------------------------------------------
    // Inherite from ParticleAffector
    // ---------------------------------------------------------------------
public:
    //! Affects a particle.
    virtual void Affect(Real now, Real timediff, LST_PARTICLE& lstParticles);

    // ---------------------------------------------------------------------
    // Self
    // ---------------------------------------------------------------------
    // 设置每秒速度相减的值
    void SetDampingSpeed(const Vector3& vSpeed){m_vDampingSpeed = vSpeed;}
    const Vector3& GetDampingSpeed() const{return m_vDampingSpeed;}
private:
    Vector3 m_vDampingSpeed;
};

RefPtr<ParticleVelocityDampingAffector> CreateVelocityDampingAffector();
}