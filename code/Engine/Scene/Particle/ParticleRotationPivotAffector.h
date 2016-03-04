#pragma once
#include "ParticleAffector.h"

namespace ma
{

//! Particle Affector for rotating particles about a point
class  ParticleRotationPivotAffector : public ParticleAffector
{
public:
	ParticleRotationPivotAffector();
	~ParticleRotationPivotAffector(void);

	// ---------------------------------------------------------------------
	// Inherite from ParticleAffector
	// ---------------------------------------------------------------------
public:
    virtual void Reset();

	//! Affects a particle.
	virtual void Affect(Real now, Real timediff, LST_PARTICLE& lstParticles);

	virtual void SetParentMatrix(const Matrix4& mat){m_matParent = mat;mFinalPivotPoint = m_matParent*mPivotPoint;}
	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
    // 旋转的中心点
	//! Set the point that particles will rotate around
	virtual void SetPivotPoint( const Vector3& point ){mPivotPoint = point;mFinalPivotPoint = m_matParent*mPivotPoint;}

    // 每秒旋转速度（角度）
	//! Set the speed in degrees per second in all 3 dimensions
	virtual void SetSpeed( const Vector3& speed ){mSpeed = speed;}

	//! Get the point that particles are attracted to
	virtual const Vector3& GetPivotPoint() const{return mPivotPoint;}

	//! Get the speed in degrees per second in all 3 dimensions
	virtual const Vector3& GetSpeed() const{return mSpeed;}

private:
	Vector3 mPivotPoint;
	Vector3 mSpeed;

	Vector3 mFinalPivotPoint;
	Matrix4 m_matParent;
};

//! Creates a rotation affector.
/** This affector modifies the positions of the particles and attracts
them to a specified point at a specified speed per second.
\param speed: Rotation in degrees per second
\param pivotPoint: Point to rotate the particles around
\return Pointer to the created particle affector. To add this affector
as new affector of this particle system, just call addAffector(). Note
that you'll have to drop() the returned pointer, after you don't need
it any more, see IReferenceCounted::drop() for more informations. */
 RefPtr<ParticleRotationPivotAffector> CreateRotationPivotAffector();
}