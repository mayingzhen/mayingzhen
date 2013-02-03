#ifndef __MONOSCRIPTEXPORT_H_
#define __MONOSCRIPTEXPORT_H_


#ifdef MONOSCRIPT_EXPORTS
# define MonoScriptAPI __declspec ( dllexport )
#else
# define MonoScriptAPI __declspec ( dllimport )
#endif

#endif // __MONOSCRIPTEXPORT_H_
