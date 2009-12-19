#ifndef _RENDERER_H
#define _RENDERER_H

#include <d3d9.h>
#include <d3dx9.h>

#include <string>

#include "common/Ipc.h"

using namespace std;

//-----------------------------------------------------
class Renderer
{
  public:
    Renderer(LPDIRECT3DDEVICE9 pDevice = NULL);
    ~Renderer();

    static Renderer *Get();
    static Renderer *Set(Renderer *pInstance);

    void SetDevice(LPDIRECT3DDEVICE9 pDevice);

    void Render();

    void DrawText(LPAIOVERLAY_Info pInfo);
    void CalculateRect(LPAIOVERLAY_Info pInfo);
    void CreateFont();

  private:
    static Renderer *ms_pInstance;

    LPDIRECT3DDEVICE9 m_pDevice;
    ID3DXFont *m_font;
    LPD3DXSPRITE m_sprite;

    string m_sInfo;
};

#endif
