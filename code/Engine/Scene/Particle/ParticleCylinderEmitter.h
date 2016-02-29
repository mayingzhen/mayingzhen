#pragma once
#include "ParticleAreaEmitter.h"

namespace ma
{

class  CParticleCylinderEmitter : public CParticleAreaEmitter
{
public:
	CParticleCylinderEmitter();
	~CParticleCylinderEmitter(void);

	// ---------------------------------------------------------------------
	// Inherite from CParticleEmitter
	// ---------------------------------------------------------------------
public:
	//! Prepares an array with new particles to emitt into the system
	//! and returns how much new particles there are.
	virtual int Emitt(Real now, Real timeSinceLastCall, SParticle*& outArray, uint32 nFinalMaxParticles);

	//! Get emitter type
	virtual E_PARTICLE_EMITTER_TYPE GetType() const { return EPET_CYLINDER; }

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
public:

};

 RefPtr<CParticleCylinderEmitter> CreateCylinderEmitter();
 
}