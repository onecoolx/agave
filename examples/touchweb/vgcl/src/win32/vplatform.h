/**
 *	VPlatform.h - Vector graphic component library.
 *
 *	Copyright (C) 2010 Zhang Ji Peng
 *	Contact: onecoolx@gmail.com
 */

#ifndef _VGCL_VPLATFORM_H_
#define _VGCL_VPLATFORM_H_

#include <windows.h>

namespace VGCL {

/* Thread functions. */
#define InvalidThreadHandle NULL

typedef HANDLE PlatformThreadHandle;

typedef DWORD PlatformThreadResult;

typedef LPVOID PlatformThreadArg; 

typedef PlatformThreadResult (WINAPI *PlatformThreadFunc)(PlatformThreadArg);


PlatformThreadHandle CreatePlatformThread(PlatformThreadFunc func, void* ThreadHost);

void KillPlatformThread(PlatformThreadHandle, PlatformThreadResult exit);

void CloseThreadHandle(PlatformThreadHandle);

bool SuspendPlatformThread(PlatformThreadHandle);

bool ResumePlatformThread(PlatformThreadHandle);

void PlatformSleep(int miliSeconds);

/* Mutex function. */

typedef CRITICAL_SECTION PlatformMutexHandle;

void InitPlatformMutex(PlatformMutexHandle*);

void DestroyPlatformMutex(PlatformMutexHandle*);

bool LockPlatformMutex(PlatformMutexHandle*);

bool TryLockPlatformMutex(PlatformMutexHandle*);

bool UnLockPlatformMutex(PlatformMutexHandle*);
}

#endif /*_VGCL_VPLATFORM_H_*/
