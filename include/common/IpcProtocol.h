#ifndef _IPCPROTOCOL_H
#define _IPCPROTOCOL_H

#define MAX_INFOS 50                //Max number of infos that can be displayed at one time.
#define MAX_INFO_LENGTH 1024        //Max size of the text for each info.
#define INFO_SCREEN_PADDING 10      //The padding between info text and the screen border.
#define INFO_CONSECUTIVE_PADDING -2 //The padding between consecutive infos.

#include <windows.h>

//-----------------------------------------------------
enum InfoState
{
  info_Invalid = 0,
  info_New,
  info_Displaying,
  info_Fading
};

//The position at which  the info will be placed. If it is specified
//as absolute, then the nX and nY parameters are used to position the
//info. If any of the auto ones are used, then text will be automatically
//place based on other infos that are currently there.
enum InfoPosition
{
  pos_Absolute = 0,
  pos_AutoTopLeft,
  pos_AutoTopRight,
  pos_AutoTopCenter,
  pos_AutoBottomLeft,
  pos_AutoBottomRight,
  pos_AutoBottomCenter,
  pos_AutoCenterCenter
};

//The options for each info.
typedef struct {
  InfoPosition position;
  RECT rect;

  DWORD dwColor;
  int nAlpha;
  DWORD dwOutlineColor;
  int nOutlineAlpha;

  DWORD dwHoldTime; //How long to stay on screen (in miliseconds)
  int nFadeRate;    //The rate at which text will fade
} AIOVERLAY_InfoOptions, *LPAIOVERLAY_InfoOptions;

//-Not to be used by the user.-
typedef struct {
  DWORD dwStartTime;
} AIOVERLAY_Calculations, *LPAIOVERLAY_Calculations;

//The info.
typedef struct {
  InfoState nState;
  AIOVERLAY_InfoOptions options;
  AIOVERLAY_Calculations calculations;
  char szData[MAX_INFO_LENGTH];
} AIOVERLAY_Info, *LPAIOVERLAY_Info;

typedef struct {
  AIOVERLAY_Info infos[MAX_INFOS];
} IpcStruct, *LPIpcStruct;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++
void InitIpcStruct (LPIpcStruct pStruct);
void ResetInfo (LPAIOVERLAY_Info pInfo);

#endif
