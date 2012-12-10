#ifndef  _RESOURCE_MANAGER__H__
#define  _RESOURCE_MANAGER__H__

namespace ma
{
	class AnimationRes;
	class SkeletonRes;
	class MeshRes;
	class Texture;

	typedef Resource* (*ResourceCreator)();

	class FRAMEWORK_API ResourceManager
	{
	public:
		Resource* DeclareResource(const char* pszRelPath);

		/////// Resource Creation
		void RegisterResourceFactory(const char* fileExt,ResourceCreator pResCreator);

		void UnregisterResourceFactory(const char* fileExt,ResourceCreator pResCreator);


	private:
		typedef std::map<std::string,Resource*> ResMap;
		typedef std::map<std::string,ResourceCreator> ResCreateFunMap;
		typedef std::map<GUID,SkeletonRes*> SkeletonMap;

		ResMap			m_resMap;

		SkeletonMap		m_skelMap;
		
		ResCreateFunMap m_resCreateFunMap;
	};
}


#endif