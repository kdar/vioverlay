#include <wx/wx.h>
#include <wx/app.h>
#include <wx/image.h>
#include <wx/filesys.h>
#include <wx/fs_zip.h>

#include "gui/MainApp.h"

#define PRODUCTION 0

#if (PRODUCTION == 1)
#include "Resources.cpp"
#endif

IMPLEMENT_APP (MainApp)

//=====================================================
bool MainApp::OnInit ()
{
  wxImage::AddHandler (new wxXPMHandler);

  wxXmlResource::Get ()->InitAllHandlers ();

#if (PRODUCTION == 1)
    wxFileSystem::AddHandler (new wxZipFSHandler);
    InitXmlResource ();
    //wxXmlResource::Get ()->Load ("resources/main.xrs");
#else    
    wxXmlResource::Get ()->Load ("../resources/main.xml");
#endif

  m_pFrame = new MainFrame (NULL);
  SetTopWindow (m_pFrame);
  m_pFrame->Show (true);

  return true;
}

//=====================================================
int MainApp::OnExit ()
{
  return 0;
}
