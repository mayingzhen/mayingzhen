#ifndef  _LineRender__H__
#define  _LineRender__H__

namespace ma
{
	class LineRender
	{
	public:
		LineRender();

		void Init();

		void DrawLine(const Vector3& p0,const Vector3& p1,Color dwColor);

		void DrawBox(const Matrix4x4& wordMat,const Vector3& boxSize, Color color);

		void DrawCircle(UINT nbSegments, const Matrix4x4& world, Color dwColor, float radius, bool semicircle = false);

		void DrawWireSphere(const Matrix4x4& wordMat,float fRadius, Color color);

		void Start();

		void Finish();

		Matrix4x4& GetViewProjMatrix() {return m_matViewPorj;}

	private:
		MeshBatch*	m_pMeshBatch;
		
		Matrix4x4 	m_matViewPorj;			
	};
}

#endif
