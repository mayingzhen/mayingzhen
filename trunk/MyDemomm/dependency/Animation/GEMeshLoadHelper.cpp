
#include "GEMeshLoadHelper.h"


GEMeshLoadHelper GEMeshLoadHelper::g_MeshLoadHelper;

GEMesh* GEMeshLoadHelper::GetMeshLoaded(const string& meshFile) const
{
	map< string, list<GEMesh*> >::const_iterator itr;
	
	itr = m_MeshLoaded.find(meshFile);

	if (itr != m_MeshLoaded.end())	
	{
		if (itr->second.size() > 0)
		{
			return itr->second.front();
		}
	}
	
    return NULL;

}


bool GEMeshLoadHelper::TagMeshAsLoaded(const string& meshFile, const GEMesh* mesh)
{
	map< string, list<GEMesh*> >::iterator itr;
	itr = m_MeshLoaded.find(meshFile);

	if (itr != m_MeshLoaded.end())	
	{
		list<GEMesh*>::const_iterator p;
		for( p = itr->second.begin(); p != itr->second.end(); ++p)
		{
			if (*p == mesh)
				break;
		}

		if (p == itr->second.end())
		{
			itr->second.push_back(const_cast<GEMesh*>(mesh));
			return true;
		}
	}
	else
	{
		// add new meshFile
		list<GEMesh*> loadedMeshList;
		loadedMeshList.push_back(const_cast<GEMesh*>(mesh));
		m_MeshLoaded.insert(make_pair(meshFile, loadedMeshList));
		return true;
	}

	return false;
}

bool GEMeshLoadHelper::TagMeshAsUnloaded(const string& meshFile, const GEMesh* mesh)
{
	map< string, list<GEMesh*> >::iterator itr;
	itr = m_MeshLoaded.find(meshFile);

	if (itr != m_MeshLoaded.end())	
	{
		list<GEMesh*>::iterator p;
		for( p = itr->second.begin(); p != itr->second.end(); ++p)
		{
			if (*p == mesh)
			{
				itr->second.erase(p);
				return true;
			}
		}

	}

	return false;
}