#ifndef  _BtPhysicsExport__H__
#define  _BtPhysicsExport__H__

#ifdef BULLETPHYSICS_EXPORTS
#define BULLETPHYSICS_API _declspec (dllexport)
#else
#define BULLETPHYSICS_API _declspec (dllimport)
#endif


#endif