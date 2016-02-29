#pragma once
#include "ParticleAffector.h"

namespace ma
{

//! Particle Affector for affecting direction of particle
class  CParticlePointForceAffector : public CParticleAffector
{
public:
	CParticlePointForceAffector();
	~CParticlePointForceAffector(void);

	// ---------------------------------------------------------------------
	// Inherite from CParticleAffector
	// ---------------------------------------------------------------------
public:
	//! Affects a particle.
	virtual void Affect(Real now, Real timediff, LST_PARTICLE& lstParticles);

	//! Get emitter type
	virtual E_PARTICLE_AFFECTOR_TYPE GetType() const { return EPAT_POINT_FORCE; }

    virtual void SetParentMatrix(const Matrix4& mat){m_matParent = mat;mFinalPoint = m_matParent*mPoint;}
    virtual void SetParentScale(const Vector3& scale){m_vParentScale = scale;}
	// ---------------------------------------------------------------------
	// self
	// ---------------------------------------------------------------------
public:
    virtual void SetPoint( const Vector3& point ){mPoint = point;mFinalPoint = m_matParent*mPoint;}
    virtual const Vector3& GetPoint() const{return mPoint;}

    virtual void SetSpeed(float speed){mSpeed = speed;}
    virtual Real GetSpeed() const{return mSpeed;}

    virtual void SetAttract( bool attract ){mAttract = attract;}
    virtual bool GetAttract() const{return mAttract;}
    
    virtual void SetAffectX( bool affect ){mAffectX = affect;}

    //! Set whether or not this will affect particles in the Y direction
    virtual void SetAffectY( bool affect ){mAffectY = affect;}

    //! Set whether or not this will affect particles in the Z direction
    virtual void SetAffectZ( bool affect ){mAffectZ = affect;}

    //! Get whether or not the particles X position are affected
    virtual bool GetAffectX() const{return mAffectX;}

    //! Get whether or not the particles Y position are affected
    virtual bool GetAffectY() const{return mAffectY;}

    //! Get whether or not the particles Z position are affected
    virtual bool GetAffectZ() const{return mAffectZ;}

    // ADD为每秒力相加的速度，AVERAGE表示生命周期结束时该粒子的运动向量就是ForceVector
	void SetForceApplication(ForceApplication fa){mForceApplication = fa;}
	ForceApplication GetForceApplication(void) const{return mForceApplication;}

    void SetStartTime(float fBeginTime){mStartTime = fBeginTime;}
    float GetStartTime() const{return mStartTime;}

    void SetDuration(float fDuration){mDuration = fDuration;}
    float GetDuration() const{return mDuration;}
private:
    float mStartTime;
    float mDuration;

    Vector3 mPoint, mFinalPoint;
    Real mSpeed;
    bool mAffectX;
    bool mAffectY;
    bool mAffectZ;
    bool mAttract;
    Vector3 m_vParentScale;
    Matrix4 m_matParent;

	/// How to apply force
	ForceApplication mForceApplication;
};

RefPtr<CParticlePointForceAffector> CreatePointForceAffector();
}