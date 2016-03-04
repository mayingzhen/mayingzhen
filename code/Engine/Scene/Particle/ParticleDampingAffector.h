#pragma once
#include "ParticleAffector.h"

namespace ma
{

class ParticleDampingAffector : public ParticleAffector
{
public:
    ParticleDampingAffector(void);
    ~ParticleDampingAffector(void);

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

RefPtr<ParticleDampingAffector> CreateDampingAffector();
}