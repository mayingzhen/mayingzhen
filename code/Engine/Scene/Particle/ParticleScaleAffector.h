#pragma once
#include "ParticleAffector.h"

namespace ma
{

class  ParticleScaleAffector : public ParticleAffector
{
public:
	ParticleScaleAffector();
	~ParticleScaleAffector(void);

	// ---------------------------------------------------------------------
	// Inherite from ParticleAffector
	// ---------------------------------------------------------------------
public:
	//! Affects a particle.
	virtual void Affect(Real now, Real timediff, LST_PARTICLE& lstParticles);

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
public:
    // 每秒缩放的速度(影响粒子的长宽)
	void SetScaleAdder(const FloatSize& size){mScaleAdder = size;}
	const FloatSize& GetScaleAdder() const{return mScaleAdder;}
protected:
	FloatSize mScaleAdder;
};


//! Creates a scale particle affector.
/** This affector scales the particle to the a multiple of its size defined
by the scaleTo variable.
\param scaleTo: multiple of the size which the particle will be scaled to until deletion
\return Pointer to the created particle affector.
To add this affector as new affector of this particle system,
just call addAffector(). Note that you'll have to drop() the
returned pointer, after you don't need it any more, see
IReferenceCounted::drop() for more information. */
 RefPtr<ParticleScaleAffector> CreateScaleAffector();

}