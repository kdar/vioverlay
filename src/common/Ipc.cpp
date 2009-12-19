#include <windows.h>
#include "common/Ipc.h"

Ipc *Ipc::ms_pInstance = NULL;

//=====================================================
Ipc::Ipc ()
  : ms_fnOnDataChange(NULL), m_PollThread(NULL), m_PollThreadID(0)
{
  m_SharedStruct.Acquire ("aioverlay_map");
  InitIpcStruct (m_SharedStruct.GetData ());
}

//=====================================================
Ipc::~Ipc ()
{
  m_SharedStruct.Release ();
}

//=====================================================
/*static*/ Ipc *Ipc::Get ()
{
  if (!ms_pInstance)
    ms_pInstance = new Ipc ();
  return ms_pInstance;
}

//=====================================================
/*static*/ Ipc *Ipc::Set (Ipc *pInstance)
{
  Ipc *pOldInstance = ms_pInstance;
  ms_pInstance = pInstance;
  return pOldInstance;
}

//=====================================================
void Ipc::StartPollThread ()
{
  if (!ms_fnOnDataChange) return;

  m_PollThread = ::CreateThread (0, 0, Ipc::_Poll, NULL,
								                 0, &m_PollThreadID);

  ::SetThreadPriority (m_PollThread, THREAD_PRIORITY_NORMAL);
}

//=====================================================
void Ipc::Poll ()
{
  bool bLoop = true;
  while (bLoop) {
    //....
    Sleep (1000);
    //if (m_SharedStruct->nInfoWrote - m_SharedStruct->nInfoRead > 0) {
    //  (*ms_fnOnDataChange) ();
    //}
    //....

    //bLoop = false;
  }
}

//=====================================================
void Ipc::SetOnDataChange (TYPEOF_fnOnDataChange func)
{
  ms_fnOnDataChange = func;
}

//=====================================================
SharedStruct<IpcStruct> *Ipc::GetSharedStruct ()
{
  return &m_SharedStruct;
}

//=====================================================
/*static*/ DWORD WINAPI Ipc::_Poll (void *pData)
{
  Ipc::Get ()->Poll ();
  return 0;
}
