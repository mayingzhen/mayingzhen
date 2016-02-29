#include "S3EngineCommon.h"
#include "ParticleMeshSurfaceEmitter.h"
#include "ParticleMeshVertexEmitter.h"
namespace ma
{

    //-----------------------------------------------------------------------
	void Triangle::calculateSquareSurface (void)
	{
		/* Calculating the surface of a triangle with the following algorithm:
		   v1 = Vector3(x1, y1, z1)
		   v2 = Vector3(x2, y2, z2)
		   v3 = Vector3(x3, y3, z3)
		   a = sqrt ((x1-x3)*(x1-x3) + (y1-y3)*(y1-y3) + (z1-z3)*(z1-z3))
		   b = sqrt ((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1))
		   c = sqrt ((x3-x2)*(x3-x2) + (y3-y2)*(y3-y2) + (z3-z2)*(z3-z2))
		   p = 0.5 * (a + b + c)
		   surface = sqrt (p * (p-a) * (p-b) * (p-c));
		*/

		// a, b and c are the length of each triangle
		Real a = sqrt (	(v1.x - v3.x) * (v1.x - v3.x) + 
							(v1.y - v3.y) * (v1.y - v3.y) + 
							(v1.z - v3.z) * (v1.z - v3.z));
		Real b = sqrt (	(v2.x - v1.x) * (v2.x - v1.x) + 
							(v2.y - v1.y) * (v2.y - v1.y) + 
							(v2.z - v1.z) * (v2.z - v1.z));
		Real c = sqrt (	(v3.x - v2.x) * (v3.x - v2.x) + 
							(v3.y - v2.y) * (v3.y - v2.y) + 
							(v3.z - v2.z) * (v3.z - v2.z));
		Real p = 0.5f * (a + b + c);

		// Assign square surface of the triangle
		squareSurface = p * (p-a) * (p-b) * (p-c);
	}
	//-----------------------------------------------------------------------
	void Triangle::calculateSurfaceNormal (void)
	{
		/* Calculating the surface normal of a triangle with the following algorithm:
		   v1 = Vector3(x1, y1, z1)
		   v2 = Vector3(x2, y2, z2)
		   v3 = Vector3(x3, y3, z3)
		   n = (v2-v1)x(v3-v1), where the 'x' is the cross product
		*/
		surfaceNormal = (v2-v1).crossProduct(v3-v1);
		surfaceNormal.normalise();
	}
	//-----------------------------------------------------------------------
	void Triangle::calculateEdgeNormals (void)
	{
		en1 = v1.crossProduct(v2);
		en1.normalise();
		en2 = v2.crossProduct(v3);
		en2.normalise();
		en3 = v3.crossProduct(v1);
		en3.normalise();
	}
	//-----------------------------------------------------------------------
	const Vector3 Triangle::getRandomTrianglePosition (void)
	{
		// Use barycentric coordinates. Let A, B, C be the three vertices of the triangle. Any point P inside can 
		// be expressed uniquely as P = aA + bB + cC, where a+b+c=1 and a,b,c are each >= 0.
		// Knowing a and b permits you to calculate c=1-a-b.
		// So if you can generate two random numbers a and b, each in [0,1], such that their sum <=1, you've got a 
		// random point in your triangle. Generate random a and b independently and uniformly in [0,1] 
		// If a+b>1, replace a by 1-a, b by 1-b. Let c=1-a-b. Then aA + bB + cC is uniformly distributed 
		// in triangle ABC: the reflection step a=1-a; b=1-b gives a point (a,b) uniformly distributed in the 
		// triangle (0,0)(1,0)(0,1), which is then mapped affinely to ABC. Now you have barycentric coordinates 
		// a,b,c. Compute your point P = aA + bB + cC.
		Real a = Math::RangeRandom(0, 1);
		Real b = Math::RangeRandom(0, 1);
		if (a + b > 1)
		{
			a = 1 - a;
			b = 1 - b;
		}
		Real c = 1 - a - b;
		return a * v1 + b * v2 + c * v3;
	}
	//-----------------------------------------------------------------------
	const Triangle::PositionAndNormal Triangle::getRandomEdgePositionAndNormal (void)
	{
		Real mult = Math::RangeRandom(0, 1);
		Real randomVal = Math::RangeRandom(0, 3);
		PositionAndNormal pAndN;
		pAndN.position = Vector3::ZERO;
		pAndN.normal = Vector3::ZERO;
		if (randomVal < 1)
		{
			pAndN.position = Vector3(	v2.x + mult*(v1.x - v2.x),
											v2.y + mult*(v1.y - v2.y),
											v2.z + mult*(v1.z - v2.z));
			pAndN.normal = en1;
		}
		else
		{
			if (randomVal < 2)
			{
				pAndN.position = Vector3(	v3.x + mult*(v2.x - v3.x),
												v3.y + mult*(v2.y - v3.y),
												v3.z + mult*(v2.z - v3.z));
				pAndN.normal = en2;
			}
			else
			{
	
				pAndN.position = Vector3(	v1.x + mult*(v3.x - v1.x),
												v1.y + mult*(v3.y - v1.y),
												v1.z + mult*(v3.z - v1.z));
				pAndN.normal = en3;
			}
		}

		return pAndN;
	}
	//-----------------------------------------------------------------------
	const Triangle::PositionAndNormal Triangle::getRandomVertexAndNormal (void)
	{
		Real randomVal = Math::RangeRandom(0, 3);
		PositionAndNormal pAndN;
		pAndN.position = Vector3::ZERO;
		pAndN.normal = Vector3::ZERO;

		if (randomVal < 1)
		{
			pAndN.position = v1;
			pAndN.normal = vn1;
		}
		else
		{
			if (randomVal < 2)
			{
				pAndN.position = v2;
				pAndN.normal = vn2;
			}
			else
			{
				pAndN.position = v3;
				pAndN.normal = vn3;
			}
		}

		return pAndN;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	MeshInfo::MeshInfo (MESHEMITTER_VERTEX* rgVertices, uint32 nNumVertices, uint16* rgIndices, uint32 nNumIndices, 
		MeshSurfaceDistribution distribution) : 
		mDistribution(distribution)
	{
	    getMeshInformation(rgVertices, nNumVertices, rgIndices, nNumIndices);
	}
	//-----------------------------------------------------------------------
	MeshInfo::~MeshInfo (void)
	{
		_triangles.clear();
	}
	//-----------------------------------------------------------------------
	Real MeshInfo::getGaussianRandom (Real high, Real cutoff)
	{
		Real x1 = 0;
		Real x2 = 0;
		Real w = 0;
		Real y1 = 0;

		uint32 max = 0;
		do
		{
			x1 = Math::RangeRandom(0,1);
			x2 = Math::RangeRandom(0,1);
			w = x1 * x1 + x2 * x2;

			// Prevent infinite loop
			if (w >= 1.0 && max > 4 )
				w = x1;
		
		} while (w >= 1.0);
	
		w = sqrt((-2.0f * log(w)) / w);
        y1 = Math::Abs(x1 * w);
		y1 = y1 > cutoff ? cutoff : y1;
		y1 *= high / cutoff;
		return y1;
	}

	//-----------------------------------------------------------------------
	const Triangle& MeshInfo::getTriangle (size_t triangleIndex)
	{
		return _triangles[triangleIndex];
	}

	//-----------------------------------------------------------------------
	const size_t MeshInfo::getRandomTriangleIndex (void)
	{
		size_t index;
		if (mDistribution == MSD_HOMOGENEOUS || mDistribution == MSD_HETEROGENEOUS_1)
		{
			index = (size_t)getGaussianRandom((float)_triangles.size() - 0.01f);
		}
		else
			index = (size_t)Math::RangeRandom(0, (float)_triangles.size() - 0.01f);

		return index;
	}

	//-----------------------------------------------------------------------
	const Triangle::PositionAndNormal MeshInfo::getRandomPositionAndNormal (const size_t triangleIndex)
	{
		Triangle triangle = getTriangle(triangleIndex);
		Triangle::PositionAndNormal pAndN;
		pAndN.position = Vector3::ZERO;
		pAndN.normal = Vector3::ZERO;
		if (mDistribution == MSD_HOMOGENEOUS || mDistribution == MSD_HETEROGENEOUS_1 || mDistribution == MSD_HETEROGENEOUS_2)
		{
			pAndN.position = triangle.getRandomTrianglePosition();
			pAndN.normal = triangle.surfaceNormal;
		}
		else
		{
			if (mDistribution == MSD_VERTEX)
			{
				pAndN = triangle.getRandomVertexAndNormal();
			}
			else
			{
				if (mDistribution == MSD_EDGE)
				{
					pAndN = triangle.getRandomEdgePositionAndNormal();
				}
			}
		}
		return pAndN;
	}

	//-----------------------------------------------------------------------
	bool MeshInfo::getMeshInformation(MESHEMITTER_VERTEX* rgVertices, int nNumVertices, uint16* rgIndices, int nNumIndices)
	{
		// Create triangles from the retrieved data
		for (int k = 0; k < nNumIndices-1; k+=3)
		{
			Triangle t;
			t.v1 = rgVertices[rgIndices[k]].pos;
			t.vn1 = rgVertices[rgIndices[k]].normal;

			t.v2 = rgVertices[rgIndices[k+1]].pos;
			t.vn2 = rgVertices[rgIndices[k+1]].normal;

			t.v3 = rgVertices[rgIndices[k+2]].pos;
			t.vn3 = rgVertices[rgIndices[k+2]].normal;

			t.calculateSquareSurface();
			t.calculateSurfaceNormal();
			t.calculateEdgeNormals();
			_triangles.push_back(t);
		}

		// Sort the triangle on their size, if needed (only if a gaussian random number generator
		// function is used to perform a random lookup of a triangle)
		if (mDistribution == MSD_HOMOGENEOUS)
			sort(_triangles.begin(), _triangles.end(), SortDescending());
		else
			if (mDistribution == MSD_HETEROGENEOUS_1)
				sort(_triangles.begin(), _triangles.end(), SortAscending());

        return true;
	}

    void MeshInfo::setDistribution( MeshSurfaceDistribution eType )
    {
        if (eType == mDistribution)
        {
            return;
        }

        mDistribution = eType;
        switch(eType)
        {
        case MSD_HOMOGENEOUS:
            sort(_triangles.begin(), _triangles.end(), SortDescending());
            break;
        case MSD_HETEROGENEOUS_1:
            sort(_triangles.begin(), _triangles.end(), SortAscending());
            break;
        default:
            break;
        }
    }

    //-----------------------------------------------------------------------
//-----------------------------------------------------------------------

CParticleMeshSurfaceEmitter::CParticleMeshSurfaceEmitter(void)
{
    mScale = Vector3::UNIT_SCALE;
    mRotate = Quaternion::IDENTITY;
    mMeshInfo = NULL;
    mUseNormalDirection = false;
    mDistribution = MSD_HETEROGENEOUS_2;
}

CParticleMeshSurfaceEmitter::~CParticleMeshSurfaceEmitter(void)
{
    SAFE_DELETE(mMeshInfo);
}

int CParticleMeshSurfaceEmitter::Emitt( Real now, Real timeSinceLastCall, SParticle*& outArray, uint32 nFinalMaxParticles )
{
    if (!m_bEnabled)
        return 0;

    if(mMeshInfo == NULL)
        return 0;

    mParticles.clear();
    uint32 amount = this->GenConstantEmissionCount(now,timeSinceLastCall, nFinalMaxParticles);

    SParticle p;
    for(uint32 i=0; i<amount; ++i)
    {
        int nTriangleIndex = mMeshInfo->getRandomTriangleIndex();
        Triangle::PositionAndNormal pAndN = mMeshInfo->getRandomPositionAndNormal(nTriangleIndex);


        p.prePos = p.startPos = p.pos = mRotate*(pAndN.position*mScale)+mPos;
        p.startTime = now-timeSinceLastCall;
        GenEmission(p);

        if( mUseNormalDirection )
            p.vector = (mRotate*pAndN.normal)*(mMinVelocity + Randomizer::frand()*(mMaxVelocity-mMinVelocity));
        else
            p.vector = mDirection*(mMinVelocity + Randomizer::frand()*(mMaxVelocity-mMinVelocity));
        {
            Vector3 tgt = p.vector;
            if (mMaxAngleDegrees.z)
                tgt.rotateXYBy(Radian(Degree((Real)(Randomizer::rand()%(mMaxAngleDegrees.z*2) - mMaxAngleDegrees.z))));

            if(mMaxAngleDegrees.x)
                tgt.rotateYZBy(Radian(Degree((Real)(Randomizer::rand()%(mMaxAngleDegrees.x*2) - mMaxAngleDegrees.x))));

            if(mMaxAngleDegrees.y)
                tgt.rotateXZBy(Radian(Degree((Real)(Randomizer::rand()%(mMaxAngleDegrees.y*2) - mMaxAngleDegrees.y))));

            p.vector = tgt;
        }
        p.startVector = p.vector;

        mParticles.push_back(p);
    }

    if (mParticles.empty())
    {
        outArray = NULL;
    }
    else
    {
        outArray = &mParticles[0];
    }

    return mParticles.size();
}

void CParticleMeshSurfaceEmitter::SetPos( const Vector3& pos )
{
    CParticleEmitter::SetPos(pos);
}

void CParticleMeshSurfaceEmitter::SetDirection( const Vector3& newDirection )
{
    CParticleEmitter::SetDirection(newDirection);
    mRotate = this->CalcRotate();
}

void CParticleMeshSurfaceEmitter::SetScale( const Vector3& vScale )
{
    mScale = vScale;
}

Quaternion CParticleMeshSurfaceEmitter::CalcRotate() const
{
    Vector3 mUp = mDirection.perpendicular();
    Vector3 mLeft = mUp.crossProduct(mDirection);
    return Quaternion(mLeft.normalisedCopy(), mUp.normalisedCopy(), mDirection.normalisedCopy());
}

void CParticleMeshSurfaceEmitter::SetFile( const char* pszMesh )
{
    if (m_strFile == pszMesh)
    {
        return;
    }

    m_strFile = pszMesh;
    
}

void CParticleMeshSurfaceEmitter::SetDistribution( MeshSurfaceDistribution eDistribution )
{
    mDistribution = eDistribution;
    if (mMeshInfo)
    {
        mMeshInfo->setDistribution(eDistribution);
    }
}

void CParticleMeshSurfaceEmitter::SetMesh( MESHEMITTER_VERTEX* rgVertices, uint32 nNumVertices, uint16* rgIndices, uint32 nNumIndices )
{
    m_vecVertices.clear();
    m_vecIndices.clear();

    m_vecVertices.resize(nNumVertices);
    m_vecIndices.resize(nNumIndices);
    for (uint32 i = 0;i< nNumVertices;++i)
    {
        m_vecVertices[i] = rgVertices[i];
    }
    for (uint32 i = 0;i< nNumIndices;++i)
    {
        m_vecIndices[i] = rgIndices[i];
    }

    SAFE_DELETE(mMeshInfo);
    mMeshInfo = new MeshInfo(rgVertices, nNumVertices, rgIndices, nNumIndices, mDistribution);
}

RefPtr<CParticleMeshSurfaceEmitter> CreateMeshSurfaceEmitter()
{
    return new CParticleMeshSurfaceEmitter();
}

}