#ifndef  _FbxImport_Module__H__
#define  _FbxImport_Module__H__


#ifdef FBXImporter_EXPORTS
#define FBXImporter_API _declspec (dllexport)
#else
#define FBXImporter_API _declspec (dllimport)
#endif


#include "Plugin/FbxImport/FBXImporter.h"


#endif

