#pragma once
#include "ParticleAreaEmitter.h"

namespace ma
{

class  ParticleCylinderEmitter : public ParticleAreaEmitter
{
public:
	ParticleCylinderEmitter();
	~ParticleCylinderEmitter(void);

	// ---------------------------------------------------------------------
	// Inherite from ParticleEmitter
	// ---------------------------------------------------------------------
public:
	//! Prepares an array with new particles to emitt into the system
	//! and returns how much new particles there are.
	virtual int Emitt(Real now, Real timeSinceLastCall, SParticle*& outArray, uint32 nFinalMaxParticles);

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
public:

};

 RefPtr<ParticleCylinderEmitter> CreateCylinderEmitter();
 
}