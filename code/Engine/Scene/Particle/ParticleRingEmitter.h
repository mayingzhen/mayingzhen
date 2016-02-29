#pragma once
#include "ParticleAreaEmitter.h"

namespace ma
{

class  CParticleRingEmitter : public CParticleAreaEmitter
{
public:
	CParticleRingEmitter();
	~CParticleRingEmitter(void);

	//! Prepares an array with new particles to emitt into the system
	//! and returns how much new particles there are.
	virtual int Emitt(Real now, Real timeSinceLastCall, SParticle*& outArray, uint32 nFinalMaxParticles);

	//! Get emitter type
	virtual E_PARTICLE_EMITTER_TYPE GetType() const { return EPET_RING; }
	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
public:
	//! Set the innerRadio of the ring
    // 半径的百分比[0.0~1.0],从改值到1.0直接的半径才会产生粒子,其中x表示x半径比，y表示y方向的半径比
	virtual void SetInnerRatio( const Vector2& v ){mInnerRatio = v;}

	//! Get the innerRadio of the ring
	virtual const Vector2& GetInnerRatio() const{return mInnerRatio;}

private:
	Vector2 mInnerRatio;
};

//! Creates a ring particle emitter.
 RefPtr<CParticleRingEmitter> CreateRingEmitter();
}