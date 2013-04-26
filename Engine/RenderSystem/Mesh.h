#ifndef  _Mesh__H__
#define  _Mesh__H__

#include "Renderer.h"
#include "Material.h"

//#include "MWResource.h"
//#include "MWColMesh.h"

namespace ma
{

/* :) SubMesh
-----------------------------------------------------
-----------------------------------------------------
*/
class ENGINE_API SubMesh 
{
    //friend class MeshManager;

public:
    SubMesh();
    ~SubMesh();

    //void                    SetPrimitiveCount(int iPrimCount);
    //void                    SetPrimitiveType(PRIMITIVE_TYPE pt);

    //VertexStream *          GetVertexStream();
    //IndexStream *           GetIndexStream();
    //int                     GetPrimitiveCount();
    //PRIMITIVE_TYPE          GetPrimitiveType();

	Material *				GetMaterial();

protected:

//     int                     mPrimCount;
//     PRIMITIVE_TYPE          mPrimType;
// 	Material				mMaterial;

	UINT					m_nIndexStart;
	UINT					m_nIndexCount;
	UINT					m_nVertexStart;
	UINT					m_nVertexCount;

	Material				m_nMateiralID;

	//Bounding				m_subMeshBound;
	std::vector<BoneIndex>	m_arrBonePalette;
};


/* class: Mesh
----------------------------------------------------
@Remark:
    Triangle Mesh.
----------------------------------------------------
*/
class ENGINE_API Mesh : public Resource/*, public RefCountObject*/
{
    //DeclareRefObj();

    //friend class MeshManager;

public:
    Mesh();
    ~Mesh();

    //const TString128 &          GetName() const;

    void                        SetAabb(const Vec3 & vMin, const Vec3 & vMax);
    void                        SetAabb(const Aabb & bound);
    //void                        SetBoundingSphere(const Vec3 & vCenter, float fRadius);
    //void                        SetBoundingSphere(const Sphere & sphere);
    const AABB &                GetAabb() const;
    //const Sphere &              GetBoundingSphere() const;

    SubMesh *                   CreateSubMesh();

    SubMesh *                   GetSubMesh(int index);
    int                         GetSubMeshCount();
    void                        RemoveSubMesh(int index);
    void                        RemoveAllSubMehs();

	//Skeleton *					GetSkeleton();

	//ColMesh *					GetColMesh();
	//void						GenColMeshFromRenderMesh();

    void                        CalcuBounds();

    virtual void                Load();
    virtual void                Reload();
    virtual void                Unload();
    //virtual void                LoadImp(DataStreamPtr stream);
	virtual void				Serialize(SerializeListener& sl,const char* pszLabel = "SubMeshData");

protected:
    //TString128					mName;
    AABB						mBound;
    //Sphere						mSphere;
	std::vector<SubMesh*>       mMeshes;
	
	//ColMesh					mColMesh;

	VertexStream				mVertexStream;
	IndexStream					mIndexStream;
};

DeclareSmartPtr(Mesh);

}