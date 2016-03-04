#pragma once
#include "ParticleEmitter.h"

namespace ma
{
class  ParticleMeshVertexEmitter : public ParticleEmitter
{
public:
	ParticleMeshVertexEmitter();
	~ParticleMeshVertexEmitter(void);

	// ---------------------------------------------------------------------
	// Inherite from ParticleEmitter
	// ---------------------------------------------------------------------

	//! Prepares an array with new particles to emitt into the system
	//! and returns how much new particles there are.
	virtual int Emitt(Real now, Real timeSinceLastCall, SParticle*& outArray, uint32 nFinalMaxParticles);

	//! Get emitter type
	virtual E_PARTICLE_EMITTER_TYPE GetType() const { return EPET_MESH_VERTEX; }

	virtual void SetPos( const Vector3& pos );
	virtual void SetDirection( const Vector3& newDirection );

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------
public:
	//! Set Mesh to emit particles from
	virtual void SetMesh(MESHEMITTER_VERTEX* rgVertices, uint32 nNumVertices);

	virtual void SetFile(const char* pszFile){m_strFile = pszFile;}
	virtual const char* GetFile() const{return m_strFile.c_str();}

    // 是否使用法线发射方向
	//! Set whether to use vertex normal for direction, or direction specified
	virtual void SetUseNormalDirection( bool useNormalDirection = true ){mUseNormalDirection = useNormalDirection;}
    virtual bool GetUsingNormalDirection() const{return mUseNormalDirection;}

	//! Get Mesh we're emitting particles from
	virtual uint32 GetNumVertices() const{return mSrcVertices.size();}
	virtual const MESHEMITTER_VERTEX& GetVertexByIndex(uint32 nIndex) const{return mSrcVertices[nIndex];}

	// 模型缩放
	virtual void SetScale(const Vector3& vScale);
	virtual const Vector3& GetScale() const{return mScale;}

public:
	void BeginVertex();
	void AddVertex(const MESHEMITTER_VERTEX& v);
	void EndVertex();
private:
	Quaternion CalcRotate() const;
private:
	string m_strFile;
	bool mUseNormalDirection;

	vector<MESHEMITTER_VERTEX> mSrcVertices;
	Vector3 mScale;
    Quaternion mRotate;
};

//! Creates a mesh particle emitter.
 RefPtr<ParticleMeshVertexEmitter> CreateMeshVertexEmitter();

}