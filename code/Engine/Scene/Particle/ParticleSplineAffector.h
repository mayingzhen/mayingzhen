#pragma once
#include "ParticleAffector.h"
#include "SimpleSpline.h"

namespace ma
{

class  CParticleSplineAffector : public CParticleAffector
{
public:
	CParticleSplineAffector(void);
	~CParticleSplineAffector(void);

	// ---------------------------------------------------------------------
	// Inherite from CParticleAffector
	// ---------------------------------------------------------------------
public:
	//! Affects a particle.
	virtual void Affect(Real now, Real timediff, LST_PARTICLE& lstParticles);

	//! Get emitter type
	virtual E_PARTICLE_AFFECTOR_TYPE GetType() const { return EPAT_SPLINE; }

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
	//! Gets array of spline points
	virtual uint32 GetNumPoints() const{return mSpline.getNumPoints();}
	virtual const Vector3& GetPointByIndex(uint32 i) const { return mSpline.getPoint(i); }

	//! Sets array of spline points
	virtual void AddPoint( const Vector3& point ) { mSpline.addPoint(point);}
	virtual void UpdatePoint(uint32 i, const Vector3& point){ mSpline.updatePoint(i, point);}
	virtual void RemovePoint(uint32 i){mSpline.removePoint(i);}
	virtual void ClearAllPoints(){mSpline.clear();}
private:
	SimpleSpline mSpline;
};

 RefPtr<CParticleSplineAffector> CreateSplineAffector();

}