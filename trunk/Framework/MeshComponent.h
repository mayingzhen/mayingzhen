#ifndef  _MESHCOMPONENT__H__
#define  _MESHCOMPONENT__H__

#include "Framework/Object.h"

namespace ma
{
	class Mesh;
}

namespace ma
{
	class MeshComponent : public Object
	{
	public:
		MeshComponent();

		~MeshComponent();

		void Render();

		void Update();

		void Start();

		void Stop();

	private:
		Mesh* m_pMesh;


	};



}




#endif