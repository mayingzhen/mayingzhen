#pragma once
#include "ParticleAffector.h"

namespace ma
{

class CParticleAlignAffector : public CParticleAffector
{
public:
	CParticleAlignAffector(void);
	~CParticleAlignAffector(void);

	// ---------------------------------------------------------------------
	// Inherite from CParticleAffector
	// ---------------------------------------------------------------------
public:
	//! Affects a particle.
	virtual void Affect(Real now, Real timediff, LST_PARTICLE& lstParticles);

	//! Get emitter type
	virtual E_PARTICLE_AFFECTOR_TYPE GetType() const { return EPAT_ALIGN; }

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
public:
	/** Set resize. This attribute determines whether the size of the particle must be changed
		according to its alignment with the previous particle.
	*/
    // 粒子的高度是否要跟随粒子距离进行调整值为\"距离上一个粒子的长度\"+\"附加长度\"）
	void SetResize(bool bResize){mResize = bResize;}
	bool GetResize() const{return mResize;}

    // 附加的最小长度（仅对Resize为true时有效）
	void SetMinAddLength(Real minAddLength){mMinAddLength = minAddLength;}
	Real GetMinAddLength() const{return mMinAddLength;}

    // 附加的最大长度（仅对Resize为true时有效）
	void SetMaxAddLength(Real maxAddLength){mMaxAddLength = maxAddLength;}
	Real GetMaxAddLength() const{return mMaxAddLength;}

private:
	bool mResize;
	Real mMinAddLength;
	Real mMaxAddLength;
};

RefPtr<CParticleAlignAffector> CreateAlignAffector();
}