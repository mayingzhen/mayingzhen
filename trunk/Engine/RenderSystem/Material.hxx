#include "Material.h"
#include "IRenderDevice.h"

#pragma warning(push)
#pragma warning(disable : 4244)

namespace ma
{

// 	const Color4 Color4::White              = Color4(1.0f, 1.0f, 1.0f, 1.0f);
// 	const Color4 Color4::Red                = Color4(1.0f, 0.0f, 0.0f, 1.0f);
// 	const Color4 Color4::Green              = Color4(0.0f, 1.0f, 0.0f, 1.0f);
// 	const Color4 Color4::Blue               = Color4(0.0f, 0.0f, 1.0f, 1.0f);
// 	const Color4 Color4::Black              = Color4(0.0f, 0.0f, 0.0f, 1.0f);
// 	const Color4 Color4::Gray               = Color4(0.5f, 0.5f, 0.5f, 1.0f);

Material::Material()
{
    mRenderState.cullMode = CULL_CCW;
    mRenderState.fillMode = FILL_SOLID;
    mRenderState.colorWrite = CWE_ALL;
    mRenderState.depthWrite = true;
    mRenderState.depthCheck = DCM_LESS_EQUAL;
    mRenderState.blendMode = BM_OPATICY;
    mRenderState.alphaTestRef = 0.5f;

//     mEmissive = COLOR::Black;
//     mAmbient = COLOR::Gray;
//     mDiffuse = COLOR::White;
//     mSpecular = COLOR::Black;
	   mEmissive = Color(0.0f, 0.0f, 0.0f, 1.0f);
	   mAmbient = Color(0.5f, 0.5f, 0.5f, 1.0f);
       mDiffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	   mSpecular = Color(0.0f, 0.0f, 0.0f, 1.0f);
    mSpecularPower = 80;

    //mEmissiveMap = RenderHelper::Instance()->GetBlackTexture();
    //mDiffuseMap = RenderHelper::Instance()->GetWhiteTexture();
    //mSpecularMap = RenderHelper::Instance()->GetBlackTexture();
    //mNormalMap = RenderHelper::Instance()->GetDefaultNormalTexture();
}

Material::~Material()
{
}

void Material::SetEmissive(const COLOR & color)
{
    mEmissive = color;
}

void Material::SetAmbient(const COLOR & color)
{
    mAmbient = color;
}

void Material::SetDiffuse(const COLOR & color)
{
    mDiffuse = color;
}

void Material::SetSpecular(const COLOR & color)
{
    mSpecular = color;
}

void Material::SetEmissiveMap(const char* tex)
{
	//if (tex != "")
	//	mEmissiveMap = VideoBufferManager::Instance()->Load2DTexture(tex, tex);
	//else
	//	mEmissiveMap = RenderHelper::Instance()->GetBlackTexture();
}

void Material::SetDiffuseMap(const char* tex)
{
	//if (tex != "")
	//	mDiffuseMap = VideoBufferManager::Instance()->Load2DTexture(tex, tex);
	//else
	//	mDiffuseMap = RenderHelper::Instance()->GetWhiteTexture();
}

void Material::SetSpecularMap(const char* tex)
{
	//if (tex != "")
	//	mSpecularMap = VideoBufferManager::Instance()->Load2DTexture(tex, tex);
	//else
	//	mSpecularMap = RenderHelper::Instance()->GetBlackTexture();
}

void Material::SetNormalMap(const char* tex)
{
	//if (tex != "")
	//	mNormalMap = VideoBufferManager::Instance()->Load2DTexture(tex, tex);
	//else
	//	mNormalMap = RenderHelper::Instance()->GetDefaultNormalTexture();
}

void Material::SetSpecularPower(float power)
{
    mSpecularPower = power;
}

void Material::SetEmissiveMap(Texture* tex)
{
    mEmissiveMap = tex;
}

void Material::SetDiffuseMap(Texture* tex)
{
    mDiffuseMap = tex;
}

void Material::SetSpecularMap(Texture* tex)
{
    mSpecularMap = tex;
}

void Material::SetNormalMap(Texture* tex)
{
    mNormalMap = tex;
}

const COLOR & Material::GetEmissive() const
{
    return mEmissive;
}

const COLOR & Material::GetAmbient() const
{
    return mAmbient;
}

const COLOR & Material::GetDiffuse() const
{
    return mDiffuse;
}

const COLOR & Material::GetSpecular() const
{
    return mSpecular;
}

float Material::GetSpecularPower() const
{
    return mSpecularPower;
}

Texture* Material::GetEmissiveMap() const
{
    return mEmissiveMap;
}

Texture* Material::GetDiffuseMap() const
{
    return mDiffuseMap;
}

Texture* Material::GetSpecularMap() const
{
    return mSpecularMap;
}

Texture* Material::GetNormalMap() const
{
    return mNormalMap;
}












// #include "MeshData.h"
// 
// CULL_MODE GetCullMode(const char * m)
// {
//     if (strcmp(m, "none") == 0)
//         return CULL_NONE;
//     else if (strcmp(m, "ccw") == 0)
//         return CULL_CCW;
//     else if (strcmp(m, "cw") == 0)
//         return CULL_CW;
//     else 
//     {
//         LOG_PRINT_FORMAT("unknown cull mode '%s'.\n", m);
//         return CULL_NONE;
//     }
// }
// 
// FILL_MODE GetFillMode(const char * m)
// {
//     if (strcmp(m, "solid") == 0)
//         return FILL_SOLID;
//     else if (strcmp(m, "frame") == 0)
//         return FILL_FRAME;
//     else if (strcmp(m, "point") == 0)
//         return FILL_POINT;
//     else
//     {
//         LOG_PRINT_FORMAT("unknown fill mode '%s'.\n", m);
//         return FILL_SOLID;
//     }
// }
// 
// bool GetBoolen(const char * m)
// {
//     if (strcmp(m, "true") == 0)
//         return true;
//     else if (strcmp(m, "false") == 0)
//         return false;
//     else
//     {
//         LOG_PRINT_FORMAT("unknown bool '%s'. \n", m);
//         return true;
//     }
// }
// 
// DEPTH_CHECK_MODE GetDepthCheck(const char * m)
// {
//     if (strcmp(m, "less_equal") == 0)
//         return DCM_LESS_EQUAL;
//     else if (strcmp(m, "less") == 0)
//         return DCM_LESS;
//     else if (strcmp(m, "greater_equal") == 0)
//         return DCM_GREATER_EQUAL;
//     else if (strcmp(m, "greater") == 0)
//         return DCM_GREATER;
//     else if (strcmp(m, "equal"))
//         return DCM_EQUAL;
//     else if (strcmp(m, "always") == 0)
//         return DCM_ALWAYS;
//     else
//     {
//         LOG_PRINT_FORMAT("unknown depth check '%s'.\n", m);
//         return DCM_LESS_EQUAL;
//     }
// }
// 
// BLEND_MODE GetBlendMode(const char * m)
// {
//     if (strcmp(m, "opaticy") == 0)
//         return BM_OPATICY;
//     else if (strcmp(m, "alpha_blend") == 0)
//         return BM_ALPHA_BLEND;
//     else if (strcmp(m, "alpha_test") == 0)
//         return BM_ALPHA_TEST;
//     else if (strcmp(m, "add") == 0)
//         return BM_ADD;
//     else if (strcmp(m, "multiply") == 0)
//         return BM_MULTIPLY;
//     else
//     {
//         LOG_PRINT_FORMAT("unknown blend mode '%s'.\n", m);
//         return BM_OPATICY;
//     }
// }
// 
// void MaterialLoader::_loadMaterial(Material * mat, l_node * node)
// {
//     l_node * ndCullMode = node->first_node("cull_mode");
//     l_node * ndFillMode = node->first_node("fill_mode");
//     l_node * ndDepthWrite = node->first_node("depth_write");
//     l_node * ndDepthCheck = node->first_node("depth_check");
//     l_node * ndBlendMode = node->first_node("blend_mode");
//     l_node * ndAlphaTestRef = node->first_node("alpha_test_ref");
//     l_node * ndEmissive = node->first_node("emissive");
//     l_node * ndAmbient = node->first_node("ambient");
//     l_node * ndDiffuse = node->first_node("diffuse");
//     l_node * ndSpecular = node->first_node("specular");
//     l_node * ndSpecularPower = node->first_node("specular_power");
//     l_node * ndEmissiveMap = node->first_node("emissive_map");
//     l_node * ndDiffuseMap = node->first_node("diffuse_map");
//     l_node * ndSpecularMap = node->first_node("specular_map");
//     l_node * ndNormalMap = node->first_node("normal_map");
// 
//     l_node * ndShaderProvider = doc.first_node("shader_provider");
// 
//     if (ndCullMode)
//     {
//         l_attri * attri = ndCullMode->first_attribute("value");
//         CULL_MODE mode = GetCullMode(attri->value());
//         mat->SetCullMode(mode);
//     }
// 
//     if (ndFillMode)
//     {
//         l_attri * attri = ndFillMode->first_attribute("value");
//         FILL_MODE mode = GetFillMode(attri->value());
//         mat->SetFillMode(mode);
//     }
// 
//     if (ndDepthWrite)
//     {
//         l_attri * attri = ndFillMode->first_attribute("value");
//         bool b = GetBoolen(attri->value());
//         mat->SetDepthWrite(b);
//     }
// 
//     if (ndDepthCheck)
//     {
//         l_attri * attri = ndDepthCheck->first_attribute("value");
//         DEPTH_CHECK_MODE mode = GetDepthCheck(attri->value());
//         mat->SetDepthCheck(mode);
//     }
// 
//     if (ndBlendMode)
//     {
//         l_attri * attri = ndBlendMode->first_attribute("value");
//         BLEND_MODE mode = GetBlendMode(attri->value());
//         mat->SetBlendMode(mode);
//     }
// 
//     if (ndAlphaTestRef)
//     {
//         l_attri * attri = ndAlphaTestRef->first_attribute("value");
//         float v = atof(attri->value());
//         mat->SetAlphaTestRef(v);
//     }
// 
//     if (ndEmissive)
//     {
//         float r = atof(ndEmissive->first_attribute("r")->value()); 
//         float g = atof(ndEmissive->first_attribute("g")->value()); 
//         float b = atof(ndEmissive->first_attribute("b")->value());
// 
//         mat->SetEmissive(COLOR(r, g, b));
//     }
// 
//     if (ndAmbient)
//     {
//         float r = atof(ndAmbient->first_attribute("r")->value()); 
//         float g = atof(ndAmbient->first_attribute("g")->value()); 
//         float b = atof(ndAmbient->first_attribute("b")->value());
// 
//         mat->SetAmbient(COLOR(r, g, b));
//     }
// 
//     if (ndDiffuse)
//     {
//         float r = atof(ndDiffuse->first_attribute("r")->value()); 
//         float g = atof(ndDiffuse->first_attribute("g")->value()); 
//         float b = atof(ndDiffuse->first_attribute("b")->value());
// 
//         mat->SetDiffuse(COLOR(r, g, b));
//     }
// 
//     if (ndSpecular)
//     {
//         float r = atof(ndSpecular->first_attribute("r")->value()); 
//         float g = atof(ndSpecular->first_attribute("g")->value()); 
//         float b = atof(ndSpecular->first_attribute("b")->value());
// 
//         mat->SetSpecular(COLOR(r, g, b));
//     }
// 
//     if (ndSpecularPower)
//     {
//         float v = atof(ndSpecularPower->first_attribute("value")->value());
//         mat->SetSpecularPower(v);
//     }
// 
//     if (ndEmissiveMap)
//     {
//         const char * t = ndEmissiveMap->first_attribute("filename")->value();
//         mat->SetEmissiveMap(t);
//     }
// 
//     if (ndDiffuseMap)
//     {
//         const char * t = ndDiffuseMap->first_attribute("filename")->value();
//         mat->SetDiffuseMap(t);
//     }
// 
//     if (ndSpecularMap)
//     {
//         const char * t = ndSpecularMap->first_attribute("filename")->value();
//         mat->SetSpecularMap(t);
//     }
// 
//     if (ndNormalMap)
//     {
//         const char * t = ndNormalMap->first_attribute("filename")->value();
//         mat->SetNormalMap(t);
//     }
// }


// void MaterialLoader::Load(MeshData * mesh, DataStreamPtr stream)
// {
//     doc.parse<0>((char *)stream->GetData());
// 
//     int i = 0;
// 
//     l_node * root = doc.first_node("Materials"); 
//     l_node * node = root->first_node("Material");
// 
//     while (node)
//     {
//         Material * mat = mesh->GetSubMesh(i++)->GetMaterial();
// 
//         _loadMaterial(mat, node);
// 
//         node = node->next_sibling("Material");
//     }
// }


}


//#pragma warning(pop)

