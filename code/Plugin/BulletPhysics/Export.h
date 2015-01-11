#ifndef  _BtPhysicsExport__H__
#define  _BtPhysicsExport__H__

#ifdef _MSC_VER
	#ifdef BULLETPHYSICS_EXPORTS
	#define BULLETPHYSICS_API _declspec (dllexport)
	#else
	#define BULLETPHYSICS_API _declspec (dllimport)
	#endif
#else // BULLETPHYSICS_EXPORTS
	#define BULLETPHYSICS_API
#endif //_MSC_VER

#endif //_BtPhysicsExport__H__
