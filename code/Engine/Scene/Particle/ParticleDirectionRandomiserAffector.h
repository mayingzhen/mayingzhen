#pragma once
#include "ParticleAffector.h"

namespace ma
{
 /** This class defines a ParticleAffector which applies randomness to the movement of the particles.
	@remarks
		This affector (see ParticleAffector) applies randomness to the movement of the particles by
		changing the direction vectors.
	@par
		The most important parameter to control the effect is randomness. It controls the range in which changes
		are applied to each axis of the direction vector.
		The parameter scope can be used to limit the effect to a certain percentage of the particles.
	*/
class  ParticleDirectionRandomiserAffector : public ParticleAffector
{
public:
	ParticleDirectionRandomiserAffector(void);
	~ParticleDirectionRandomiserAffector(void);

	// ---------------------------------------------------------------------
	// Inherite from ParticleAffector
	// ---------------------------------------------------------------------
public:
	//! Affects a particle.
	virtual void Affect(Real now, Real timediff, LST_PARTICLE& lstParticles);

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
	/** Sets the randomness to apply to the particles in a system. */
    // 抖动方向，数值越大，抖动幅度越大
	void SetRandomness(Real force){mRandomness = force;}

	/** Sets the scope (percentage of particles which are randomised). */
    // 抖动粒子数量的百分比[0.0,1.0]，为0表示没有粒子抖动，为0.5表示有50%的粒子正在抖动，为1表示所有粒子抖动
	void SetScope(Real force){mScope = force;}

    // 是不是要保持速度数值不变，只是方向抖动
	/** Set flag which detemines whether particle speed is changed. */
	void SetKeepVelocity(bool keepVelocity){mKeepVelocity = keepVelocity;}

	/** Gets the randomness to apply to the particles in a system. */
	Real GetRandomness(void) const{return mRandomness;}

	/** Gets the scope (percentage of particles which are randomised). */
	Real GetScope(void) const{return mScope;}

	/** Gets flag which detemines whether particle speed is changed. */
	bool GetKeepVelocity(void) const{return mKeepVelocity;}
protected:
	Real mRandomness;
	Real mScope;
	bool mKeepVelocity;
};

 RefPtr<ParticleDirectionRandomiserAffector> CreateDirectionRandomiserAffector();
}