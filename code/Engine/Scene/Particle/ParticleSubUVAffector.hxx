#include "ParticleSubUVAffector.h"

namespace ma
{

CParticleSubUVAffector::CParticleSubUVAffector()
{
	m_nXTile = 1;
	m_nYTile = 1;
	m_nCycles = 1;
    m_bFixedTile = false;
    m_bTileBlend = false;
    m_bUseEmitterTime = true;
    m_fRate = 0.033f;
    m_nStartIndex = 0;

	SetTile(m_nXTile,m_nYTile);
}

CParticleSubUVAffector::~CParticleSubUVAffector(void)
{
}

void CParticleSubUVAffector::Init(SParticle* particlearray, uint32 count)
{
    if( !m_bEnabled )
        return;

    if (particlearray == NULL)
        return;

    int nTileCount = m_nXTile * m_nYTile;
    float nRandomStart = m_nStartIndex+0.99f;
    if (m_bFixedTile)
    {
        for(uint32 i=0;i<count;i++)
        {
            SParticle& p = particlearray[i];
            int nStartIndex = (int)(Math::UnitRandom()*nRandomStart);
            p.uv = m_vecTileUV[nStartIndex%nTileCount];
        }
    }
    else
    {
        for(uint32 i=0;i<count;i++)
        {
            SParticle& p = particlearray[i];
            int nStartIndex = (int)(Math::UnitRandom()*nRandomStart);
            p.nStartIndex = nStartIndex;
        }
    }
}

void CParticleSubUVAffector::Affect( Real now, Real timediff, LST_PARTICLE& lstParticles )
{
	if( !m_bEnabled )
		return;
	
    // 若固定格子，则不走格
    if (m_bFixedTile)
    {
        return;
    }

	int nTileCount = m_nXTile * m_nYTile;

	if (m_bTileBlend)
	{
		for(LST_PARTICLE::iterator iter = lstParticles.begin();iter != lstParticles.end();++iter)
		{
			SParticle& p = *iter;

			float fTile = (float)p.nStartIndex;
			if (m_bUseEmitterTime)
			{
				float fPercent = (now - p.startTime) / (p.endTime - p.startTime);
				fTile += fPercent * (nTileCount * m_nCycles);
			}
			else
			{
				fTile += (now - p.startTime) /m_fRate;
			}

			fTile = fmod(fTile, (float)nTileCount);
			int nTile = (int)fTile;
			if (nTile < 0 || nTile >= nTileCount)
			{
				ASSERT(false);
				continue;
			}

			p.uv = m_vecTileUV[nTile];

	
			int nNextTile = nTile <= nTileCount - 2 ? nTile + 1 : 0;
			p.nextUV = m_vecTileUV[nNextTile];
			p.nBlend = WeightToBlend(fTile - nTile,0,0,0);
		}
	}
	else
	{
		for(LST_PARTICLE::iterator iter = lstParticles.begin();iter != lstParticles.end();++iter)
		{
			SParticle& p = *iter;

			int nTile = p.nStartIndex;
			if (m_bUseEmitterTime)
			{
				float fPercent = (now - p.startTime) / (p.endTime - p.startTime);
				nTile += (int)(fPercent * (nTileCount * m_nCycles));
			}
			else
			{

				nTile += (int)((now - p.startTime) / m_fRate);
			}

			nTile = nTile % nTileCount;
			if (nTile < 0 || nTile >= nTileCount)
			{
				ASSERT(false);
				continue;
			}

			p.uv = m_vecTileUV[nTile];
		}
	}
}

uint32 CParticleSubUVAffector::WeightToBlend(float fWeight0,float fWeight1,float fWeight2,float fWeight3)
{
    union UIndex
    {
        uint32 uInde;
        uint8 uByte[4];
    };

    UIndex uBondWeight;
    uBondWeight.uByte[0] = (uint8)(fWeight0 * 255.0f);
    uBondWeight.uByte[1] = (uint8)(fWeight1 * 255.0f);
    uBondWeight.uByte[2] = (uint8)(fWeight2 * 255.0f);
    uBondWeight.uByte[3] = (uint8)(fWeight3 * 255.0f);

    return uBondWeight.uInde;
}

void CParticleSubUVAffector::SetTile(int nXTile,int nYTile)
{
	ASSERT(nXTile >=1 && nYTile >=1);
	if (nXTile < 1 || nYTile < 1)
		return;

	m_nXTile = nXTile;
	m_nYTile = nYTile;

	m_vecTileUV.clear();

	m_vecTileUV.resize(nXTile * nYTile);

	for (int i = 0; i < nXTile; ++i)
	{
		for (int j = 0; j < nYTile; ++j)
		{
			float fLeft = (float)i / (float)nXTile;
			float fTop = (float)j / (float)nYTile;
			float fRight = fLeft + 1.0f / (float)nXTile;
			float fBottom = fTop + 1.0f / (float)nYTile;
			
			m_vecTileUV[j * nXTile + i] = Rectangle(fLeft, fTop, fRight, fBottom);
		}
	}
}

void CParticleSubUVAffector::SetStartIndex( int nStartIndex )
{
    if (nStartIndex < 0)
    {
        nStartIndex = 0;
    }
    m_nStartIndex = nStartIndex;
}

RefPtr<CParticleSubUVAffector> CreateSubUVAffector()
{
	return new CParticleSubUVAffector();
}
}