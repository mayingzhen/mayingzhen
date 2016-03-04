#pragma once
#include "ParticleAffector.h"

namespace ma
{

class ParticlePointAttractionAffector : public ParticleAffector
{
public:
    ParticlePointAttractionAffector(void);
    ~ParticlePointAttractionAffector(void);

    // ---------------------------------------------------------------------
    // Inherite from ParticleAffector
    // ---------------------------------------------------------------------
public:
    //! Affects a particle.
    virtual void Affect(Real now, Real timediff, LST_PARTICLE& lstParticles);

    virtual void SetParentMatrix(const Matrix4& mat){m_matParent = mat;mFinalPoint = m_matParent*mPoint;}
    virtual void SetParentScale(const Vector3& scale){m_vParentScale = scale;}
    // ---------------------------------------------------------------------
    // Self
    // ---------------------------------------------------------------------
    //! Set the point that particles will attract to
    // 引力点的位置
    virtual void SetPoint( const Vector3& point ){mPoint = point;mFinalPoint = m_matParent*mPoint;}

    //! Set Speed in units per second, to attract to the specified point.
    // 引力速度(每秒)
    virtual void SetSpeed(float speed){mSpeed = speed;}

    //! Set whether or not the particles are attracting or detracting
    // 设置向里吸引还是向外抛出
    virtual void SetAttract( bool attract ){mAttract = attract;}

    //! Set whether or not this will affect particles in the X direction
    // x方向是否受影响
    virtual void SetAffectX( bool affect ){mAffectX = affect;}

    //! Set whether or not this will affect particles in the Y direction
    virtual void SetAffectY( bool affect ){mAffectY = affect;}

    //! Set whether or not this will affect particles in the Z direction
    virtual void SetAffectZ( bool affect ){mAffectZ = affect;}

    //! Get the point that particles are attracted to
    virtual const Vector3& GetPoint() const{return mPoint;}

    //! Get whether or not the particles are attracting or detracting
    virtual bool GetAttract() const{return mAttract;}

    //! Get whether or not the particles X position are affected
    virtual bool GetAffectX() const{return mAffectX;}

    //! Get whether or not the particles Y position are affected
    virtual bool GetAffectY() const{return mAffectY;}

    //! Get whether or not the particles Z position are affected
    virtual bool GetAffectZ() const{return mAffectZ;}

    virtual Real GetSpeed() const{return mSpeed;}
private:
    Vector3 mPoint, mFinalPoint;
    Real mSpeed;
    bool mAffectX;
    bool mAffectY;
    bool mAffectZ;
    bool mAttract;
    Vector3 m_vParentScale;
    Matrix4 m_matParent;
};

RefPtr<ParticlePointAttractionAffector> CreatePointAttractionAffector();

}