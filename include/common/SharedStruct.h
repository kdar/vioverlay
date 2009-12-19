#if !defined(AFX_SharedStruct_H__86467BA6_5AFA_11D3_863D_00A0244A9CA7__INCLUDED_)
#define AFX_SharedStruct_H__86467BA6_5AFA_11D3_863D_00A0244A9CA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string.h>
#include <stdlib.h>

#include "Aclapi.h"

//-----------------------------------------------------
template <class StructType>
class SharedStruct
{
  public:
    SharedStruct();
    SharedStruct(char *szName);
    ~SharedStruct();
    
    bool Acquire(char *szName);
    void Release();

    bool Lock();
    void Unlock();

    StructType *operator->();
    StructType *GetData() const;

  private:
    HANDLE m_hFileMapping;
    char   m_szSharedName[MAX_PATH];
    DWORD  m_dwMaxDataSize;
    StructType *m_pvData;

    HANDLE m_mutex;

    bool m_bCreated;  
};


//=====================================================
template <class StructType>
SharedStruct<StructType>::SharedStruct()
{
  m_hFileMapping = NULL;
  m_mutex = NULL;
}

//=====================================================
template <class StructType>
SharedStruct<StructType>::SharedStruct(char *szName)
{
  SharedStruct ();
  Acquire (szName);
}

//=====================================================
template <class StructType>
SharedStruct<StructType>::~SharedStruct()
{
  Release ();
}

//=====================================================
template <class StructType>
bool SharedStruct<StructType>::Acquire(char *szName)
{
  try {
    m_mutex = ::CreateMutex(NULL, FALSE, "aioverlay_dxhook01");
    if (!m_mutex) {
			throw __LINE__;
		}

    m_dwMaxDataSize = 0;
    m_hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(StructType), szName);
    if (m_hFileMapping == NULL) {
      throw __LINE__;
    }

    SetNamedSecurityInfo(szName, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, 0, 0, (PACL)NULL, NULL);

    m_dwMaxDataSize = sizeof(StructType);
    strncpy_s (m_szSharedName, MAX_PATH - 1, szName, MAX_PATH - 1);

    m_pvData = (StructType *)MapViewOfFile(m_hFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
    if (m_pvData == NULL) {
      CloseHandle(m_hFileMapping);
      throw __LINE__;
    }
  } catch (...) {
    return FALSE;
  }

  return TRUE;
}

//=====================================================
template <class StructType>
void SharedStruct<StructType>::Release()
{
  if (m_mutex)
	  ::ReleaseMutex(m_mutex);

  if (m_pvData) {
    UnmapViewOfFile(m_pvData);
    m_pvData = NULL;
  }

  if (m_hFileMapping) {
    CloseHandle(m_hFileMapping);
  }
}

//=====================================================
template <class StructType>
bool SharedStruct<StructType>::Lock()
{
	if (m_mutex) {
	  if (::WaitForSingleObject(m_mutex, 100) == WAIT_OBJECT_0) 
	    return true;
	  else
	    return false;
  }
	return false;
}

//=====================================================
template <class StructType>
void SharedStruct<StructType>::Unlock()
{
	if (m_mutex) {
		::ReleaseMutex(m_mutex);
	}	
}

//=====================================================
template <class StructType>
StructType *SharedStruct<StructType>::operator->()
{
  return m_pvData;
}

//=====================================================
template <class StructType>
StructType *SharedStruct<StructType>::GetData() const
{
  return m_pvData;
}

#endif
