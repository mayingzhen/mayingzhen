#pragma once
#include "ParticleEmitter.h"

namespace ma
{

class  CParticleLineEmitter : public CParticleEmitter
{
public:
	CParticleLineEmitter(void);
	~CParticleLineEmitter(void);

	// ---------------------------------------------------------------------
	// Inherite from CParticleEmitter
	// ---------------------------------------------------------------------
public:
    virtual void Reset();

	//! Prepares an array with new particles to emitt into the system
	//! and returns how much new particles there are.
	virtual int Emitt(Real now, Real timeSinceLastCall, SParticle*& outArray, uint32 nFinalMaxParticles);

	//! Get emitter type
	virtual E_PARTICLE_EMITTER_TYPE GetType() const { return EPET_LINE; }

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
public:
    // 线段的终止位置(粒子从线段的Pos到End直接产生)
	void SetEnd(const Vector3& vEnd){m_vEnd = vEnd;}
	const Vector3& GetEnd() const{return m_vEnd;}

    // 每次产生距离起始点的最小距离（取值范围(0,线段的长度]），粒子递增产生，超过线段的长度后产生新的粒子的生命周期就为0了
	void SetMinIncrement(Real minIncrement){mMinIncrement = minIncrement;}
	Real GetMinIncrement() const{return mMinIncrement;}

    // 每次产生距离起始点的最大距离（取值范围(0,线段的长度]），粒子递增产生，超过线段的长度后产生新的粒子的生命周期就为0了
	void SetMaxIncrement(Real maxIncrement){mMaxIncrement = maxIncrement;}
	Real GetMaxIncrement() const{return mMaxIncrement;}

    // 距离线段的最大偏移垂直距离
	void SetMaxDeviation(Real maxDeviation){mMaxDeviation = maxDeviation;}
	Real GetMaxDeviation() const{return mMaxDeviation;}
private:
	Vector3 m_vEnd;

	Real mMinIncrement;
	Real mMaxIncrement;
	Real mMaxDeviation;

	Vector3 mPerpendicular;
	Real mIncrement;
	Real mLength;
	bool mIncrementsLeft;
	bool mFirst;
};


// ---------------------------------------------------------------------
// Global Function
// ---------------------------------------------------------------------
 RefPtr<CParticleLineEmitter> CreateLineEmitter();

}