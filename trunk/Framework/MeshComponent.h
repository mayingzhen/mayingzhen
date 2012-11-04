#ifndef  _MESHCOMPONENT__H__
#define  _MESHCOMPONENT__H__

#include "Component.h"

namespace ma
{
	class Mesh;
}

namespace ma
{
	class MeshComponent : public Component
	{
	public:
		MeshComponent();

		~MeshComponent();

		void Render();

		void Update();

	private:
		Mesh* m_pMesh;


	};



}




#endif