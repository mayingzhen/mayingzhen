#pragma once
#include "ParticleAreaEmitter.h"

namespace ma
{

class  CParticleEllipsoidEmitter : public CParticleAreaEmitter
{
public:
	CParticleEllipsoidEmitter();
	~CParticleEllipsoidEmitter(void);

	// ---------------------------------------------------------------------
	// Inherite from CParticleEmitter
	// ---------------------------------------------------------------------
public:
	//! Prepares an array with new particles to emitt into the system
	//! and returns how much new particles there are.
	virtual int Emitt(Real now, Real timeSinceLastCall, SParticle*& outArray, uint32 nFinalMaxParticles);

	//! Get emitter type
	virtual E_PARTICLE_EMITTER_TYPE GetType() const { return EPET_ELLIPSOID; }

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
public:

};


//! Creates a sphere particle emitter.
 RefPtr<CParticleEllipsoidEmitter> CreateEllipsoidEmitter();
}