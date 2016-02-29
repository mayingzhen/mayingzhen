#pragma once
#include "ParticleAffector.h"

namespace ma
{

//! Particle Affector for affecting direction of particle
class  CParticleLinearForceAffector : public CParticleAffector
{
public:
	CParticleLinearForceAffector();
	~CParticleLinearForceAffector(void);

	// ---------------------------------------------------------------------
	// Inherite from CParticleAffector
	// ---------------------------------------------------------------------
public:
	//! Affects a particle.
	virtual void Affect(Real now, Real timediff, LST_PARTICLE& lstParticles);

	//! Get emitter type
	virtual E_PARTICLE_AFFECTOR_TYPE GetType() const { return EPAT_LINEAR_FORCE; }

    virtual void SetParentMatrix(const Matrix4& mat){mat.extract3x3Matrix(m_mat3);mFinalForceVector = m_mat3*mForceVector;}
	// ---------------------------------------------------------------------
	// self
	// ---------------------------------------------------------------------
public:
    // 力向量(当ForceApplication为ADD时，表示受每秒力相加的速度；当为AVERAGE时，表示粒子生命周期结束时候的最终运动向量)
	/** Sets the force vector to apply to the particles in a system. */
	void SetForceVector(const Vector3& force){mForceVector = force;mFinalForceVector = m_mat3*mForceVector;}

    /** Gets the force vector to apply to the particles in a system. */
	const Vector3& GetForceVector(void) const{return mForceVector;}

    // ADD为每秒力相加的速度，AVERAGE表示生命周期结束时该粒子的运动向量就是ForceVector
    /** Sets how the force vector is applied to a particle. 
    @remarks
      The default is FA_ADD.
    @param fa A member of the ForceApplication enum.
    */
	void SetForceApplication(ForceApplication fa){mForceApplication = fa;}

    /** Retrieves how the force vector is applied to a particle. 
    @param fa A member of the ForceApplication enum.
    */
	ForceApplication GetForceApplication(void) const{return mForceApplication;}

    void SetStartTime(float fStartTime){mStartTime = fStartTime;}
    float GetStartTime() const{return mStartTime;}

    void SetDuration(float fDuration){mDuration = fDuration;}
    float GetDuration() const{return mDuration;}
private:
    float mStartTime;
    float mDuration;

	/// Force vector
	Vector3 mForceVector;
    Vector3 mFinalForceVector;

	/// How to apply force
	ForceApplication mForceApplication;

    Matrix3 m_mat3;
};


//! Creates a gravity affector.
/** This affector modifies the direction of the particle. It assumes
that the particle is fired out of the emitter with huge force, but is
loosing this after some time and is catched by the gravity then. This
affector is ideal for creating things like fountains.
\param gravity: Direction and force of gravity.
\param timeForceLost: Time in milli seconds when the force of the
emitter is totally lost and the particle does not move any more. This
is the time where gravity fully affects the particle.
\return Pointer to the created particle affector. To add this affector
as new affector of this particle system, just call addAffector(). Note
that you'll have to drop() the returned pointer, after you don't need
it any more, see IReferenceCounted::drop() for more informations. */
 RefPtr<CParticleLinearForceAffector> CreateLinearForceAffector();
}