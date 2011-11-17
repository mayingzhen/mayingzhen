
#ifndef GEMESHLOADHELPER_H
#define GEMESHLOADHELPER_H

#include "GEMesh.h"

#include <map>
#include <list>
using namespace std;

class GEMeshLoadHelper
{
public:
	~GEMeshLoadHelper(void){};

	static GEMeshLoadHelper& GetSingleton();

	GEMesh* GetMeshLoaded(const string& meshFile) const;

	bool TagMeshAsLoaded(const string& meshFile, const GEMesh* mesh);
	bool TagMeshAsUnloaded(const string& meshFile, const GEMesh* mesh);
		
private:
	GEMeshLoadHelper() {}

	static GEMeshLoadHelper g_MeshLoadHelper;

	map< string, list<GEMesh*> > m_MeshLoaded;
};

inline GEMeshLoadHelper& GEMeshLoadHelper::GetSingleton()
{ 
	return g_MeshLoadHelper; 
}

#endif