#pragma once
#include "ParticleEmitter.h"

namespace ma
{

class  CParticleAreaEmitter : public CParticleEmitter
{
public:
	CParticleAreaEmitter(void);
	~CParticleAreaEmitter(void);

	// ---------------------------------------------------------------------
	// Inherite from CParticleEmitter
	// ---------------------------------------------------------------------
	virtual void SetDirection( const Vector3& newDirection );

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
public:
	//! Set the size of the emitter
	virtual void SetSize( const Vector3& size);

	//! Get the size of the emitter
	virtual const Vector3& GetSize() const{return mSize;}

	const Vector3& GetXRange() const{return mXRange;}
	const Vector3& GetYRange() const{return mYRange;}
	const Vector3& GetZRange() const{return mZRange;}
protected:
	/// Internal method for generating the area axes
	void GenAreaAxes(void);

protected:
	/// Size of the area
	Vector3 mSize;

	/// Local axes, not normalised, their magnitude reflects area size
	Vector3 mXRange, mYRange, mZRange;
	Vector3 mUp;
};

}