#ifndef _IPC_H
#define _IPC_H

#include "common/IpcProtocol.h"
#include "SharedStruct.h"

typedef void (*TYPEOF_fnOnDataChange)();

//-----------------------------------------------------
class Ipc
{
  public:
    Ipc ();
    ~Ipc ();

    static Ipc *Get ();
    static Ipc *Set (Ipc *pInstance);    
    
    //Creates the thread on _Poll which eventually called Poll ()
    void StartPollThread ();    

    //Set the function to be called if a change is made in the shared memory
    //when we are polling. We do this so we don't have to include the Renderer
    //into this class. Ipc is not only used by the dll, but the main GUI as well
    //and GUI has no use for Renderer.
    void SetOnDataChange (TYPEOF_fnOnDataChange func);

    //Return a pointer to the shared struct
    SharedStruct<IpcStruct> *GetSharedStruct ();

  protected:
    //The real poll, where all the code is at.
    void Poll ();

  private:
    //CreateThread needs a static function to call, and this is it.
    static DWORD WINAPI _Poll (void *pData);

  private:
    static Ipc *ms_pInstance;

    HANDLE	m_PollThread;
	  DWORD	  m_PollThreadID;

    TYPEOF_fnOnDataChange ms_fnOnDataChange;

    SharedStruct<IpcStruct> m_SharedStruct;
};

#endif
