/**
 *	VThread.h - Vector graphic component library.
 *
 *	Copyright (C) 2010 Zhang Ji Peng
 *	Contact: onecoolx@gmail.com
 */

#ifndef _VGCL_VTHREAD_H_
#define _VGCL_VTHREAD_H_

#include "vobject.h"
#include "vplatform.h"

namespace VGCL {

class VThreadPriv;

class VThread : public VObject
{
public:
    VThread();
    virtual ~VThread();

	virtual void Run(void) = 0;

	bool Start(void);
    bool StartOnce(void);
	bool Stop(void);
	bool isRunning(void) const;
	void Exit(int code = 0);

    static void Sleep(int ms);
protected:
	PlatformThreadHandle Handle(void);
private:
    VThread(const VThread&);
    VThread& operator=(const VThread&);

    VThreadPriv* m_data;
};


class VMutex : public VObject
{
public:
    VMutex();
    virtual ~VMutex();

    bool TryLock(void);
    bool Lock(void);
    bool UnLock(void);

private:
    VMutex(const VMutex&);
    VMutex& operator=(const VMutex&);

    PlatformMutexHandle m_handle;
};


template<typename T>
class VMutexLocker : public VObject
{
public:
    VMutexLocker(T& mutex)
        : m_mutex(mutex)
    {
        m_mutex.Lock(); 
    }

    virtual ~VMutexLocker()
    {
        m_mutex.UnLock();
    }

private:
    VMutexLocker(const VMutexLocker&);
    VMutexLocker& operator=(const VMutexLocker&);

    T& m_mutex;
};

}

#endif /*_VGCL_VTHREAD_H_*/
