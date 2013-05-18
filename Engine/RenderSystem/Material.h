#ifndef  _Material__H__
#define  _Material__H__


#include "Texture.h"


namespace ma
{

	class ENGINE_API Material
	{
	public:
		static const int MAX_EX_TEXTURES = 8;
		static const int MAX_TECHNIQUES = 8; 

	public:
		Material();
		~Material();

		void SetEmissive(const COLOR & color);
		void SetAmbient(const COLOR & color);
		void SetDiffuse(const COLOR & color);
		void SetSpecular(const COLOR & color);
		void SetSpecularPower(float power);

		void SetEmissiveMap(const char* tex);
		void SetDiffuseMap(const char* tex);
		void SetSpecularMap(const char* tex);
		void SetNormalMap(const char* tex);

		void SetEmissiveMap(Texture* tex);
		void SetDiffuseMap(Texture* tex);
		void SetSpecularMap(Texture* tex);
		void SetNormalMap(Texture* tex);

		const COLOR & GetEmissive() const;
		const COLOR & GetAmbient() const;
		const COLOR & GetDiffuse() const;
		const COLOR & GetSpecular() const;
		float GetSpecularPower() const;

		Texture* GetEmissiveMap() const;
		Texture* GetDiffuseMap() const;
		Texture* GetSpecularMap() const;
		Texture* GetNormalMap() const;


		//cull mode
		void SetCullMode(CULL_MODE mode)
		{ 
			mRenderState.cullMode = mode;
		}

		CULL_MODE GetCullMode() const
		{ 
			return mRenderState.cullMode;
		}

		//fill mode
		void SetFillMode(FILL_MODE mode)
		{
			mRenderState.fillMode = mode;
		}

		FILL_MODE GetFillMode() const
		{
			return mRenderState.fillMode;
		}

		// blend
		void SetBlendMode(BLEND_MODE mode)
		{
			mRenderState.blendMode = mode;
		}

		BLEND_MODE GetBlendMode() const
		{
			return mRenderState.blendMode;
		}

		//color write
		void SetColorWrite(int flag)
		{
			mRenderState.colorWrite = flag;
		}

		int GetColorWrite() const
		{
			return mRenderState.colorWrite;
		}

		//depth write
		void SetDepthWrite(bool enabled)
		{
			mRenderState.depthWrite = enabled;
		}

		bool GetDepthWrite() const
		{
			return mRenderState.depthWrite;
		}

		//depth check
		void SetDepthCheck(DEPTH_CHECK_MODE m)
		{
			mRenderState.depthCheck = m;
		}

		DEPTH_CHECK_MODE GetDepthCheck() const
		{
			return mRenderState.depthCheck;
		}

		//alpha test reference
		void SetAlphaTestRef(float a)
		{
			mRenderState.alphaTestRef = a;
		}

		float GetAlphaTestRef() const
		{
			return mRenderState.alphaTestRef;
		}

		const RenderState & GetRenderState() const
		{
			return mRenderState;
		}

		bool IsTransparency() const
		{
			return mRenderState.blendMode >= BM_ALPHA_BLEND;
		}

	protected:
		RenderState mRenderState;

		// material
		COLOR mEmissive;
		COLOR mAmbient;
		COLOR mDiffuse;
		COLOR mSpecular;
		float mSpecularPower;

		Texture* mEmissiveMap;
		Texture* mDiffuseMap;
		Texture* mSpecularMap;
		Texture* mNormalMap;
	};


	// class MeshData;
	// class MaterialLoader
	// {
	// public:
	// 	void Load(MeshData * mesh, DataStreamPtr stream);
	// 
	// protected:
	// 	void _loadMaterial(Material * mat, l_node * node);;
	// 
	// protected:
	// 	l_doc doc;
	// };


}

#endif