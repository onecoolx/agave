/*
 * VGCL - Vector graphic component library.
 *
 * Copyright (c) 2010, Zhang Ji Peng (onecoolx@gmail.com)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "vgconfig.h"
#include "vplatform_thread.h"

namespace vgcl {

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

void TrySuspendPlatformThread(PlatformThreadHandle handle)
{
    // not needed
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

void InitPlatformMutex(PlatformMutexHandle m)
{
    InitializeCriticalSection(m);
}

void DestroyPlatformMutex(PlatformMutexHandle m)
{
    DeleteCriticalSection(m);
}

bool LockPlatformMutex(PlatformMutexHandle m)
{
    EnterCriticalSection(m);
    return true;
}

bool TryLockPlatformMutex(PlatformMutexHandle m)
{
#if(_WIN32_WINNT >= 0x0400)
    return TryEnterCriticalSection(m) ? true : false;
#else
    return false;
#endif
}

bool UnLockPlatformMutex(PlatformMutexHandle m)
{
    LeaveCriticalSection(m);
    return true;
}

}
