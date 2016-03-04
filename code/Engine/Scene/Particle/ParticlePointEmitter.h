#pragma once
#include "ParticleEmitter.h"

namespace ma
{

//! A default point emitter
class  ParticlePointEmitter : public ParticleEmitter
{
public:
	ParticlePointEmitter();
	~ParticlePointEmitter(void);

	DECL_OBJECT(ParticlePointEmitter);

	static void RegisterAttribute();

	//! Prepares an array with new particles to emitt into the system
	//! and returns how much new particles there are.
	virtual int Emitt(Real now, Real timeSinceLastCall, SParticle*& outArray, uint32 nFinalMaxParticles);

};

//! Creates a point particle emitter.
 RefPtr<ParticlePointEmitter> CreatePointEmitter();

}