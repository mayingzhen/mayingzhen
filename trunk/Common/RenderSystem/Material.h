#pragma once


#include "Common/RenderSystem/Texture.h"


namespace ma
{

class COMMON_API Material
{
public:
    static const int MAX_EX_TEXTURES = 8;
    static const int MAX_TECHNIQUES = 8; 

public:
    Material();
    ~Material();

    void SetEmissive(const D3DXCOLOR & color);
    void SetAmbient(const D3DXCOLOR & color);
    void SetDiffuse(const D3DXCOLOR & color);
    void SetSpecular(const D3DXCOLOR & color);
    void SetSpecularPower(float power);

    void SetEmissiveMap(const char* tex);
    void SetDiffuseMap(const char* tex);
    void SetSpecularMap(const char* tex);
    void SetNormalMap(const char* tex);

    void SetEmissiveMap(Texture* tex);
    void SetDiffuseMap(Texture* tex);
    void SetSpecularMap(Texture* tex);
    void SetNormalMap(Texture* tex);

    const D3DXCOLOR & GetEmissive() const;
    const D3DXCOLOR & GetAmbient() const;
    const D3DXCOLOR & GetDiffuse() const;
    const D3DXCOLOR & GetSpecular() const;
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
	D3DXCOLOR mEmissive;
    D3DXCOLOR mAmbient;
    D3DXCOLOR mDiffuse;
    D3DXCOLOR mSpecular;
    float mSpecularPower;

    Texture* mEmissiveMap;
    Texture* mDiffuseMap;
    Texture* mSpecularMap;
    Texture* mNormalMap;
};


// class Mesh;
// class MaterialLoader
// {
// public:
// 	void Load(Mesh * mesh, DataStreamPtr stream);
// 
// protected:
// 	void _loadMaterial(Material * mat, xml_node * node);;
// 
// protected:
// 	xml_doc doc;
// };


}