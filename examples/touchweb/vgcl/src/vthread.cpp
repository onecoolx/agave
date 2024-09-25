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
#include "vthread.h"

namespace vgcl {

// VThread
class VThreadPriv
{
public:
    VThreadPriv(VThread* thread)
        : thread(thread)
        , handle(InvalidThreadHandle)
        , isRunning(false)
        , needLoop(false)
        , retval(0)
    {
    }

    ~VThreadPriv()
    {
        if (handle != InvalidThreadHandle) {
            KillPlatformThread(handle, (PlatformThreadResult) - 1);
            CloseThreadHandle(handle);
            handle = InvalidThreadHandle;
            isRunning = false;
        }
    }

    bool runInternal(void);

    VThread* thread;
    PlatformThreadHandle handle;
    bool isRunning;
    bool needLoop;
    int retval;
};

bool VThreadPriv::runInternal(void)
{
    isRunning = true;
    thread->Run();
    isRunning = false;
    return needLoop;
}

static PlatformThreadResult CALLBACK_TYPE ThreadRoutine(PlatformThreadArg host)
{
    VThreadPriv* thread = (VThreadPriv*)host;

    while (thread->runInternal())
        ;//run loop

    CloseThreadHandle(thread->handle);
    thread->handle = InvalidThreadHandle;
    return (PlatformThreadResult)thread->retval;
}

VThread::VThread()
    : m_data(0)
{
    m_data = new VThreadPriv(this);
}

VThread::~VThread()
{
    delete m_data;
}

bool VThread::Start(void)
{
    m_data->needLoop = true;
    PlatformThreadHandle thandle = Handle();
    bool ret = ResumePlatformThread(thandle);
    if (ret) { m_data->isRunning = true; }
    return ret;
}

bool VThread::StartOnce(void)
{
    m_data->needLoop = false;
    PlatformThreadHandle thandle = Handle();
    bool ret = ResumePlatformThread(thandle);
    if (ret) { m_data->isRunning = true; }
    return ret;
}

bool VThread::Stop(void)
{
    PlatformThreadHandle thandle = Handle();
    bool ret = SuspendPlatformThread(thandle);
    if (ret) { m_data->isRunning = false; }
    return ret;
}

PlatformThreadHandle VThread::Handle(void)
{
    if (m_data->handle == InvalidThreadHandle) {
        m_data->handle = CreatePlatformThread(ThreadRoutine, m_data);
    }

    return m_data->handle;
}

void VThread::Sleep(int ms)
{
    PlatformSleep(ms);
}

bool VThread::isRunning(void) const
{
    return m_data->isRunning;
}

void VThread::Exit(int c)
{
    m_data->retval = c;
    m_data->needLoop = false;
}

//VMutex
VMutex::VMutex()
{
    InitPlatformMutex(&m_handle);
}

VMutex::~VMutex()
{
    DestroyPlatformMutex(&m_handle);
}

bool VMutex::TryLock(void)
{
    return TryLockPlatformMutex(&m_handle);
}

bool VMutex::Lock(void)
{
    return LockPlatformMutex(&m_handle);
}

bool VMutex::UnLock(void)
{
    return UnLockPlatformMutex(&m_handle);
}

}
