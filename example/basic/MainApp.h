#ifndef _MAINAPP_H
#define _MAINAPP_H

#include "MainFrame.h"

//-----------------------------------------------------
class MainApp : public wxApp
{
  public:
    virtual bool OnInit ();
	virtual int OnExit ();

  private:
    MainFrame *m_pFrame;
};

DECLARE_APP (MainApp)

#endif
