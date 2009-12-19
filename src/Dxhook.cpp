#include <windows.h>
#define _DLL_
#include <string>
#include "Dxhook.h"
#include "Dx9hook.h"
#include "Renderer.h"
#include "common/Ipc.h"

//: GLOBALS
//:---------------------------------------------------------------------------
char 				     *g_Version          = "1.0";
HINSTANCE			    g_hInst            = NULL;
CRITICAL_SECTION	g_critical_section;

//: SHARED GLOBALS
//:---------------------------------------------------------------------------
#pragma data_seg(".SHAREDATA")
#pragma data_seg()

//=====================================================
HOOK_APICALL int _cdecl Hook()
{	
	::EnterCriticalSection(&g_critical_section);
	
	SetUpDx9Hook();

	::LeaveCriticalSection(&g_critical_section);
	
	return 1;
}

//=====================================================
HOOK_APICALL int _cdecl Unhook()
{
	::EnterCriticalSection(&g_critical_section);

	CleanUpDx9Hook();

	::LeaveCriticalSection(&g_critical_section);
	
	return 1;
}

//=====================================================
BOOL APIENTRY DllMain(HANDLE hModule, DWORD reason, LPVOID lpReserved)
{
	g_hInst = (HINSTANCE)hModule;
  	
	if (reason == DLL_PROCESS_ATTACH) {
		try
		{
			::DisableThreadLibraryCalls(g_hInst);

			::InitializeCriticalSection(&g_critical_section);

			Hook();
		}
		catch (...) {
      return FALSE;
    }
	} else if (reason == DLL_PROCESS_DETACH) {
		try
		{
			Unhook();             

			::DeleteCriticalSection(&g_critical_section);
		}
		catch (...) {
      return FALSE;
    }
	}
	
	return TRUE;
}
