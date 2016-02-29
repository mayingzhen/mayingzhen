#pragma once
#include "ParticleEmitter.h"

namespace ma
{

//! A default point emitter
class  CParticlePointEmitter : public CParticleEmitter
{
public:
	CParticlePointEmitter();
	~CParticlePointEmitter(void);

	// ---------------------------------------------------------------------
	// Inherite from CParticleEmitter
	// ---------------------------------------------------------------------
public:
	//! Prepares an array with new particles to emitt into the system
	//! and returns how much new particles there are.
	virtual int Emitt(Real now, Real timeSinceLastCall, SParticle*& outArray, uint32 nFinalMaxParticles);

	//! Get emitter type
	virtual E_PARTICLE_EMITTER_TYPE GetType() const { return EPET_POINT; }

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------

};

//! Creates a point particle emitter.
 RefPtr<CParticlePointEmitter> CreatePointEmitter();

}