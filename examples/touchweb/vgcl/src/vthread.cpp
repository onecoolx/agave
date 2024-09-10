/**
 *	VThread.cpp - Vector graphic component library.
 *
 *	Copyright (C) 2010 Zhang Ji Peng
 *	Contact: onecoolx@gmail.com
 */

#include "vgconfig.h"
#include "vthread.h"

namespace VGCL {

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
			KillPlatformThread(handle, -1);
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


static PlatformThreadResult WINAPI ThreadRoutine(PlatformThreadArg host)
{
	VThreadPriv * thread = (VThreadPriv*)host;
	
	while (thread->runInternal())
        ;//run loop

	CloseThreadHandle(thread->handle);
	thread->handle = InvalidThreadHandle;
	return thread->retval;
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
	if (ret) m_data->isRunning = true;
	return ret;
}

bool VThread::StartOnce(void)
{
    m_data->needLoop = false;
	PlatformThreadHandle thandle = Handle();
	bool ret = ResumePlatformThread(thandle);
	if (ret) m_data->isRunning = true;
	return ret;
}

bool VThread::Stop(void)
{
	PlatformThreadHandle thandle = Handle();
	bool ret = SuspendPlatformThread(thandle);
	if (ret) m_data->isRunning = false;
	return ret;
}

PlatformThreadHandle VThread::Handle(void)
{
	if (m_data->handle == InvalidThreadHandle)
		m_data->handle = CreatePlatformThread(ThreadRoutine, m_data);

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
