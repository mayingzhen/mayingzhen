#ifndef  _FrameWOrkExport__H__
#define  _FrameWOrkExport__H__

#ifdef FRAMEWORK_EXPORTS
#define FRAMEWORK_API _declspec (dllexport)
#else
#define FRAMEWORK_API _declspec (dllimport)
#endif

#endif