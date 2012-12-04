#ifndef  _MESHCOMPONENT__H__
#define  _MESHCOMPONENT__H__

#include "Framework/Component.h"


namespace ma
{
	class MeshRes;
	class Texture;

	class FRAMEWORK_API MeshComponent : public Component
	{
	public:
		MeshComponent();

		~MeshComponent();

		void Render();

		void Update();

		void Start();

		void Stop();

		void Load(const char* pszMeshPath,const char* pszTexPath);

		MeshRes* GetMeshRes() {return m_pMeshRes;}

		Texture* GetTexture() {return m_pTexture;}

	private:
		MeshRes*		m_pMeshRes;
		Texture*	m_pTexture;


	};



}




#endif