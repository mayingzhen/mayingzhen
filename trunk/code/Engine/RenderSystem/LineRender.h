#ifndef  _LineRender__H__
#define  _LineRender__H__

namespace ma
{
	class ENGINE_API LineRender
	{
	public:
		static void Init();

		static void ShutDown();

		static void	Begin();

		//static void EndFrame();

		static void Flush();

		static void DrawLine(const Vector3& p0,const Vector3& p1,Color dwColor);

		static void DrawBox(const Matrix4x4& wordMat,const Vector3& boxSize, Color color);

		static void DrawCapsule(const Matrix4x4& wordMat,float fRadius, float Height, Color color);

		static void DrawCircle(UINT nbSegments, const Matrix4x4& world, Color dwColor, float radius, bool semicircle = false);

		static void DrawWireSphere(const Matrix4x4& wordMat,float fRadius, Color color);
	};
}

#endif
