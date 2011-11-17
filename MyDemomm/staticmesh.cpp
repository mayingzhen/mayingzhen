#include "staticmesh.h"
#include "Terrain.h"
#include "public.h"
#include "GameApp.h"


CStaticMesh::CStaticMesh(const D3DXVECTOR3 &pos,float scale,float angle)
{
	m_pos = pos;
	m_scale = scale;
	m_angle = angle;

	m_mesh = NULL;

	ComputeTransMatr(m_pos,m_scale,m_angle);
}

CStaticMesh::~CStaticMesh()
{
	SAFE_RELEASE(m_mesh);

	m_mtrls.clear();
	m_textures.clear();
}

void CStaticMesh::LoadXFile(const char *fileName)
{
	//
	// Load the XFile data.
	//
	HRESULT      hr=0;
	ID3DXBuffer* adjBuffer  = NULL;
	ID3DXBuffer* mtrlBuffer = NULL;
	DWORD        numMtrls   = 0;

	hr = D3DXLoadMeshFromX(fileName,D3DXMESH_MANAGED,g_pD3DDevice,
				&adjBuffer,&mtrlBuffer,0,&numMtrls,&m_mesh);

	if(FAILED(hr))
	{
		::MessageBox(0, "D3DXLoadMeshFromX() - FAILED", 0, 0);
	}

	if(!(m_mesh->GetFVF() & D3DFVF_NORMAL))//如果X文件没有顶点法线数据，计算
	{
		ID3DXMesh* pMesh=0;
		m_mesh->CloneMeshFVF(D3DXMESH_MANAGED,m_mesh->GetFVF() | D3DFVF_NORMAL,
				g_pD3DDevice,&pMesh );

		// compute the normals:
		D3DXComputeNormals( pMesh, 0 );

		m_mesh->Release();  // get rid of the old mesh
		m_mesh = pMesh; // save the new mesh with normals
	}

	//
	// Extract the materials, and load textures.
	//

	if( mtrlBuffer != NULL && numMtrls != 0 )
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		m_mtrls.reserve(numMtrls); //提前分配空间
		m_textures.reserve(numMtrls); //提前分配空间
		
		for(int i = 0; i < (int)numMtrls; i++)
		{
			// the MatD3D property doesn't have an ambient value set
			// when its loaded, so set it now:
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			// save the ith material
			m_mtrls.push_back( mtrls[i].MatD3D );

			// check if the ith material has an associative texture
			if( mtrls[i].pTextureFilename != 0 )
			{
				// yes, load the texture for the ith subset
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(g_pD3DDevice, mtrls[i].pTextureFilename, &tex);
				
				// save the loaded texture
				m_textures.push_back( tex );
			}
			else
			{
				// no texture for the ith subset
				m_textures.push_back( 0 );
			}
		}
	}
	mtrlBuffer->Release(); // done w/ buffer

	//
	// Optimize the mesh.
	//
	hr = m_mesh->OptimizeInplace(		
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT  |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjBuffer->GetBufferPointer(),
		0, 0, 0);
	adjBuffer->Release(); // done w/ buffer

	if(FAILED(hr))
	{
		::MessageBox(0, "OptimizeInplace() - FAILED", 0, 0);
	}

	m_mesh->GetVertexBuffer(&m_pVB);
	m_mesh->GetIndexBuffer(&m_pIB);

	ComputeBoundBox();//计算包围体
}


void CStaticMesh::ComputeTransMatr(const D3DXVECTOR3 &pos,float scale,float angle) //计算变换矩阵
{
	D3DXMATRIX matRy,matSc,matTr;

	D3DXMatrixIdentity(&matRy);
	D3DXMatrixIdentity(&matSc);
	D3DXMatrixIdentity(&matTr);

	D3DXMatrixRotationY(&matRy,angle);
	D3DXMatrixScaling(&matSc,scale,scale,scale);
	D3DXMatrixTranslation(&matTr,pos.x,pos.y,pos.z);

	m_trans = matRy * matSc * matTr;
}

D3DXMATRIX CStaticMesh::GetWordTransMatr() const
{
	return m_trans;
}


void CStaticMesh::GetBoundingBox(D3DXVECTOR3 &max,D3DXVECTOR3 &min) const
{
	max = m_max;
	min = m_min;
}

void CStaticMesh::Render()
{

	g_pD3DDevice->SetTransform(D3DTS_WORLD,&m_trans);

	//
	// Set texture filters.
	//
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	for(int i = 0; i < (int)m_mtrls.size(); i++)
	{
		g_pD3DDevice->SetMaterial( &m_mtrls[i] );
		g_pD3DDevice->SetTexture(0, m_textures[i]);
		m_mesh->DrawSubset(i);
	}
	
}

LPD3DXMESH CStaticMesh::GetObjectMesh() const
{
	return m_mesh;
}

bool CStaticMesh::ComputeBoundBox()
{
	BYTE *v = 0;
	m_mesh->LockVertexBuffer(0,(void**)&v);

	HRESULT hr = D3DXComputeBoundingBox(
		(D3DXVECTOR3*)v,
		m_mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(m_mesh->GetFVF()),
		&m_min,&m_max);

	m_mesh->UnlockAttributeBuffer();

	if( FAILED(hr) )
		return false;

	return true;
}

BOOL CStaticMesh::RaySphereIntTest(CRay &ray)
{
	ray.TransformToMeshWord(m_trans);	//将射线转到物体坐标空间 

	BOOL res = FALSE;
	D3DXIntersect(m_mesh,&(ray.m_origin),&(ray.m_direction),
		&res,NULL,NULL,NULL,NULL,NULL,NULL);
	return res;
}

bool CStaticMesh::CollisionTest(const D3DXVECTOR3 &pos)  //碰撞检测
{
	D3DXVECTOR3 max,min;
	D3DXVec3TransformCoord(&max,&m_max,&m_trans); //将坐标转换到世界空间下
	D3DXVec3TransformCoord(&min,&m_min,&m_trans);

	return ( pos.x > min.x && pos.x < max.x &&
			   pos.z > min.z && pos.z < max.z );


}





