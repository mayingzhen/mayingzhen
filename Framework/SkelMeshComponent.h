#ifndef  _SKELMESHCOMPONENT__H__
#define  _SKELMESHCOMPONENT__H__

namespace ma
{
	class SkeletonRes;
	class MeshComponent;

	class FRAMEWORK_API SkelMeshComponent : public Component
	{
	public:
		SkelMeshComponent();

		~SkelMeshComponent();

		void AddMeshComp(MeshComponent* pMeshComp);

		void LoadSkeleton(const char* pSkelPath);
	
	private:
		SkeletonRes* m_pSkelRes;

		std::vector<MeshComponent*> m_vMeshComp;	
	};
}

#endif
