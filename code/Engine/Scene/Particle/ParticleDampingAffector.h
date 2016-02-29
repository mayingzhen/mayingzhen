#pragma once
#include "ParticleAffector.h"

namespace ma
{

class CParticleDampingAffector : public CParticleAffector
{
public:
    CParticleDampingAffector(void);
    ~CParticleDampingAffector(void);

    // ---------------------------------------------------------------------
    // Inherite from CParticleAffector
    // ---------------------------------------------------------------------
public:
    //! Affects a particle.
    virtual void Affect(Real now, Real timediff, LST_PARTICLE& lstParticles);

    //! Get emitter type
    virtual E_PARTICLE_AFFECTOR_TYPE GetType() const { return EPAT_DAMPING; }

    // ---------------------------------------------------------------------
    // Self
    // ---------------------------------------------------------------------
    // 设置每秒速度相减的值
    void SetDampingSpeed(float fSpeed){m_fDampingSpeed = fSpeed;}
    float GetDampingSpeed() const{return m_fDampingSpeed;}

	void SetStartTime(float fStartTime){mStartTime = fStartTime;}
	float GetStartTime() const{return mStartTime;}

	void SetDuration(float fDuration){mDuration = fDuration;}
	float GetDuration() const{return mDuration;}
private:
    float m_fDampingSpeed;
	float mStartTime;
	float mDuration;
};

RefPtr<CParticleDampingAffector> CreateDampingAffector();
}