#ifndef  _MESH__H__
#define  _MESH__H__

#include "Resource.h"


namespace ma
{
	class IRendMesh;

	class FRAMEWORK_API Mesh : public Resource
	{
	public:
		Mesh(const char* pszPath = NULL);
		
		~Mesh();

		void Load();
		
		bool IsLoad();

		IRendMesh* GetRendMesh() {return m_pRendMesh;}

	private:
		IRendMesh* m_pRendMesh;

	};
}




#endif
