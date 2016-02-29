#pragma once
#include "ParticleAffector.h"

namespace ma
{

class CParticleVelocityDampingAffector : public CParticleAffector
{
public:
    CParticleVelocityDampingAffector(void);
    ~CParticleVelocityDampingAffector(void);

    // ---------------------------------------------------------------------
    // Inherite from CParticleAffector
    // ---------------------------------------------------------------------
public:
    //! Affects a particle.
    virtual void Affect(Real now, Real timediff, LST_PARTICLE& lstParticles);

    //! Get emitter type
    virtual E_PARTICLE_AFFECTOR_TYPE GetType() const { return EPAT_VELOCITY_DAMPING; }

    // ---------------------------------------------------------------------
    // Self
    // ---------------------------------------------------------------------
    // 设置每秒速度相减的值
    void SetDampingSpeed(const Vector3& vSpeed){m_vDampingSpeed = vSpeed;}
    const Vector3& GetDampingSpeed() const{return m_vDampingSpeed;}
private:
    Vector3 m_vDampingSpeed;
};

RefPtr<CParticleVelocityDampingAffector> CreateVelocityDampingAffector();
}