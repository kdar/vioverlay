#ifndef _MAINFRAME_H
#define _MAINFRAME_H

#include <wx/xrc/xmlres.h>
#include <wx/frame.h>

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "MainFrame.cpp"
#endif

#ifndef __WIN32__
typedef HWND int;
#endif

#include "wxpl.h"

//-----------------------------------------------------
class MainFrame : public wxFrame
{
  public:
    MainFrame () {};
    MainFrame (wxWindow *pParent);
	  ~MainFrame ();

    void OnButton_Test (wxCommandEvent &event);
    void OnButton_Test2 (wxCommandEvent &event);
    void OnButton_Test3 (wxCommandEvent &event);
    void OnButton_Test4 (wxCommandEvent &event);

  protected:
    DECLARE_EVENT_TABLE ()

  private:
    HMODULE m_hMod;

    PluginServer *m_pPluginServer;
};

#endif
