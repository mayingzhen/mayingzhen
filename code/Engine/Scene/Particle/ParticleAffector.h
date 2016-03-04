#pragma once
#include "Particle.h"

namespace ma
{


//! A particle affector modifies particles.
class ParticleAffector : public Object
{
public:
	ParticleAffector(void);
	virtual ~ParticleAffector(void);
	
	virtual void Init(SParticle* particlearray, uint32 count){}
    virtual void Reset(){}

	//! Affects an array of particles.
	/** \param now Current time. (Same as ITimer::getTime() would return)
	\param particlearray Array of particles.
	\param count Amount of particles in array. */
	virtual void Affect(Real now, Real timediff, LST_PARTICLE& lstParticles) = 0;

	//! Sets whether or not the affector is currently enabled.
	virtual void SetEnabled(bool enabled) { m_bEnabled = enabled; }

	//! Gets whether or not the affector is currently enabled.
	virtual bool GetEnabled() const { return m_bEnabled; }

    virtual void SetParentMatrix(const Matrix4& mat){}
    virtual void SetParentScale(const Vector3& scale){}
protected:
	bool m_bEnabled;
};

}