#ifndef  _Technique__H__
#define  _Technique__H__

namespace ma
{

	enum EShaderFlag
	{
		ShderFLag_SKIN = 1 << 1,
	};

	class ENGINE_API Technique
	{
	public:
		virtual void Load(const char* pFileName, const char* pTechName) = 0;

		virtual void Begin(UINT uShaderFlg = 0) = 0;

		virtual void CommitChanges() = 0;

		virtual void End() = 0;

		virtual void SetMatrixArray(const char* pParameter,Matrix4x4* pMatrix, UINT Count) = 0;

		virtual void SetMatrix(const char* pParameter,Matrix4x4* pMatrix) = 0;

		virtual void SetVector(const char* pParameter, Vector4* pVector) = 0;

		virtual void SetTexture(const char* pParameter, Texture* pTexture) = 0;
	};
}



#endif