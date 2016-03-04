#include "S3EngineCommon.h"
#include "ParticleAreaEmitter.h"

namespace ma
{

ParticleAreaEmitter::ParticleAreaEmitter(void)
{
	mUp = Vector3::UNIT_Y;
	this->SetSize(Vector3(1,1,1));
}

ParticleAreaEmitter::~ParticleAreaEmitter(void)
{
}

// ---------------------------------------------------------------------
// Inherite from ParticleEmitter
// ---------------------------------------------------------------------
void ParticleAreaEmitter::SetDirection( const Vector3& newDirection )
{
	ParticleEmitter::SetDirection(newDirection);

	// Generate an up vector (any will do)
	mUp = mDirection.perpendicular();
	mUp.normalise();

	this->GenAreaAxes();
}

void ParticleAreaEmitter::SetSize( const Vector3& size )
{
	mSize = size;
	this->GenAreaAxes();
}

// ---------------------------------------------------------------------
// Self
// ---------------------------------------------------------------------
void ParticleAreaEmitter::GenAreaAxes( void )
{
	Vector3 mLeft = mUp.crossProduct(mDirection);

	mXRange = mLeft * (mSize.x * 0.5f);
	mYRange = mUp * (mSize.y * 0.5f);
	mZRange = mDirection * (mSize.z * 0.5f);
}

}