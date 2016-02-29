#pragma once
#include "ParticleAreaEmitter.h"

namespace ma
{

class  CParticleBoxEmitter : public CParticleAreaEmitter
{
public:
	CParticleBoxEmitter();
	~CParticleBoxEmitter(void);

	// ---------------------------------------------------------------------
	// Inherite from CParticleEmitter
	// ---------------------------------------------------------------------
public:
	//! Prepares an array with new particles to emitt into the system
	//! and returns how much new particles there are.
	virtual int Emitt(Real now, Real timeSinceLastCall, SParticle*& outArray, uint32 nFinalMaxParticles);

	//! Get emitter type
	virtual E_PARTICLE_EMITTER_TYPE GetType() const { return EPET_BOX; }

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
private:

};


// ---------------------------------------------------------------------
// Global Function
// ---------------------------------------------------------------------
 RefPtr<CParticleBoxEmitter> CreateBoxEmitter();
}