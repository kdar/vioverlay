#include "Renderer.h"

Renderer *Renderer::ms_pInstance = NULL;

//=====================================================
Renderer::Renderer(LPDIRECT3DDEVICE9 pDevice)
  : m_pDevice(pDevice), m_font(NULL), m_sprite(NULL)
{
}

//=====================================================
Renderer::~Renderer()
{
  if (m_font)
    m_font->Release();
}

//=====================================================
/*static*/ Renderer *Renderer::Get()
{
  if (!ms_pInstance)
    ms_pInstance = new Renderer();
  return ms_pInstance;
}

//=====================================================
/*static*/ Renderer *Renderer::Set(Renderer *pInstance)
{
  Renderer *pOldInstance = ms_pInstance;
  ms_pInstance = pInstance;
  return pOldInstance;
}

//=====================================================
void Renderer::SetDevice(LPDIRECT3DDEVICE9 pDevice)
{
  m_pDevice = pDevice;
}

//=====================================================
void Renderer::Render()
{
  SharedStruct<IpcStruct> *pSharedStruct = Ipc::Get()->GetSharedStruct();
  LPAIOVERLAY_Info pInfo;
  LPAIOVERLAY_InfoOptions pOptions;
  LPAIOVERLAY_Calculations pCalculations;
  for (int x = 0; x < MAX_INFOS; x++) {
    pInfo = &(*pSharedStruct)->infos[x];
    pOptions = &pInfo->options;
    pCalculations = &pInfo->calculations;

    if (pInfo->nState == info_New) {
      pInfo->nState = info_Displaying;
      CalculateRect (pInfo);     

      switch (pOptions->position) {
        case pos_AutoTopLeft:
          pOptions->rect.top = INFO_SCREEN_PADDING;
          pOptions->rect.left = INFO_SCREEN_PADDING;
          for (int y = 0; y < MAX_INFOS; y++) {
            if (x != y && (*pSharedStruct)->infos[y].nState != info_Invalid &&
                (*pSharedStruct)->infos[y].options.position == pos_AutoTopLeft) {
              if ((*pSharedStruct)->infos[y].options.rect.bottom >= pOptions->rect.top) {
                int nOldDistance =  pOptions->rect.bottom - pOptions->rect.top;
                pOptions->rect.top = (*pSharedStruct)->infos[y].options.rect.bottom + INFO_CONSECUTIVE_PADDING;
                pOptions->rect.bottom = pOptions->rect.top + nOldDistance;
              }
            }
          }
        break;
      }

      //char test[1024];
      //sprintf (test, "%d, %d, %d, %d", pInfo->options.rect.top, pInfo->options.rect.left, pInfo->options.rect.bottom, pInfo->options.rect.right);
      //MessageBox (NULL, test, "OK", MB_OK);

      pCalculations->dwStartTime = GetTickCount();
    } else if (pInfo->nState == info_Displaying) {
      DrawText (pInfo);
      if (GetTickCount () - pCalculations->dwStartTime >= pOptions->dwHoldTime) {
        pInfo->nState = info_Fading;
      }
    } else if (pInfo->nState == info_Fading) {
      DrawText (pInfo);
      pOptions->nAlpha -= pOptions->nFadeRate;
      pOptions->nOutlineAlpha -= pOptions->nFadeRate;
      if (pOptions->nAlpha < 0) {
        ResetInfo (pInfo);
      }
      if (pOptions->nOutlineAlpha < 0) {
        pOptions->nOutlineAlpha = 0;
      }
    }
  }
}

//=====================================================
void Renderer::DrawText (LPAIOVERLAY_Info pInfo)
{
  if (!m_pDevice) return;
  if (!m_font) CreateFont();
  LPAIOVERLAY_InfoOptions pOptions = &pInfo->options;

  if (!m_sprite) {
    D3DXCreateSprite (m_pDevice, &m_sprite);
  }

  m_sprite->Begin (D3DXSPRITE_ALPHABLEND);

  //Calculate the true color. It includes RGB color plus alpha.
  DWORD dwTrueColor = 0xFFFFFFFF & (pOptions->nAlpha << 24) | pOptions->dwColor;
  DWORD dwTrueOutlineColor = 0xFFFFFFFF & (pOptions->nOutlineAlpha << 24) | pOptions->dwOutlineColor;

  /*!!!!!!!!!*/
  //HELP!!! I DON'T KNOW HOW TO DO DIRECTX TEXT OUTLINING OR TRANSPARENT BOXES!
  RECT rect = pOptions->rect;
  rect.top -= 1;
  rect.left -= 1;
  m_font->DrawText(m_sprite, pInfo->szData, -1, &rect, DT_LEFT|DT_NOCLIP, dwTrueOutlineColor);
  rect.left += 1;
  m_font->DrawText(m_sprite, pInfo->szData, -1, &rect, DT_LEFT|DT_NOCLIP, dwTrueOutlineColor);
  rect.left += 1;
  m_font->DrawText(m_sprite, pInfo->szData, -1, &rect, DT_LEFT|DT_NOCLIP, dwTrueOutlineColor);
    
  rect.top += 1;
  m_font->DrawText(m_sprite, pInfo->szData, -1, &rect, DT_LEFT|DT_NOCLIP, dwTrueOutlineColor);
  rect.left -= 1;
  m_font->DrawText(m_sprite, pInfo->szData, -1, &rect, DT_LEFT|DT_NOCLIP, dwTrueOutlineColor);
  rect.left -= 1;
  m_font->DrawText(m_sprite, pInfo->szData, -1, &rect, DT_LEFT|DT_NOCLIP, dwTrueOutlineColor);

  rect.top += 1;
  m_font->DrawText(m_sprite, pInfo->szData, -1, &rect, DT_LEFT|DT_NOCLIP, dwTrueOutlineColor);
  rect.left += 1;
  m_font->DrawText(m_sprite, pInfo->szData, -1, &rect, DT_LEFT|DT_NOCLIP, dwTrueOutlineColor);
  rect.left += 1;
  m_font->DrawText(m_sprite, pInfo->szData, -1, &rect, DT_LEFT|DT_NOCLIP, dwTrueOutlineColor);
  /*!!!!!!!!*/

  //Draw the text.
  m_font->DrawText(m_sprite, pInfo->szData, -1, &pOptions->rect, DT_LEFT|DT_NOCLIP, dwTrueColor);
  
  m_sprite->End();
}

//=====================================================
void Renderer::CalculateRect (LPAIOVERLAY_Info pInfo)
{
  if (!m_font) CreateFont();
  m_font->DrawText(NULL, pInfo->szData, -1, &pInfo->options.rect, DT_CALCRECT, 0);
}

//=====================================================
void Renderer::CreateFont ()
{
  if (!m_pDevice) return;

  if (!m_font) {
    D3DXCreateFont(m_pDevice,                //D3D Device
                   18,                       //Font height
                   0,                        //Font width
                   FW_NORMAL,                //Font Weight
                   1,                        //MipLevels
                   false,                    //Italic
                   DEFAULT_CHARSET,          //CharSet
                   OUT_DEFAULT_PRECIS,       //OutputPrecision
                   ANTIALIASED_QUALITY,      //Quality
                   DEFAULT_PITCH|FF_DONTCARE,//PitchAndFamily
                   "Corbel Bold",            //pFacename,
                   &m_font);                 //ppFont
  }
}
