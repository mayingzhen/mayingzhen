#include "ParticleSystem.h"
#include "ParticleBoxEmitter.h"


namespace ma
{

float g_fParticlePercent = 1.f;

CParticleSystem::CParticleSystem()
{
    m_bParticlesScaleAffect = false;
	m_bParallelUpdate = true;
    m_nPlayState = 0;
	mIsEmitting = true;
	mNonVisibleTimeout = 1.0;
	mLastVisibleFrame = 0;
	mLastEmitTime = 0;
	mTimeSinceLastVisible = 0;
	mBoundsUpdateTime = 10.f;//秒
	m_bParticlesAreGlobal = false;
	m_nFinalMaxParticles = m_nMaxParticles = 30;
    m_bLodEnabled = true;
	m_eBillboardType = BBT_AlignViewPlane;
    m_fPlaySpeed = 1;
	m_lstParticles.clear();
	m_pCPURenderable = NULL;

	m_bOnLoadOver = false;
	mFastFarward = Vector2(0.f, 0.1f);

	m_fMotionInherite = 0;
	m_bMotionInheriteInited = false;
	m_vPreWorldPos = Vector3::ZERO;
	m_vPreDir = Vector3::ZERO;
}

CParticleSystem::~CParticleSystem(void)
{
	this->RemoveAllAffectors();
}

void CParticleSystem::RegisterAttribute()
{
	REGISTER_OBJECT(CParticleSystem,CreateParticleSystem); 

	ACCESSOR_ATTRIBUTE(CParticleSystem, "MtlPath", GetMaterialFile, SetMaterialFile, const char*, NULL, AM_DEFAULT);
}


void CParticleSystem::Update()
{
	RenderComponent::Update();

	if(!this->IsReady())
		return;

    if (m_nPlayState == 0)
    {
        return;
    }

	if (this->GetAABB().isNull())
	{
		this->InitBounds();
	}
    else if (m_bMatrixDirty && m_bParticlesAreGlobal)
    {
        AABB aabb = this->GetAABB();
        aabb.merge(m_pSceneNode->GetPosWS());
        this->SetAABB(aabb);
    }

	Real timeDiff = GetTimer()->GetFrameDeltaTime() * m_fPlaySpeed;
	if (mNonVisibleTimeout > 0)
	{
		// Check whether it's been more than one frame (update is ahead of
		// camera notification by one frame because of the ordering)
		if(GetTimer()->GetFrameCount() - mLastVisibleFrame > 1)
		{
			mTimeSinceLastVisible += timeDiff;
			if (mTimeSinceLastVisible >= mNonVisibleTimeout)//超过ms不在视锥体内就不更新了
			{
				return;
			}
		}
	}

    if (m_nPlayState == 1)
    {
        this->FastForward(mFastFarward.x, mFastFarward.y);
        m_nPlayState = 2;
    }

	if (GetJobScheduler()->GetNumThreads() > 0 && m_bParallelUpdate)
	{
		m_pSceneNode->GetScene()->AddParallelUpdate(this);	
	}
	else
	{
		ParallelUpdate();
		EndParallelUpdate();
	}
}

void CParticleSystem::ParallelUpdate()
{
	AutoLock lock(m_csParallelUpdate);

	Real timeDiff = GetTimer()->GetFrameDeltaTime() * m_fPlaySpeed;
	this->DoParticleSystem(timeDiff);

	if (m_lstParticles.empty())
	{
		return;
	}

	bool bNeedCalcBounds = true;
	if (mBoundsUpdateTime > 0)
	{
		bNeedCalcBounds = (mLastEmitTime <= mBoundsUpdateTime);
	}

	mAABBTemp.setNull();
	if (bNeedCalcBounds || (m_bMatrixDirty && m_bParticlesAreGlobal))
	{
		mAABBTemp = this->_updateBounds();
	}
}


void CParticleSystem::EndParallelUpdate()
{
	if ( !mAABBTemp.isNull() )
	{
		this->SetAABB(mAABBTemp);
	}
}

void CParticleSystem::ParallelShow(Camera* pCamera)
{
    if (m_pMaterialSet == NULL)
        return;

	AutoLock lock(m_csParallelUpdate);

	this->ShowCPUPoint(pCamera);
}

void CParticleSystem::Show(Camera* pCamera)
{
	RenderComponent::Show(pCamera);

	if (!m_bOnLoadOver)
	{
		return;
	}

	ASSERT(this->GetVisible());
	mTimeSinceLastVisible = 0;
	mLastVisibleFrame = GetTimer()->GetFrameCount();

	if (m_lstParticles.empty())
	{
		return;
	}

	if (GetJobScheduler()->GetNumThreads() > 0 && m_bParallelUpdate)
	{
		m_pSceneNode->GetScene()->AddParallelShow(this);
	}
	else
	{
		ParallelShow(pCamera);
	}

    m_pSceneNode->GetScene()->GetRenderQueue()->AddRenderObj(RL_Particle,m_pCPURenderable.get());
}


void CParticleSystem::Reset()
{
	AutoLock lock(m_csParallelUpdate);

    for (VEC_EMITTER::iterator iter = m_vecEmitter.begin();iter != m_vecEmitter.end();++iter)
    {
        (*iter)->Reset();
    }

    for (VEC_AFFECTOR::iterator iter = m_vecAffectors.begin();iter != m_vecAffectors.end();++iter)
    {
        (*iter)->Reset();
    }

	m_lstParticles.clear();
	mLastEmitTime = 0;
    mIsEmitting = true;
	m_bMotionInheriteInited = false;

	this->InitBounds();
}

void CParticleSystem::ShowCPUPoint(Camera* pCamera)
{
	if (pCamera == NULL)
		return;

	// reallocate arrays, if they are too small
	ReallocateBuffers();

	// create particle vertex data
	int idx = 0;
	for (LST_PARTICLE::iterator iter = m_lstParticles.begin();iter != m_lstParticles.end();++iter)
	{
		SParticle& particle = (*iter);
		uint32 argb = particle.color.getAsARGB();
		Vector3 vPPos = particle.pos;
		if (!m_bParticlesAreGlobal)
		{
			vPPos = m_pSceneNode->GetMatrixWS() * vPPos;
		}

        float fParticleWidth = particle.size.width;
		float fParticleHeight = particle.size.height;
        if (m_bParticlesScaleAffect)
        {
            fParticleWidth *= m_pSceneNode->GetScaleWS().x;
            fParticleHeight *= m_pSceneNode->GetScaleWS().y;
        }

		// SetUV
        //        ^y
        //     3  |   0
        //  ---------->x
        //     2  |   1
		const Rectangle& uv = particle.uv;
		m_pCPURenderable->vertices[0+idx].uv = Vector2(uv.right,uv.top);
		m_pCPURenderable->vertices[1+idx].uv = Vector2(uv.right,uv.bottom);
		m_pCPURenderable->vertices[2+idx].uv = Vector2(uv.left,uv.bottom);
		m_pCPURenderable->vertices[3+idx].uv = Vector2(uv.left,uv.top);
		
		const Rectangle& nextUV = particle.nextUV;
		m_pCPURenderable->vertices[0+idx].nextUV = Vector2(nextUV.right,nextUV.top);
		m_pCPURenderable->vertices[1+idx].nextUV = Vector2(nextUV.right,nextUV.bottom);
		m_pCPURenderable->vertices[2+idx].nextUV = Vector2(nextUV.left,nextUV.bottom);
		m_pCPURenderable->vertices[3+idx].nextUV = Vector2(nextUV.left,nextUV.top);
		m_pCPURenderable->vertices[0+idx].blend = particle.nBlend;
		m_pCPURenderable->vertices[1+idx].blend = particle.nBlend;
		m_pCPURenderable->vertices[2+idx].blend = particle.nBlend;
		m_pCPURenderable->vertices[3+idx].blend = particle.nBlend;
	
		if (m_eBillboardType == BBT_AlignViewPlane)
		{
			float fHalfWidth = fParticleWidth/2;
			const Vector3 horizontal = pCamera->GetRight()*fHalfWidth;

			float fHalfHeight = fParticleHeight/2;
			const Vector3 vertical = pCamera->GetUp()*fHalfHeight;

			if (particle.rotation == Radian(0.f))
			{
				m_pCPURenderable->vertices[0+idx].pos = vPPos + horizontal + vertical;
				m_pCPURenderable->vertices[0+idx].color = argb;

				m_pCPURenderable->vertices[1+idx].pos = vPPos + horizontal - vertical;
				m_pCPURenderable->vertices[1+idx].color = argb;

				m_pCPURenderable->vertices[2+idx].pos = vPPos - horizontal - vertical;
				m_pCPURenderable->vertices[2+idx].color = argb;

				m_pCPURenderable->vertices[3+idx].pos = vPPos - horizontal + vertical;
				m_pCPURenderable->vertices[3+idx].color = argb;
			}
			else
			{
				Quaternion qRotate(particle.rotation, pCamera->GetForward());
				m_pCPURenderable->vertices[0+idx].pos = vPPos + qRotate*(horizontal + vertical);
				m_pCPURenderable->vertices[0+idx].color = argb;

				m_pCPURenderable->vertices[1+idx].pos = vPPos + qRotate*(horizontal - vertical);
				m_pCPURenderable->vertices[1+idx].color = argb;

				m_pCPURenderable->vertices[2+idx].pos = vPPos + qRotate*(- horizontal - vertical);
				m_pCPURenderable->vertices[2+idx].color = argb;

				m_pCPURenderable->vertices[3+idx].pos = vPPos + qRotate*(- horizontal + vertical);
				m_pCPURenderable->vertices[3+idx].color = argb;
			}
			
		}
		else if(m_eBillboardType == BBT_AlignBottomCenter) // 一般用于闪电链
		{
			Vector3 vDirection(particle.orientation.x,particle.orientation.y,particle.orientation.z);
			Vector3 vY = vDirection.normalisedCopy();

			const Vector3& vEye = pCamera->GetPosWS();
			Vector3 vZ = vEye - vPPos;
			vZ.normalise();
			Vector3 vX = vY.crossProduct(vZ);

			float fHalfWidth = 0.5f * fParticleWidth;
			m_pCPURenderable->vertices[0+idx].pos = vPPos + vX*fHalfWidth + vY*fParticleHeight;
			m_pCPURenderable->vertices[0+idx].color = argb;;

			m_pCPURenderable->vertices[3+idx].pos = vPPos - vX*fHalfWidth + vY*fParticleHeight;
			m_pCPURenderable->vertices[3+idx].color = argb;

			m_pCPURenderable->vertices[2+idx].pos = vPPos - vX*fHalfWidth;
			m_pCPURenderable->vertices[2+idx].color = argb;

			m_pCPURenderable->vertices[1+idx].pos = vPPos + vX*fHalfWidth;
			m_pCPURenderable->vertices[1+idx].color = argb;
		}
		else
		{
			float fHalfWidth = fParticleWidth/2;
			const Vector3 horizontal = Vector3::UNIT_X*fHalfWidth;

			float fHalfHeight = fParticleHeight/2;
			const Vector3 vertical = Vector3::UNIT_Y*fHalfHeight;

            Matrix4 matBillboard;
            if (m_eBillboardType == BBT_AlignPos || m_eBillboardType == BBT_ZAlignPos)
            {
                if (particle.prePos != particle.pos)
                {
                    particle.preAlignDirection = particle.pos - particle.prePos;
                    particle.prePos = particle.pos;
                }
 
                CalcBillboardMatrix(pCamera,m_eBillboardType, particle.pos, particle.preAlignDirection, matBillboard);
            }
            else if (m_eBillboardType == BBT_None)
            {
                matBillboard = m_pSceneNode->GetRotationWS();
            }
            else
            {
                if (!m_bParticlesAreGlobal)
                {
                    Vector4 v4 = m_pSceneNode->GetMatrixWS()*Vector4(particle.vector.x, particle.vector.y, particle.vector.z, 0);
                    CalcBillboardMatrix(pCamera,m_eBillboardType, vPPos, Vector3(v4.x, v4.y, v4.z), matBillboard);
                }
                else
                {
                    CalcBillboardMatrix(pCamera,m_eBillboardType, vPPos, particle.vector, matBillboard);
                }
            }

			if (particle.rotation != Radian(0.f))
			{
				Quaternion qRotate(particle.rotation, Vector3::UNIT_Z);
				matBillboard = matBillboard*Matrix4(qRotate);
			}

			m_pCPURenderable->vertices[0+idx].pos = vPPos + matBillboard*(horizontal + vertical);
			m_pCPURenderable->vertices[0+idx].color = argb;

			m_pCPURenderable->vertices[1+idx].pos = vPPos + matBillboard*(horizontal - vertical);
			m_pCPURenderable->vertices[1+idx].color = argb;

			m_pCPURenderable->vertices[2+idx].pos = vPPos + matBillboard*(- horizontal - vertical);
			m_pCPURenderable->vertices[2+idx].color = argb;

			m_pCPURenderable->vertices[3+idx].pos = vPPos + matBillboard*(- horizontal + vertical);
			m_pCPURenderable->vertices[3+idx].color = argb;
		}
		
		idx +=4;
	}

	m_pCPURenderable->m_nNumVertices = m_lstParticles.size()*4;
	m_pCPURenderable->m_nNumIndices = m_lstParticles.size()*6;
}


void CParticleSystem::SetParticlesAreGlobal( bool global/*=true*/ )
{
	if (m_bParticlesAreGlobal == global)
	{
		return;
	}

	m_bParticlesAreGlobal = global;
	if (m_bParticlesAreGlobal)
	{
		for (VEC_AFFECTOR::iterator iter = m_vecAffectors.begin();iter != m_vecAffectors.end();++iter)
		{
			CParticleAffector* pAffector = (*iter).get();
			pAffector->SetParentMatrix(m_pSceneNode->GetMatrixWS());
			pAffector->SetParentScale(m_pSceneNode->GetScaleWS());
		}
	}
	else
	{
		for (VEC_AFFECTOR::iterator iter = m_vecAffectors.begin();iter != m_vecAffectors.end();++iter)
		{
			CParticleAffector* pAffector = (*iter).get();
			pAffector->SetParentMatrix(Matrix4::IDENTITY);
			pAffector->SetParentScale(Vector3::UNIT_SCALE);
		}
	}
}

CParticleEmitter* CParticleSystem::GetEmitterByIndex(uint32 nIndex) const 
{
	AutoLock lock(m_csParallelUpdate);

	return m_vecEmitter[nIndex].get();
}

void CParticleSystem::AddEmitter( CParticleEmitter* emitter )
{
	AutoLock lock(m_csParallelUpdate);

	ASSERT(emitter != NULL);
    emitter->_SetLodEnabled(m_bLodEnabled);
	m_vecEmitter.push_back(emitter);
}

void CParticleSystem::RemoveEmitter( CParticleEmitter* emitter )
{
	AutoLock lock(m_csParallelUpdate);

	VEC_EMITTER::iterator iter = std::find(m_vecEmitter.begin(), m_vecEmitter.end(), emitter);
	if (iter == m_vecEmitter.end())
	{
		return;
	}

	m_vecEmitter.erase(iter);
}

void CParticleSystem::RemoveAllEmitters(void)
{
	AutoLock lock(m_csParallelUpdate);

	m_vecEmitter.clear();
}


void CParticleSystem::AddAffector( CParticleAffector* affector )
{
	AutoLock lock(m_csParallelUpdate);

	m_vecAffectors.push_back(affector);
}

void CParticleSystem::RemoveAffector( CParticleAffector* affector )
{
	AutoLock lock(m_csParallelUpdate);

	VEC_AFFECTOR::iterator iter = std::find(m_vecAffectors.begin(), m_vecAffectors.end(), affector);
	if (iter == m_vecAffectors.end())
	{
		return;
	}
	
	m_vecAffectors.erase(iter);
}

void CParticleSystem::RemoveAllAffectors()
{
	AutoLock lock(m_csParallelUpdate);

	m_vecAffectors.clear();
}

// ---------------------------------------------------------------------
// Self
// ---------------------------------------------------------------------

void CParticleSystem::DoParticleSystem(Real timediff)
{ 
	if (timediff <= 0)
	{
		return;
	}

	Real now = mLastEmitTime + timediff;
	mLastEmitTime = now;

	if (mIsEmitting)
	{
		if (m_fMotionInherite > 0 && m_bParticlesAreGlobal)
		{
			if (m_bMotionInheriteInited)
			{
				Vector3 vDir = (m_pSceneNode->GetPosWS() - m_vPreWorldPos);
				float fSpeed = vDir.length()/timediff;
				m_vPreDir = vDir.normalisedCopy()*fSpeed;
				m_vPreWorldPos = m_pSceneNode->GetPosWS();
			}
			else
			{
				m_bMotionInheriteInited = true;
				m_vPreDir = Vector3::ZERO;
				m_vPreWorldPos = m_pSceneNode->GetPosWS();
			}

		}
		uint32 emitterCount = m_vecEmitter.size();

		// run emitter
		if (m_vecRequested.size() != emitterCount)
			m_vecRequested.resize(emitterCount);

		if (m_vecArray.size() != emitterCount)
			m_vecArray.resize(emitterCount);

		uint32 nTotalRequested = 0;
		uint32 i = 0;
		VEC_EMITTER::iterator iter;
		for (iter = m_vecEmitter.begin(), i = 0;iter != m_vecEmitter.end();++iter, ++i)
		{
			SParticle* array = 0;
			m_vecRequested[i] = (*iter)->Emitt(now, timediff, array, m_nFinalMaxParticles);
			m_vecArray[i] = array;
			nTotalRequested += m_vecRequested[i];
		}

		// Check if the quota will be exceeded, if so reduce demand
		Real ratio =  1.0f;
		if (nTotalRequested > m_nFinalMaxParticles)
		{
			// Apportion down requested values to allotted values
			ratio =  (Real)m_nFinalMaxParticles / (Real)nTotalRequested;
			for (i = 0; i < emitterCount; ++i)
			{
				m_vecRequested[i] = static_cast<unsigned>(m_vecRequested[i] * ratio);
			}
		}

		for (iter = m_vecEmitter.begin(), i = 0;iter != m_vecEmitter.end();++iter, ++i)
		{
			SParticle* array = m_vecArray[i];
			int newParticles = m_vecRequested[i];

			if (newParticles && array)
			{
				int j=m_lstParticles.size();
				if ((int)newParticles > (int)(m_nFinalMaxParticles-j))
					newParticles= (int)m_nFinalMaxParticles-j;

                for (VEC_AFFECTOR::iterator ait = m_vecAffectors.begin();ait != m_vecAffectors.end(); ++ait)
                {
                    (*ait)->Init(array, newParticles);
                }

				for (int i=0; i<newParticles; ++i)
				{
					m_lstParticles.push_back(array[i]);
					SParticle& p = m_lstParticles.back();
                    
					if (m_bParticlesAreGlobal)
					{
                        Matrix3 mat3x3;
                        m_pSceneNode->GetMatrixWS().extract3x3Matrix(mat3x3);
                        p.startVector = mat3x3*p.startVector;
                        p.vector = p.startVector;
						p.prePos = p.startPos = p.pos =  m_pSceneNode->GetMatrixWS()*p.pos;
						if (m_fMotionInherite > 0)
						{
							p.vector = p.startVector = p.startVector + m_vPreDir*m_fMotionInherite;
						}
					}
                    p.preAlignDirection = p.startVector;
				}
			}
		}
	}

	if(m_lstParticles.empty())
	{
        
		return;
	}

	// run affectors
	VEC_AFFECTOR::iterator ait = m_vecAffectors.begin();
	for (; ait != m_vecAffectors.end(); ++ait)
		(*ait)->Affect(now, timediff, m_lstParticles);

	// animate all particles
	for (LST_PARTICLE::iterator iter = m_lstParticles.begin();iter != m_lstParticles.end();)
	{
		SParticle& p = *iter;

		// erase is pretty expensive!
		if (now > p.endTime)
		{
			m_lstParticles.erase(iter++);
		}
		else
		{
			p.pos += (p.vector * timediff);
			++iter;
		}
	}
}

void CParticleSystem::ReallocateBuffers()
{
	if (m_lstParticles.size() * 4 > m_pCPURenderable->vertices.size() ||
		m_lstParticles.size() * 6 > m_pCPURenderable->indices.size())
	{
		uint32 oldSize = m_pCPURenderable->vertices.size();
		m_pCPURenderable->vertices.resize(m_lstParticles.size() * 4);

		uint32 i;

		// fill remaining indices
		uint32 oldIdxSize = m_pCPURenderable->indices.size();
		uint32 oldvertices = oldSize;
		m_pCPURenderable->indices.resize(m_lstParticles.size() * 6);

		for (i=oldIdxSize; i<m_pCPURenderable->indices.size(); i+=6)
		{
			m_pCPURenderable->indices[0+i] = (uint16)0+oldvertices;
			m_pCPURenderable->indices[1+i] = (uint16)1+oldvertices;
			m_pCPURenderable->indices[2+i] = (uint16)2+oldvertices;
			m_pCPURenderable->indices[3+i] = (uint16)0+oldvertices;
			m_pCPURenderable->indices[4+i] = (uint16)2+oldvertices;
			m_pCPURenderable->indices[5+i] = (uint16)3+oldvertices;
			oldvertices += 4;
		}
	}
}



bool CParticleSystem::IsReady()
{
    if (m_bOnLoadOver)
    {
        return true;
    }

    if (m_pMaterialSet && !m_pMaterialSet->IsReady())
    {
        return false;
    }

    if (m_pMaterialSet != NULL)
    {
        SubMaterial* pMaterial = m_pMaterialSet->GetSubMaterialByIndex(0,0);

        m_pCPURenderable = new CParticleSystemRenderable(this);

        m_pCPURenderable->m_pSubMaterial = pMaterial;
	}
    
    m_bOnLoadOver = true;

	return true;
}


void CParticleSystem::SetMaterialSet(Material* pMaterialSet)
{
    if (m_pMaterialSet == pMaterialSet)
        return;

    m_pMaterialSet = pMaterialSet;

    m_bOnLoadOver= false;
    IsReady();
}

const char*	CParticleSystem::GetMaterialFile() const
{
	return m_pMaterialSet ? m_pMaterialSet->GetXMLFile()->GetResPath() : NULL;
}

void CParticleSystem::SetMaterialFile(const char* pFile)
{
	SetMaterialSet( CreateMaterial(pFile).get() );
}


Renderable* CParticleSystem::GetRenderable() const
{
	return m_pCPURenderable.get();
}

void CParticleSystem::SetMaxParticles( uint32 num )
{
	m_nFinalMaxParticles = m_nMaxParticles = num;
    if (m_bLodEnabled)
    {
		m_nFinalMaxParticles = m_nMaxParticles>0?__max(1,uint32(m_nMaxParticles*g_fParticlePercent)):0;
    }
  
}

void CParticleSystem::FastForward( Real time, Real interval)
{
	for (Real i = 0;i< time;i+= interval)
	{
		this->DoParticleSystem(interval);
	}
	
	this->_updateBounds();
}


AABB CParticleSystem::_updateBounds( void )
{
	if (m_lstParticles.empty())
	{
		return AABB();
	}

	Vector3 vBoundsMin, vBoundsMax;
	vBoundsMin.x = vBoundsMin.y = vBoundsMin.z = Math::POS_INFINITY;
	vBoundsMax.x = vBoundsMax.y = vBoundsMax.z = Math::NEG_INFINITY;

	if (mBoundsUpdateTime > 0 && mLastEmitTime <= mBoundsUpdateTime)
	{
		// 在原有bounds上面扩大
		if (m_bParticlesAreGlobal)
		{
			const AABB& box = this->GetAABB();
			vBoundsMin = box.getMinimum();
			vBoundsMax = box.getMaximum();
		}
		else
		{
			const AABB& box = this->GetAABB();
			vBoundsMin = box.getMinimum();
			vBoundsMax = box.getMaximum();
		}

	}

	Vector3 halfScale = Vector3::UNIT_SCALE * 0.5f;

	// animate all particles
	for (LST_PARTICLE::iterator iter = m_lstParticles.begin();iter != m_lstParticles.end();++iter)
	{
		const SParticle& p = *iter;
		Vector3 padding = halfScale * __max(p.size.width, p.size.height);
		vBoundsMin.makeFloor(p.pos - padding);
		vBoundsMax.makeCeil(p.pos + padding);
	}

	AABB aabb(vBoundsMin, vBoundsMax);

	return aabb;
}

void CParticleSystem::InitBounds()
{
	mLastVisibleFrame = GetTimer()->GetFrameCount();

	AABB aabb;
	for (VEC_EMITTER::iterator iter = m_vecEmitter.begin();iter != m_vecEmitter.end();++iter)
	{
		Vector3 vPos = (*iter)->GetPos();
		if (m_bParticlesAreGlobal)
		{
			vPos = m_pSceneNode->GetMatrixWS() * vPos;
		}
		aabb.merge(vPos);
	}
	this->SetAABB(aabb);
}

const SParticle& CParticleSystem::GetParticleByIndex( uint32 nIndex ) const
{
	LST_PARTICLE::const_iterator iter = m_lstParticles.begin();
	std::advance(iter, nIndex);
	return *iter;
}

void CParticleSystem::Play(uint32 nElapsedTime, bool bCascade)
{
	AutoLock lock(m_csParallelUpdate);

    this->SetEmitting(true);
    mLastEmitTime = 0;
    if (nElapsedTime > 0)
    {
        this->SetFastForward(Vector2(nElapsedTime*0.001f, 0.1f));
    }
    m_nPlayState = 1;

    m_lstParticles.clear();

    for (VEC_EMITTER::iterator iter = m_vecEmitter.begin();iter != m_vecEmitter.end();++iter)
    {
        (*iter)->Reset();
    }

    for (VEC_AFFECTOR::iterator iter = m_vecAffectors.begin();iter != m_vecAffectors.end();++iter)
    {
        (*iter)->Reset();
    }
}

void CParticleSystem::Stop(bool bCascade)
{
    AutoLock lock(m_csParallelUpdate);

    this->SetEmitting(false);
}

void CParticleSystem::SetPlaySpeed(float fPlaySpeed, bool bCascade)
{
    m_fPlaySpeed = fPlaySpeed;
}

void CParticleSystem::SetLodEnabled( bool bEnabled )
{
    AutoLock lock(m_csParallelUpdate);

    m_bLodEnabled = bEnabled;
    this->SetMaxParticles(m_nMaxParticles);

    for (VEC_EMITTER::iterator iter = m_vecEmitter.begin();iter != m_vecEmitter.end(); ++iter)
    {
        CParticleEmitter* pEmitter = (*iter).get();
        pEmitter->_SetLodEnabled(bEnabled);
    }
}

void CParticleSystem::UpdateGlobalAffectors()
{
    if (m_bParticlesAreGlobal)
    {
        for (VEC_AFFECTOR::iterator iter = m_vecAffectors.begin();iter != m_vecAffectors.end();++iter)
        {
            CParticleAffector* pAffector = (*iter).get();
            pAffector->SetParentMatrix(m_pSceneNode->GetMatrixWS());
            pAffector->SetParentScale(m_pSceneNode->GetScaleWS());
        }
    }
}

bool CParticleSystem::CalcBillboardMatrix(Camera* pCamera,BillboardType eType, const Vector3& vWorldPos, const Vector3& vDirection, OUT Matrix4& matBillboard)
{
	Matrix4 matView = pCamera->GetMatView();

	Vector3 vX, vY, vZ;
	switch (eType)
	{
	case BBT_None:
		matBillboard = Matrix4::IDENTITY;
		return true;
	case BBT_AlignViewPlane:
		vX = Vector3(matView[0][0], matView[0][1], matView[0][2]);
		vY = Vector3(matView[1][0], matView[1][1], matView[1][2]);
		vZ = Vector3(matView[2][0], matView[2][1], matView[2][2]);
		break;
	case BBT_AlignViewPoint:
		{
			const Vector3& vEye = pCamera->GetPosWS();
			vZ = vEye - vWorldPos;
			vZ.normalise();
			vX = Vector3(matView[0][0], matView[0][1], matView[0][2]);
			vY = vZ.crossProduct(vX);
			vY.normalise();
			vX = vY.crossProduct(vZ);
			vX.normalise();
		}
		break;
	case BBT_AlignViewPlane_YConstraint:
		{
			if ( (Math::RealEqual(matView[0][0], 0) && Math::RealEqual(matView[0][1], 0))
				|| (Math::RealEqual(matView[2][0], 0) && Math::RealEqual(matView[2][1], 0)))
			{
				vX = Vector3(1,0,0);
				vY = Vector3(0,1,0);
				vZ = Vector3(0,0,1);
			}
			else
			{
				vX = Vector3(matView[0][0], matView[0][1], 0);
				vZ = Vector3(matView[2][0], matView[2][1], 0);
				vX.normalise();
				vZ.normalise();
				vY = vZ.crossProduct(vX);
				vY.normalise();
			}
		}
		break;
	case BBT_AlignViewPoint_YConstraint:
		{
			const Vector3& vEye = pCamera->GetPosWS();
			vZ = vEye - vWorldPos;

			if ( (Math::RealEqual(vZ.x, 0) && Math::RealEqual(vZ.y, 0))
				|| (Math::RealEqual(matView[0][0], 0) && Math::RealEqual(matView[0][1], 0)))
			{
				vX = Vector3(1,0,0);
				vY = Vector3(0,1,0);
				vZ = Vector3(0,0,1);
			}
			else
			{
				vZ.z = 0;
				vY = vZ.crossProduct(Vector3(matView[0][0], matView[0][1], 0));
				vZ.normalise();
				vY.normalise();
				vX = vY.crossProduct(vZ);
				vX.normalise();
			}
		}
		break;
	case BBT_AlignDirection:
	case BBT_AlignPos:
		{
			vY = vDirection.normalisedCopy();

			const Vector3& vEye = pCamera->GetPosWS();
			vZ = vEye - vWorldPos;
			vZ.normalise();
			vX = vY.crossProduct(vZ);
			vX.normalise();
			vZ = vX.crossProduct(vY);
			vZ.normalise();
		}
		break;
	case BBT_ZAlignPos:
	case BBT_ZAlignDirection:
		{
			Quaternion qRotate = Vector3::UNIT_Z.getRotationTo(vDirection.normalisedCopy());
			matBillboard = Matrix4(qRotate);
			return true;
		}
		break;
	default:
		ASSERT(false);
		return false;
	}

	matBillboard = Matrix4(Quaternion(vX, vY, vZ));
	return true;
}

bool CParticleSystem::Improt(rapidxml::xml_node<>* pXmlElem)
{
	Serializable::Improt(pXmlElem);

	rapidxml::xml_node<>* pXmlEmitter = pXmlElem->first_node("Emitter");
	while(pXmlEmitter)
	{
		const char* pszType = pXmlEmitter->findAttribute("ClassName");

		RefPtr<CParticleEmitter> pEmitter = CreateObject<CParticleEmitter>(pszType);
		this->AddEmitter(pEmitter.get());

		pEmitter->Improt(pXmlEmitter);

		pXmlEmitter = pXmlEmitter->next_sibling("Emitter");
	}

	rapidxml::xml_node<>* pXmlAffector = pXmlElem->first_node("Affector");
	while(pXmlAffector)
	{
		const char* pszType = pXmlAffector->findAttribute("ClassName");

		RefPtr<CParticleAffector> pAffector = CreateObject<CParticleAffector>(pszType);
		this->AddAffector(pAffector.get());

		pAffector->Improt(pXmlAffector);

		pXmlAffector = pXmlAffector->next_sibling("Affector");
	}

	return true;
}

bool CParticleSystem::Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc)
{
	Serializable::Export(pXmlElem,doc);

	for (UINT i = 0; i < m_vecEmitter.size(); ++i)
	{
		rapidxml::xml_node<>* pXmlEmitter = doc.allocate_node(rapidxml::node_element, doc.allocate_string("Emitter"));
		pXmlElem->append_node(pXmlEmitter);

		CParticleEmitter* pCParticleEmitter = m_vecEmitter[i].get();
		pCParticleEmitter->Export(pXmlEmitter,doc);
	}

	for (UINT i = 0; i < m_vecAffectors.size(); ++i)
	{
		rapidxml::xml_node<>* pXmlAffector = doc.allocate_node(rapidxml::node_element, doc.allocate_string("Affector"));
		pXmlElem->append_node(pXmlAffector);

		CParticleAffector* pAffector = m_vecAffectors[i].get();
		pAffector->Export(pXmlAffector,doc);
	}

	return true;
}

// ---------------------------------------------------------------------
// Global Function
// ---------------------------------------------------------------------

RefPtr<CParticleSystem> CreateParticleSystem()
{
	RefPtr<CParticleSystem> p =  new CParticleSystem();

	return p;
}


}