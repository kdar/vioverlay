#include <wx/wx.h>
#include <wx/frame.h>

#include "gui/MainFrame.h"
#include "common/Ipc.h"
#include "RemoteLib.h"

#include <string>
using namespace std;

BEGIN_EVENT_TABLE (MainFrame, wxFrame)
  EVT_BUTTON (XRCID ("Id_Button_Test"), MainFrame::OnButton_Test)
  EVT_BUTTON (XRCID ("Id_Button_Test2"), MainFrame::OnButton_Test2)
  EVT_BUTTON (XRCID ("Id_Button_Test3"), MainFrame::OnButton_Test3)
  EVT_BUTTON (XRCID ("Id_Button_Test4"), MainFrame::OnButton_Test4)
END_EVENT_TABLE ()

//=====================================================
MainFrame::MainFrame (wxWindow *pParent)
{
  //We must have a frame created to set and receive a hotkey
  SetParent (pParent);
  wxXmlResource::Get ()->LoadFrame (this, GetParent (), wxT("MainFrame"));

  //SetIcon (wxIcon ("IDI_MAIN", wxBITMAP_TYPE_ICO_RESOURCE));

  m_pPluginServer = new PluginServer ();
}

//=====================================================
MainFrame::~MainFrame ()
{
  if (m_pPluginServer) delete m_pPluginServer;
}

//=====================================================
void MainFrame::OnButton_Test (wxCommandEvent &WXUNUSED(event))
{
  HWND hWnd = ::FindWindow ("D3D Tutorial", "D3D Tutorial 02: Vertices");
  if (hWnd == NULL)
    hWnd = ::FindWindow ("GxWindowClassD3d", "World of Warcraft");
  if (hWnd == NULL) {
    MessageBox (NULL, "Window not found.", "Error", MB_OK);
    return;
  }
  m_hMod = RemoteLoadLibrary (hWnd, "D:\\Programming\\C++\\Projects\\AIOverlay\\distro\\Dxhook.dll", 3000);
  if (m_hMod == NULL)
    MessageBox (NULL, "Unable to load library.", "Error", MB_OK);
}

//=====================================================
void MainFrame::OnButton_Test2 (wxCommandEvent &WXUNUSED(event))
{
  SharedStruct<IpcStruct> *pSharedStruct = Ipc::Get ()->GetSharedStruct ();
  
  (*pSharedStruct)->infos[0].nState = info_New;
  (*pSharedStruct)->infos[0].options.dwHoldTime = 4000;
  strncpy ((*pSharedStruct)->infos[0].szData, "I love kristine more than", strlen ("I love kristine more than"));

  (*pSharedStruct)->infos[1].nState = info_New;
  (*pSharedStruct)->infos[1].options.dwHoldTime = 5000;
  strncpy ((*pSharedStruct)->infos[1].szData, "anything else in this world.", strlen ("anything else in this world."));

  (*pSharedStruct)->infos[2].nState = info_New;
  (*pSharedStruct)->infos[2].options.dwHoldTime = 6000;
  strncpy ((*pSharedStruct)->infos[2].szData, "and I won't ever stop", strlen ("and I won't ever stop"));

  (*pSharedStruct)->infos[3].nState = info_New;
  (*pSharedStruct)->infos[3].options.dwHoldTime = 7000;
  strncpy ((*pSharedStruct)->infos[3].szData, "convincing her of that", strlen ("convincing her of that"));
}

//=====================================================
void MainFrame::OnButton_Test3 (wxCommandEvent &WXUNUSED(event))
{
  HWND hWnd = ::FindWindow ("D3D Tutorial", "D3D Tutorial 02: Vertices");
  if (hWnd == NULL)
    hWnd = ::FindWindow ("GxWindowClassD3d", "World of Warcraft");
  RemoteFreeLibrary (hWnd, m_hMod);
}

//=====================================================
void MainFrame::OnButton_Test4 (wxCommandEvent &WXUNUSED(event))
{
  PluginArgs args;
  args.SetArg (0, this);
  m_pPluginServer->LoadAllPlugins ("plugins", "*.dll", &args);
  m_pPluginServer->GetAllPluginInfos ();
  PluginInfo pluginInfo;
  for (int i = 0; i < m_pPluginServer->GetLoadedPlugins (); i++ ) {
		pluginInfo = m_pPluginServer->GetSafedPluginInfo (i);
	}
  m_pPluginServer->RunPlugin (0);
  m_pPluginServer->UnloadAllPlugins (NULL);
}
