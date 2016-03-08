#pragma once
#include "../RenderComponent.h"
#include "Particle.h"
#include "ParticleEmitter.h"
#include "ParticleAffector.h"
#include "ParticleSystemRenderable.h"

namespace ma
{

//! A particle system scene node for creating snow, fire, exlosions, smoke...
/** A scene node controlling a particle System. The behavior of the particles
can be controlled by setting the right particle emitters and affectors.
You can for example easily create a campfire by doing this:
*/
class  ParticleSystem : public RenderComponent
{

public:
	ParticleSystem();

	virtual ~ParticleSystem(void);

	DECL_OBJECT(ParticleSystem);

	static void	RegisterAttribute();

	void Update();

	void Show(Camera* pCamera);

	void Reset();

	void ParallelShow(Camera* pCamera);

	void ParallelUpdate();
	void EndParallelUpdate();

    virtual void Play(uint32 nElapsedTime = 0, bool bCascade = false);
    virtual void Stop(bool bCascade = false);
    virtual void SetPlaySpeed(float fPlaySpeed, bool bCascade = false);
	// ---------------------------------------------------------------------
	// 
	// ---------------------------------------------------------------------
public:
	//! Sets if the particles(and any affector effects)  should be global.
	/** If they are, the particles(and any affector effects)  are affected by the movement of the
	particle system scene node too, otherwise they completely ignore it.
	Default is true. */
    // 为true表示：1.affector里面的数值都是global；2.发射出来的粒子也是global，即拖尾效果
	virtual void SetParticlesAreGlobal(bool global);
	bool GetParticlesAreGlobal() const{return m_bParticlesAreGlobal;}
	
    virtual void SetLodEnabled(bool bEnabled);
    virtual bool GetLodEnabled() const{return m_bLodEnabled;}

	//! Gets the particle emitter, which creates the particles.
	/** \return The particle emitter. Can be NULL if none is set. */
	virtual ParticleEmitter* GetEmitterByIndex(uint32 nIndex) const;

	// ! Gets the num of emitters
	uint32 GetNumEmitters() const{return m_vecEmitter.size();}

	//! Sets the particle emitter, which creates the particles.
	virtual void AddEmitter(ParticleEmitter* emitter);
	virtual void RemoveEmitter(ParticleEmitter* emitter);

	/** Removes all the emitters from this system. */
	virtual void RemoveAllEmitters(void);

	//! Adds new particle effector to the particle system.
	virtual void AddAffector(ParticleAffector* affector);
	virtual void RemoveAffector(ParticleAffector* affector);

	//! Removes all particle affectors in the particle system.
	virtual void RemoveAllAffectors();

	/** This is used to turn on or off particle emission for this system.
		@remarks
		By default particle system is always emitting particles (if a emitters exists)
		and this can be used to stop the emission for all emitters. To turn it on again, 
		call it passing true.

		Note that this does not detach the particle system from the scene node, it will 
		still use some CPU.
	*/
	void SetEmitting(bool v){mIsEmitting = v;}

	/** Returns true if the particle system emitting flag is turned on.
	@remarks
		This function will not actually return whether the particles are being emitted.
		It only returns the value of emitting flag.
	*/
	bool GetEmitting() const{return mIsEmitting;}

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
public:
	/** Sets when the particle system should stop updating after it hasn't been
			visible for a while.
	@remarks
		By default, visible particle systems update all the time, even when 
		not in view. This means that they are guaranteed to be consistent when 
		they do enter view. However, this comes at a cost, updating particle
		systems can be expensive, especially if they are perpetual.
	@par
		This option lets you set a 'timeout' on the particle system, so that
		if it isn't visible for this amount of time, it will stop updating
		until it is next visible.
	@param timeout The time after which the particle system will be disabled
		if it is no longer visible. 0 to disable the timeout and always update.
	*/
    // 设置不可见后，继续update的时间(秒)
	void SetNonVisibleTimeout(Real timeout){mNonVisibleTimeout = timeout;}

	/** Gets when the particle system should stop updating after it hasn't been
		visible for a while.
	*/
	Real GetNonVisibleTimeout(void) const { return mNonVisibleTimeout; }

	// 设置boundingbox的更新时间（秒）,为0表示一直更新下去，但是有损耗
	void SetBoundingBoxTimeout(Real timeout){mBoundsUpdateTime = timeout;}
	Real GetBoundingBoxTimeout() const{return mBoundsUpdateTime;}

