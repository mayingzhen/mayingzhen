#pragma once
#include "ParticleEllipsoidEmitter.h"

namespace ma
{

class  CParticleHollowEllipsoidEmitter : public CParticleEllipsoidEmitter
{
public:
	CParticleHollowEllipsoidEmitter(void);
	~CParticleHollowEllipsoidEmitter(void);

	// ---------------------------------------------------------------------
	// Inherite from CParticleEllipsoidEmitter
	// ---------------------------------------------------------------------
public:
	//! Prepares an array with new particles to emitt into the system
	//! and returns how much new particles there are.
	virtual int Emitt(Real now, Real timeSinceLastCall, SParticle*& outArray, uint32 nFinalMaxParticles);

	//! Get emitter type
	virtual E_PARTICLE_EMITTER_TYPE GetType() const { return EPET_HOLLOW_ELLIPSOID; }

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
public:
    // 半径的百分比[0.0~1.0],从改值到1.0直接的半径才会产生粒子,其中x表示x半径比，y表示y方向的半径比,z表示z方向的半径比
	void SetInnerRatio(const Vector3& v){mInnerRatio = v;}
	const Vector3& GetInnerRatio() const{return mInnerRatio;}

protected:
	Vector3 mInnerRatio;
};

 RefPtr<CParticleHollowEllipsoidEmitter> CreateHollowEllipsoidEmitter();

}