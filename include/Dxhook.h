#ifndef _DXHOOK_H
#define _DXHOOK_H

#ifdef  _DLL_
#define HOOK_APICALL __declspec(dllexport)
#else
#define HOOK_APICALL __declspec(dllimport)
#endif

extern "C" HOOK_APICALL int _cdecl Hook ();
extern "C" HOOK_APICALL int _cdecl Unhook ();

extern HINSTANCE g_hInst;

#endif
