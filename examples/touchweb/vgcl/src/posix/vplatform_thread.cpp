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

#include <unistd.h>
#include <stdlib.h>

#include "vgconfig.h"
#include "vplatform_thread.h"

namespace vgcl {

PlatformThreadHandle CreatePlatformThread(PlatformThreadFunc func, void* ThreadHost)
{
    posix_thread* thread = (posix_thread*)calloc(1, sizeof(posix_thread));
    pthread_create(&thread->thread, NULL, func, ThreadHost);
    pthread_cond_init(&thread->cond, NULL);
    pthread_mutex_init(&thread->lock, NULL);
    thread->suspend = 1;

    return thread;
}

void KillPlatformThread(PlatformThreadHandle handle, PlatformThreadResult exit)
{
    posix_thread* thread = handle;
    pthread_cancel(thread->thread);
    pthread_join(thread->thread, NULL);
}

void CloseThreadHandle(PlatformThreadHandle handle)
{
    posix_thread* thread = handle;
    pthread_cond_destroy(&thread->cond);
    pthread_mutex_destroy(&thread->lock);
    free(thread);
}

void TrySuspendPlatformThread(PlatformThreadHandle handle)
{
    posix_thread* thread = handle;
    pthread_mutex_lock(&thread->lock);
    if (thread->suspend == 1) {
        pthread_cond_wait(&thread->cond, &thread->lock);
    }
    pthread_mutex_unlock(&thread->lock);
}

bool SuspendPlatformThread(PlatformThreadHandle handle)
{
    posix_thread* thread = handle;
    pthread_mutex_lock(&thread->lock);
    thread->suspend = 1;
    pthread_mutex_unlock(&thread->lock);
    return true;
}

bool ResumePlatformThread(PlatformThreadHandle handle)
{
    posix_thread* thread = handle;
    pthread_mutex_lock(&thread->lock);
    thread->suspend = 0;
    pthread_cond_signal(&thread->cond);
    pthread_mutex_unlock(&thread->lock);
    return true;
}

void PlatformSleep(int miliSeconds)
{
    usleep(miliSeconds * 1000);
}

void InitPlatformMutex(PlatformMutexHandle)
{
}

void DestroyPlatformMutex(PlatformMutexHandle)
{
}

bool LockPlatformMutex(PlatformMutexHandle)
{
    return false;
}

bool TryLockPlatformMutex(PlatformMutexHandle)
{
    return false;
}

bool UnLockPlatformMutex(PlatformMutexHandle)
{
    return false;
}

}
