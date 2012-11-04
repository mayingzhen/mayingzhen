#ifndef  _MESH__H__
#define  _MESH__H__

#include "Resource.h"

namespace ma
{
	class IRendMesh;
}

namespace ma
{
	class Mesh : public Resource
	{
	public:
		Mesh();
		~Mesh();

		void Load();
		
		bool IsLoad();

		IRendMesh* GetRendMesh() {return m_pRendMesh;}

	private:
		IRendMesh* m_pRendMesh;

	};
}




#endif
