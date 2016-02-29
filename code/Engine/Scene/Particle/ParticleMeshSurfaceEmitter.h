#pragma once
#include "ParticleEmitter.h"

namespace ma
{

/** Definition of a Triangle
*/
class Triangle
{
	public:
		/** The struct is used to return both the position and the normal
	    */
		struct PositionAndNormal
		{
			Vector3 position;
			Vector3 normal;
		};

		/** Public attributes **/
		Real squareSurface;
		Vector3 surfaceNormal;	// Normal of triangle v1-v2-v3
		Vector3 v1;				// Vertex v1
		Vector3 v2;				// Vertex v2
		Vector3 v3;				// Vertex v3
		Vector3 vn1;			// Normal of vertex v1
		Vector3 vn2;			// Normal of vertex v2
		Vector3 vn3;			// Normal of vertex v3
		Vector3 en1;			// Normal of edge v1-v2
		Vector3 en2;			// Normal of edge v2-v3
		Vector3 en3;			// Normal of edge v3-v1

		/** Constructor **/
		Triangle(void){};

		/** Calculate the (square) surface of the triangle **/
		void calculateSquareSurface (void);

		/** Calculate the surface normal of the triangle **/
		void calculateSurfaceNormal (void);

		/** Calculate the edge normals of the 3 edges  **/
		void calculateEdgeNormals (void);

		/** Determine a random position on this triangle **/
		const Vector3 getRandomTrianglePosition (void);

		/** Determine a random position including its normal on a one of the edges **/
		const PositionAndNormal getRandomEdgePositionAndNormal (void);

		/** Determine a random vertex including its normal of this triangle **/
		const PositionAndNormal getRandomVertexAndNormal (void);
};

/** Comparer used for sorting vector in ascending order
*/
struct SortAscending
{
	bool operator() (const Triangle& a, const Triangle& b)
	{
		return a.squareSurface < b.squareSurface;
	}
};

/** Comparer used for sorting vector in descending order
*/
struct SortDescending
{
	bool operator() (const Triangle& a, const Triangle& b)
	{
		return a.squareSurface > b.squareSurface;
	}
};

/** Define a template class for a vector of triangles.
*/
typedef std::vector<Triangle> Triangles;

/** Class that constructs mesh information of a given mesh name
@remarks
*/
class MeshInfo
{
	public:
		/** Constructor **/
		MeshInfo (MESHEMITTER_VERTEX* rgVertices, uint32 nNumVertices, uint16* rgIndices, uint32 nNumIndices, 
			const MeshSurfaceDistribution distribution = MSD_HOMOGENEOUS);

		/** Destructor **/
		~MeshInfo (void);

		/** Generate a random number. The ´high?argument determines that numbers are 
			returned between [0..high] **/
		Real getGaussianRandom (Real high, Real cutoff = 4);

	    /** Retrieve vertex info **/
		bool getMeshInformation(MESHEMITTER_VERTEX* rgVertices, int nNumVertices, uint16* rgIndices, int nNumIndices);

		/** Get a triangle based on the index. */
		const Triangle& getTriangle (size_t triangleIndex);

			/** Get a random triangle (index) from the mesh. */
		const size_t getRandomTriangleIndex (void);

		/** Returns both a random point on a given triangle and its normal vector.
			How the random point and the normal are determined depends on the distribution type.
		**/
		const Triangle::PositionAndNormal getRandomPositionAndNormal (const size_t triangleIndex);

        void setDistribution(MeshSurfaceDistribution eDistribution);
        MeshSurfaceDistribution getDistribution() const{return mDistribution;}
	protected:
		Triangles _triangles;
		MeshSurfaceDistribution mDistribution;
};

/** The MeshSurfaceEmitter is a ParticleEmitter that emits particles on the surface of a mesh.
	@remarks
		There are several ways of emitting it on the surface, from the vertices, edges and faces of a mesh.
		It is also possible to define whether more particles emit on larger faces.
    */
class CParticleMeshSurfaceEmitter : public CParticleEmitter
{
public:
    CParticleMeshSurfaceEmitter(void);
    ~CParticleMeshSurfaceEmitter(void);

    // ---------------------------------------------------------------------
    // Inherite from CParticleEmitter
    // ---------------------------------------------------------------------

    //! Prepares an array with new particles to emitt into the system
    //! and returns how much new particles there are.
    virtual int Emitt(Real now, Real timeSinceLastCall, SParticle*& outArray, uint32 nFinalMaxParticles);

    //! Get emitter type
    virtual E_PARTICLE_EMITTER_TYPE GetType() const { return EPET_MESH_SURFACE; }

    virtual void SetPos( const Vector3& pos );
    virtual void SetDirection( const Vector3& newDirection );
    // ---------------------------------------------------------------------
    // Self
    // ---------------------------------------------------------------------
public:
    void SetScale(const Vector3& vScale);
    const Vector3& GetScale() const{return mScale;}

    virtual void SetUseNormalDirection( bool useNormalDirection = true ){mUseNormalDirection = useNormalDirection;}
    virtual bool GetUsingNormalDirection() const{return mUseNormalDirection;}

    void SetFile(const char* pszMesh);
    const char* GetFile() const{return m_strFile.c_str();}

    void SetMesh(MESHEMITTER_VERTEX* rgVertices, uint32 nNumVertices, uint16* rgIndices, uint32 nNumIndices);

    void SetDistribution(MeshSurfaceDistribution eDistribution);
    MeshSurfaceDistribution GetDistribution() const{return mDistribution;}

    const vector<MESHEMITTER_VERTEX>& GetVertices() const{return m_vecVertices;}
    const vector<uint16>& GetIndices() const{return m_vecIndices;}
private:
    Quaternion CalcRotate() const;
private:
    string m_strFile;
    MeshInfo* mMeshInfo;
    Vector3 mScale;
    Quaternion mRotate;
    bool mUseNormalDirection;
    MeshSurfaceDistribution mDistribution;

    vector<MESHEMITTER_VERTEX> m_vecVertices;
    vector<uint16> m_vecIndices;
};

RefPtr<CParticleMeshSurfaceEmitter> CreateMeshSurfaceEmitter();
}