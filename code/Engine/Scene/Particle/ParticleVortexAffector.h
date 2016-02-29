#pragma once
#include "ParticleAffector.h"

namespace ma
{

//! Particle Affector for attracting particles to a point
class  CParticleVortexAffector : public CParticleAffector
{
public:
	CParticleVortexAffector();
	~CParticleVortexAffector(void);

	// ---------------------------------------------------------------------
	// Inherite from CParticleAffector
	// ---------------------------------------------------------------------
public:
	//! Affects a particle.
	virtual void Affect(Real now, Real timediff, LST_PARTICLE& lstParticles);

	//! Get emitter type
	virtual E_PARTICLE_AFFECTOR_TYPE GetType() const { return EPAT_VORTEX; }

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
    // 绕这个向量旋转
    virtual void SetRotateVector(const Vector3& vAxis){mRotateVector = vAxis;}
    virtual const Vector3& GetRotateVector() const{return mRotateVector;}

    // 每秒旋转角度
    virtual void SetRotateSpeed(float f){mRotateSpeed = Radian(Degree(f));}
	virtual float GetRotateSpeed() const{return mRotateSpeed.valueDegrees();}
private:
	Vector3 mRotateVector;
	Radian mRotateSpeed;
};

 RefPtr<CParticleVortexAffector> CreateVortexAffector();

}