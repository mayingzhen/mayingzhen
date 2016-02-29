#pragma once
#include "Particle.h"

namespace ma
{

//! Types of built in particle affectors
enum E_PARTICLE_AFFECTOR_TYPE
{
	EPAT_NONE = 0,
	EPAT_VORTEX,
	EPAT_LINEAR_FORCE,
    EPAT_POINT_FORCE,
	EPAT_ROTATION,
	EPAT_SCALE,
    EPAT_COLOUR,
	EPAT_COLOUR_FADER,
	EPAT_COLOUR_FADER2,
	EPAT_COLOUR_INTERPOLATOR,
	EPAT_ROTATION_PIVOT,
	EPAT_DEFLECTOR_PLANE,
	EPAT_DIRECTION_RANDOMISER,
	EPAT_SCALE_INTERPOLATOR,
	EPAT_SPLINE,
	EPAT_ALIGN,
	EPAT_LINE_DRIFT,
	EPAT_SUB_UV,
    EPAT_WIND_2D,
    EPAT_POINT_ATTRACTION,
    EPAT_VELOCITY_DAMPING,
    EPAT_DAMPING,
	EPAT_COUNT
};

//! A particle affector modifies particles.
class CParticleAffector : public Serializable
{
public:
	CParticleAffector(void);
	virtual ~CParticleAffector(void);
	
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

	//! Get emitter type
	virtual E_PARTICLE_AFFECTOR_TYPE GetType() const = 0;

    virtual void SetParentMatrix(const Matrix4& mat){}
    virtual void SetParentScale(const Vector3& scale){}
protected:
	bool m_bEnabled;
};

}