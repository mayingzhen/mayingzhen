#include "S3EngineCommon.h"
#include "ParticleAreaEmitter.h"

namespace ma
{

CParticleAreaEmitter::CParticleAreaEmitter(void)
{
	mUp = Vector3::UNIT_Y;
	this->SetSize(Vector3(1,1,1));
}

CParticleAreaEmitter::~CParticleAreaEmitter(void)
{
}

// ---------------------------------------------------------------------
// Inherite from CParticleEmitter
// ---------------------------------------------------------------------
void CParticleAreaEmitter::SetDirection( const Vector3& newDirection )
{
	CParticleEmitter::SetDirection(newDirection);

	// Generate an up vector (any will do)
	mUp = mDirection.perpendicular();
	mUp.normalise();

	this->GenAreaAxes();
}

void CParticleAreaEmitter::SetSize( const Vector3& size )
{
	mSize = size;
	this->GenAreaAxes();
}

// ---------------------------------------------------------------------
// Self
// ---------------------------------------------------------------------
void CParticleAreaEmitter::GenAreaAxes( void )
{
	Vector3 mLeft = mUp.crossProduct(mDirection);

	mXRange = mLeft * (mSize.x * 0.5f);
	mYRange = mUp * (mSize.y * 0.5f);
	mZRange = mDirection * (mSize.z * 0.5f);
}

}