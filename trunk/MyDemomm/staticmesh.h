#ifndef C_STATIC_MESH_HH__
#define C_STATIC_MESH_HH__

#include <d3dx9.h>
#include <d3dx9mesh.h>
#include <d3d9.h>
#include <vector>

class CRay;

class CStaticMesh
{
public:
	CStaticMesh(const D3DXVECTOR3 &pos = D3DXVECTOR3(0,0,0),
				float scale = 1,float angle = 0);
	~CStaticMesh();
		
	void LoadXFile(const char *fileName);
	void Render();

	void ComputeTransMatr(const D3DXVECTOR3 &pos,float scale,float angle); //计算世界变换矩阵
	D3DXMATRIX GetWordTransMatr() const; //获得世界变换数据
	void GetBoundingBox(D3DXVECTOR3 &max,D3DXVECTOR3 &min) const; //获得物体包围球数据
	LPD3DXMESH GetObjectMesh() const; 
	bool CollisionTest(const D3DXVECTOR3 &pos);  //碰撞检测
	BOOL RaySphereIntTest(CRay &ray);
private:

	bool ComputeBoundBox();  //计算物体包围盒

private:
	LPD3DXMESH  m_mesh;
	std::vector<D3DMATERIAL9> m_mtrls;
	std::vector<IDirect3DTexture9*> m_textures;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9  m_pIB;

	D3DXVECTOR3 m_pos;
	float		m_scale;
	float       m_angle;

	D3DXVECTOR3 m_max; //世界坐标
	D3DXVECTOR3 m_min; //世界坐标

	D3DXMATRIX m_trans;
};


#endif