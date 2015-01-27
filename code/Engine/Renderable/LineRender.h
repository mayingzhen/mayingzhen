#ifndef  _LineRender__H__
#define  _LineRender__H__

namespace ma
{

	class LineRender
	{

	public:
		LineRender();

		void Init();

		void ShutDown();

		void OnFlushFrame();

		void Render();

		void DrawLine(const Vector3& p0,const Vector3& p1,ColourValue dwColor);

		void DrawBox(const Matrix4& wordMat,const Vector3& boxSize, ColourValue color);

		void DrawCapsule(const Matrix4& wordMat,float fRadius, float Height, ColourValue color);

		void DrawCircle(UINT nbSegments, const Matrix4& world, ColourValue dwColor, float radius, bool semicircle = false);

		void DrawWireSphere(const Matrix4& wordMat,float fRadius, ColourValue color);

		void DrawTransform(const Transform& tsf, float fCenterScale = 1.0f, float fAxisScale = 1.0f);

	private:
		RefPtr<MeshBatch>	m_pMeshBatch;

		RefPtr<Technique>	m_pLinTech;

		struct LineVertex
		{
			Vector3			pos;
			UINT			col;
		};

		std::vector<LineVertex> m_arrLineVertex[2];

	};

	 LineRender*	GetLineRender();

	 void			SetLineRender(LineRender* pLineRender);
}

#endif
