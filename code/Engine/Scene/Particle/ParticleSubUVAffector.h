#pragma once
#include "ParticleAffector.h"

namespace ma
{

//! Particle Affector for UV particles about a point
class  CParticleSubUVAffector : public CParticleAffector
{
public:
	CParticleSubUVAffector();
	~CParticleSubUVAffector(void);

	// ---------------------------------------------------------------------
	// Inherite from CParticleAffector
	// ---------------------------------------------------------------------
public:
	virtual void Init(SParticle* particlearray, uint32 count);

	//! Affects a particle.
	virtual void Affect(Real now, Real timediff, LST_PARTICLE& lstParticles);

	//! Get emitter type
	virtual E_PARTICLE_AFFECTOR_TYPE GetType() const { return EPAT_SUB_UV; }

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
	void SetTile(int nXTile,int nYTile);

    // 贴图的U方向的格子数量(>=1)
	void SetXTile(int nXTile) {SetTile(nXTile,m_nXTile);}
    int GetXTile() const {return m_nXTile;}

    // 贴图的V方向的格子数量(>=1)
	void SetYTile(int nYTile) {SetTile(m_nXTile,nYTile);}
	int GetYTile() const {return m_nYTile;}

    // 根据生命周期走格的循环次数(>=1)
    void SetCycles(int nCycles) {m_nCycles = nCycles;}
    int GetCycles() const {return m_nCycles;}

    // 是否要固定格子，默认false。若为true，表示则粒子产生的时候随机取其中的一格
    void SetFixedTile(bool bEnabled){m_bFixedTile = bEnabled;}
    bool GetFixedTile() const{return m_bFixedTile;}

    // 格子插值(若为true，则材质particlecpu里面要把TEXTURE_BLEND开启才有效果)
    void SetTileBlend(bool b){m_bTileBlend = b;}
    bool GetTileBlend() const {return m_bTileBlend;}

    // 走格速度是否根据粒子的生命周期，默认true。若为false，表示使用FrameRate进行控制走格速度
    void SetUseEmitterTime(bool b){m_bUseEmitterTime = b;}
    bool GetUseEmitterTime() const{return m_bUseEmitterTime;}

    // 默认每帧间隔0.033秒
    void SetFrameRate(float fRate){m_fRate = fRate;}
    float GetFrameRate() const{return m_fRate;}

    // 走格的起始索引（默认为0表示每次都是从0开始）；>0表示随机从起始索引里面取一个值
    void SetStartIndex(int nStartIndex);
    int GetStartIndex() const{return m_nStartIndex;}

private:
    uint32 WeightToBlend(float fWeight0,float fWeight1,float fWeight2,float fWeight3);

private:
    bool m_bTileBlend;
	int m_nXTile;
	int	m_nYTile;
	int m_nCycles;
    int m_nStartIndex;
    bool m_bFixedTile;
    bool m_bUseEmitterTime;
    float m_fRate;

	typedef vector<Rectangle> VEC_UV;
	VEC_UV m_vecTileUV;
};

 RefPtr<CParticleSubUVAffector> CreateSubUVAffector();
}