    // 同一个时刻存在的最多粒子数量
	void SetMaxParticles(uint32 num);
	uint32 GetMaxParticles() const{return m_nMaxParticles;}

    // 粒子billboard类型
	void SetBillboardType(BillboardType eType){m_eBillboardType = eType;}
	BillboardType GetBillboadType() const{return m_eBillboardType;}

  	void SetMotionInherite(float fInherite){m_fMotionInherite = fInherite;}
	float GetMotionInherite() const{return m_fMotionInherite;}

	uint32 GetNumAffectors() const{return m_vecAffectors.size();}
	ParticleAffector* GetAffectorByIndex(uint32 nIndex) const{return m_vecAffectors[nIndex].get();}

	uint32 GetNumParticles() const{return m_lstParticles.size();}
	const SParticle& GetParticleByIndex(uint32 nIndex) const;

	Renderable* GetRenderable() const;

	// 设置材质文件
	void SetMaterialSet(Material* pMaterialSet);
    Material* GetMaterialSet() const{return m_pMaterialSet.get();}

	const char*	GetMaterialFile() const;
	void SetMaterialFile(const char* pFile);


    // 时间快速推进多少秒，其中time为推进的总时间,interval为计算间隔时间(建议0.1)
	const Vector2& GetFastForward() const{return mFastFarward;}
	void SetFastForward(const Vector2& v){mFastFarward = v;}

    // 每个粒子是否受缩放影响
    void SetParticlesScaleAffect(bool bAffect){m_bParticlesScaleAffect = bAffect;}
    bool GetParticlesScaleAffect() const{return m_bParticlesScaleAffect;}

private:
	void ShowCPUPoint(Camera* pCamera);
	void DoParticleSystem(Real timediff);
	bool ReallocateBuffers();

	void InitBounds();

	/** Internal method for updating the bounds of the particle system.*/
	AABB _updateBounds(void);

    /** Fast-forwards this system by the required number of seconds.
    @remarks
        This method allows you to fast-forward a system so that it effectively looks like
        it has already been running for the time you specify. This is useful to avoid the
        'startup sequence' of a system, when you want the system to be fully populated right
        from the start.
    @param
        time The number of seconds to fast-forward by.
    @param
        interval The sampling interval used to generate particles, apply affectors etc. The lower this
        is the more realistic the fast-forward, but it takes more iterations to do it.
    */
    void FastForward(Real time, Real interval = 0.1f);

    void UpdateGlobalAffectors();
	bool CalcBillboardMatrix(Camera* pCamera,BillboardType eType, const Vector3& vWorldPos, const Vector3& vDirection, OUT Matrix4& matBillboard);

	virtual bool Improt(rapidxml::xml_node<>* pXmlElem);
	virtual bool Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc);	

private:
    int m_nPlayState; // 0-int 1-FastForward 2-Play
    float m_fPlaySpeed;

    /// Used to control if the particle system should emit particles or not.
    bool mIsEmitting;
    
    bool m_bParticlesScaleAffect;

	// motion inherite
	float m_fMotionInherite;
	bool m_bMotionInheriteInited;
	Vector3 m_vPreWorldPos;
	Vector3 m_vPreDir;

	// ms
	Real mBoundsUpdateTime;
	Real mTimeSinceLastVisible;
	uint32 mLastVisibleFrame;
	Real mNonVisibleTimeout;

    bool m_bLodEnabled;

	AABB mAABBTemp;
	
	bool m_bParticlesAreGlobal;
	Real mLastEmitTime;
	uint32 m_nMaxParticles;
    uint32 m_nFinalMaxParticles;
	BillboardType m_eBillboardType;

	Vector2 mFastFarward;

	RefPtr<ParticleSystemRenderable> m_pCPURenderable;

    RefPtr<Material> m_pMaterialSet;

	typedef vector<RefPtr<ParticleEmitter> > VEC_EMITTER;
	VEC_EMITTER m_vecEmitter;

	typedef vector<RefPtr<ParticleAffector> > VEC_AFFECTOR;
	VEC_AFFECTOR m_vecAffectors;

	LST_PARTICLE m_lstParticles;

	vector<uint32> m_vecRequested;
	vector<SParticle*> m_vecArray;


private:
	mutable CriticalSection m_csParallelUpdate;	

// ---------------------------------------------------------------------
// Background Loading
// ---------------------------------------------------------------------
private:
	bool IsReady();
    bool m_bOnLoadOver;

	friend class CRenderProbe;
};

RefPtr<ParticleSystem> CreateParticleSystem();

}