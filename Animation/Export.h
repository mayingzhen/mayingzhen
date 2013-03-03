#ifndef  _AnimationExport__H__
#define  _AnimationExport__H__

#ifdef ANIMATION_EXPORTS
#define ANIMATION_API _declspec (dllexport)
#else
#define ANIMATION_API _declspec (dllimport)
#endif


#endif