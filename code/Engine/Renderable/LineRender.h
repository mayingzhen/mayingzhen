#ifndef  _LineRender__H__
#define  _LineRender__H__

namespace ma
{
	class LineRender
	{
	public:
		static void Init();

		static void ShutDown();

		static void OnFlushFrame();

		static void Render();

		static void DrawLine(const Vector3& p0,const Vector3& p1,ColourValue dwColor);

		static void DrawBox(const Matrix4& wordMat,const Vector3& boxSize, ColourValue color);

		static void DrawCapsule(const Matrix4& wordMat,float fRadius, float Height, ColourValue color);

		static void DrawCircle(UINT nbSegments, const Matrix4& world, ColourValue dwColor, float radius, bool semicircle = false);

		static void DrawWireSphere(const Matrix4& wordMat,float fRadius, ColourValue color);

		static void DrawTransform(const Transform& tsf, float fCenterScale = 1.0f, float fAxisScale = 1.0f);

	};

}

#endif
