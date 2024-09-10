/**
 *	VPlatform.cpp - Vector graphic component library.
 *
 *	Copyright (C) 2010 Zhang Ji Peng
 *	Contact: onecoolx@gmail.com
 */

#include "vgconfig.h"
#include "vplatform.h"

namespace VGCL {

void KillPlatformThread(PlatformThreadHandle handle, PlatformThreadResult exit)
{
	TerminateThread(handle, exit);
}

void CloseThreadHandle(PlatformThreadHandle handle)
{
	CloseHandle(handle);
}

PlatformThreadHandle CreatePlatformThread(PlatformThreadFunc func, void* ThreadHost)
{
	return CreateThread(NULL, 0, func, ThreadHost, CREATE_SUSPENDED, 0);
}

bool SuspendPlatformThread(PlatformThreadHandle handle)
{
    return SuspendThread(handle) == (DWORD)(-1) ? false : true;
}

bool ResumePlatformThread(PlatformThreadHandle handle)
{
    return ResumeThread(handle) == (DWORD)(-1) ? false : true;
}

void PlatformSleep(int miliSeconds)
{
    Sleep(miliSeconds);
}

void InitPlatformMutex(PlatformMutexHandle* m)
{
    InitializeCriticalSection(m);
}

void DestroyPlatformMutex(PlatformMutexHandle* m)
{
    DeleteCriticalSection(m);
}

bool LockPlatformMutex(PlatformMutexHandle* m)
{
    EnterCriticalSection(m);
    return true;
}

bool TryLockPlatformMutex(PlatformMutexHandle* m)
{
#if(_WIN32_WINNT >= 0x0400)
    return TryEnterCriticalSection(m) ? true : false;
#else
	return false;
#endif
}

bool UnLockPlatformMutex(PlatformMutexHandle* m)
{
    LeaveCriticalSection(m);
    return true;
}

}
