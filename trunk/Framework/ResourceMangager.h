#ifndef  _RESOURCE_MANAGER__H__
#define  _RESOURCE_MANAGER__H__

namespace ma
{
	class AnimationRes;
	class SkeletonRes;
	class MeshRes;
	class Texture;

	class FRAMEWORK_API ResourceManager
	{
	public:
		AnimationRes* CreateAnimationRes(const char* pszAniPath);

		SkeletonRes* CreateSkeletonRes(const char* pszAniPath);

		MeshRes* CreatMeshRes(const char* pszMeshPath);

		Texture* CreateTextureRes(const char* pszTexturePath);

	private:
		std::map<std::string,Resource*> m_allResource;
	};
}


#endif