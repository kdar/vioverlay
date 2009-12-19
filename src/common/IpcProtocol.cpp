#include "common/IpcProtocol.h"

//=====================================================
void InitIpcStruct (LPIpcStruct pStruct)
{  
  for (int x = 0; x < MAX_INFOS; x++) {
    ResetInfo (&pStruct->infos[x]);
  }
}

//=====================================================
void ResetInfo (LPAIOVERLAY_Info pInfo)
{
  LPAIOVERLAY_InfoOptions pOptions;
  pOptions = &pInfo->options;

  pInfo->nState = info_Invalid;
  pOptions->position = pos_AutoTopLeft;
  pOptions->rect.top = 10;
  pOptions->rect.left = 10;
  pOptions->rect.bottom = 0;
  pOptions->rect.right = 0;
  pOptions->dwHoldTime = 10000;
  pOptions->nFadeRate = 2;
  pOptions->dwColor = 0xFFFFFF;
  pOptions->nAlpha = 0xFF;
  pOptions->dwOutlineColor = 0x000000;
  pOptions->nOutlineAlpha = 0xAA;
}
