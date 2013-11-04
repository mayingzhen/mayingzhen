#ifndef  _LineRender__H__
#define  _LineRender__H__

namespace ma
{

	class RENDER_API LineRender
	{

	public:
		void Init();

		void ShutDown();

		void OnFlushFrame();

		void Render();

		void DrawLine(const Vector3& p0,const Vector3& p1,Color dwColor);

		void DrawBox(const Matrix4x4& wordMat,const Vector3& boxSize, Color color);

		void DrawCapsule(const Matrix4x4& wordMat,float fRadius, float Height, Color color);

		void DrawCircle(UINT nbSegments, const Matrix4x4& world, Color dwColor, float radius, bool semicircle = false);

		void DrawWireSphere(const Matrix4x4& wordMat,float fRadius, Color color);

	private:
		MeshBatch*	m_pMeshBatch;

		Technique*	m_pLinTech;

		struct LineVertex
		{
			Vector3 pos;
			Color   col;
		};

		std::vector<LineVertex> m_arrLineVertex[2];

	};

	RENDER_API LineRender*	GetLineRender();

	RENDER_API void			SetLineRender(LineRender* pLineRender);
}

#endif
