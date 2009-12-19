#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "Dx9hook.h"
#include "Renderer.h"
#include "detours.h"

//These are addresses that are called in d3d9.dll.
//This was found by creating a sample directx program, and then debugging using OllyDbg. I then found
//out where these functions were being called and I wrote down the addresses.
#define ADDR_Present 0x4FE10EA0
#define ADDR_BeginScene 0x4FE57010
#define ADDR_EndScene 0x4FE571A0
#define ADDR_Clear 0x4FE55720

HRESULT (STDMETHODCALLTYPE *RealPresent)(IDirect3DDevice9 *This, CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion) = NULL;
HRESULT (STDMETHODCALLTYPE *RealEndScene)(IDirect3DDevice9 *This) = NULL;

//=====================================================
HRESULT STDMETHODCALLTYPE MyPresent(IDirect3DDevice9 *This, CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion)
{
  static bool bInit = true;
  if (bInit) {
    Renderer::Get()->SetDevice(This);
    bInit = false;
  }

  //Renderer::Get ()->Render();

  HRESULT hr = RealPresent(This, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

  return hr;
}

//=====================================================
HRESULT STDMETHODCALLTYPE MyEndScene(IDirect3DDevice9 *This)
{
  static bool bInit = true;
  if (bInit) {
    Renderer::Get()->SetDevice(This);
    bInit = false;
  }

  Renderer::Get()->Render();

	HRESULT rc = RealEndScene(This);
	
	return rc;
}

//=====================================================
void WINAPI SetUpDx9Hook()
{
  RealEndScene = (HRESULT(STDMETHODCALLTYPE *)(IDirect3DDevice9 *))ADDR_EndScene;
  DetourTransactionBegin();
  DetourUpdateThread (GetCurrentThread());
  DetourAttach(&(PVOID&)RealEndScene, MyEndScene);
  DetourTransactionCommit();
}

//=====================================================
void WINAPI CleanUpDx9Hook()
{
  DetourTransactionBegin();
  DetourUpdateThread(GetCurrentThread());
  DetourDetach(&(PVOID&)RealEndScene, MyEndScene);
  DetourTransactionCommit();
}
