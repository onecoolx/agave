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

#ifndef _VGCL_PLATFORM_H_
#define _VGCL_PLATFORM_H_

#include <pthread.h>

namespace vgcl {

/* Thread functions. */
#define InvalidThreadHandle NULL

#define CALLBACK_TYPE

struct posix_thread {
    pthread_t thread;
    pthread_cond_t cond;
    pthread_mutex_t lock;
    int suspend;
};

typedef struct posix_thread* PlatformThreadHandle;

typedef void* PlatformThreadResult;

typedef void* PlatformThreadArg;

typedef PlatformThreadResult (*PlatformThreadFunc)(PlatformThreadArg);

PlatformThreadHandle CreatePlatformThread(PlatformThreadFunc func, void* ThreadHost);

void TrySuspendPlatformThread(PlatformThreadHandle handle);

void KillPlatformThread(PlatformThreadHandle, PlatformThreadResult exit);

void CloseThreadHandle(PlatformThreadHandle);

bool SuspendPlatformThread(PlatformThreadHandle);

bool ResumePlatformThread(PlatformThreadHandle);

void PlatformSleep(int miliSeconds);

/* Mutex function. */

typedef pthread_mutex_t PlatformMutex;
typedef PlatformMutex* PlatformMutexHandle;

void InitPlatformMutex(PlatformMutexHandle);

void DestroyPlatformMutex(PlatformMutexHandle);

bool LockPlatformMutex(PlatformMutexHandle);

bool TryLockPlatformMutex(PlatformMutexHandle);

bool UnLockPlatformMutex(PlatformMutexHandle);

}

#endif /*_VGCL_PLATFORM_H_*/
