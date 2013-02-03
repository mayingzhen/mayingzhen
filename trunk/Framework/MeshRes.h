#ifndef  _MESH__H__
#define  _MESH__H__

#include "Resource.h"


namespace ma
{
	class IRendMesh;

	class FRAMEWORK_API MeshRes : public Resource
	{
	public:
		MeshRes(const char* pszPath = NULL);
		
		~MeshRes();

		bool Load();
		
		bool IsLoad();

		IRendMesh* GetRendMesh() {return m_pRendMesh;}

		void SetRendMesh(IRendMesh* pRendMesh) {m_pRendMesh = pRendMesh;}

	private:
		IRendMesh* m_pRendMesh;

	};
}




#endif
