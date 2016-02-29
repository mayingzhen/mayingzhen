#pragma once
#include "ParticleAffector.h"

 /** This class defines a ParticleAffector which deflects particles.
@remarks
    This affector (see ParticleAffector) offers a simple (and inaccurate) physical deflection.
    All particles which hit the plane are reflected.
@par
    The plane is defined by a point (plane_point) and the normal (plane_normal).
    In addition it is possible to change the strength of the recoil by using the bounce parameter.
*/
namespace ma
{

class  CParticleDeflectorPlaneAffector : public CParticleAffector
{
public:
	CParticleDeflectorPlaneAffector(void);
	~CParticleDeflectorPlaneAffector(void);

	// ---------------------------------------------------------------------
	// Inherite from CParticleAffector
	// ---------------------------------------------------------------------
public:
	//! Affects a particle.
	virtual void Affect(Real now, Real timediff, LST_PARTICLE& lstParticles);

	//! Get emitter type
	virtual E_PARTICLE_AFFECTOR_TYPE GetType() const { return EPAT_DEFLECTOR_PLANE; }

    virtual void SetParentMatrix(const Matrix4& mat){m_matParent = mat;m_vFinalPlanePoint = mat*mPlanePoint;}
	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
	/** Sets the plane point of the deflector plane. */
	void SetPlanePoint(const Vector3& pos){mPlanePoint = pos;m_vFinalPlanePoint = m_matParent*mPlanePoint;}

	/** Gets the plane point of the deflector plane. */
	const Vector3& GetPlanePoint(void) const{return mPlanePoint;}

	/** Sets the plane normal of the deflector plane. */
	void SetPlaneNormal(const Vector3& normal){mPlaneNormal = normal;mPlaneNormal.normalise();}

	/** Gets the plane normal of the deflector plane. */
	const Vector3& GetPlaneNormal(void) const{return mPlaneNormal;}

    // 反弹系数(百分比，1.0表示100%反弹)
	/** Sets the bounce value of the deflection. */
	void SetBounce(Real bounce){mBounce = bounce;}

	/** Gets the bounce value of the deflection. */
	Real GetBounce(void) const{return mBounce;}
protected:
	/// deflector plane point
	Vector3 mPlanePoint;
	/// deflector plane normal vector
	Vector3 mPlaneNormal;

	/// bounce factor (0.5 means 50 percent)
	Real mBounce;

    Vector3 m_vFinalPlanePoint;
    Matrix4 m_matParent;
};

 RefPtr<CParticleDeflectorPlaneAffector> CreateDeflectorPlaneAffector();
}