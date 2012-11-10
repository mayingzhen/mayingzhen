#ifndef  _MESHCOMPONENT__H__
#define  _MESHCOMPONENT__H__

#include "Framework/Component.h"


namespace ma
{
	class Mesh;
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

	private:
		Mesh*		m_pMesh;
		Texture*	m_pTexture;


	};



}




#endif