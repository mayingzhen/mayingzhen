#ifndef  _AnimationExport__H__
#define  _AnimationExport__H__

#ifdef _MSC_VER
	#ifdef ANIMATION_EXPORTS
	#define ANIMATION_API _declspec (dllexport)
	#else
	#define ANIMATION_API _declspec (dllimport)
	#endif
#else
	#define ANIMATION_API	
#endif

#endif