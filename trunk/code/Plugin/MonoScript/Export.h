#ifndef __MONOSCRIPTEXPORT_H_
#define __MONOSCRIPTEXPORT_H_

#ifdef _MSC_VER

	#ifdef MONOSCRIPT_EXPORTS
	# define MonoScriptAPI __declspec ( dllexport )
	#else
	# define MonoScriptAPI __declspec ( dllimport )
	#endif
#else 
	#define  MonoScriptAPI

#endif



#endif // __MONOSCRIPTEXPORT_H_
