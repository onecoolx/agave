
#include <stdio.h>
#include <gtest/gtest.h>
#include <vgcl.h>

using namespace vgcl;

VMutex mutex;
int i = 0;

void value_count(void)
{
    VMutexLocker<VMutex> locker(mutex);
    i++;
    fprintf (stderr, "value count %d\n", i);
}

class Thread : public vgcl::VThread
{
public:
    virtual void Run(void)
    {
        fprintf(stderr, "Run Thread [%d]\n", Handle());
        value_count();
        VThread::Sleep(1);
    }
};

TEST(vgcl_API, VThread)
{
    Thread* p1 = new Thread;
    Thread* p2 = new Thread;

    p1->Start();
    p2->StartOnce();
    fprintf(stderr, "start thread [%d]\n", p1);
    fprintf(stderr, "start thread [%d]\n", p2);
    vgcl::VThread::Sleep(30);
    p2->StartOnce();
    p1->Stop();
    fprintf(stderr, "stop thread [%d]\n", p1);
    fprintf(stderr, "stop thread [%d]\n", p2);

    vgcl::VThread::Sleep(30);

    if (p1->isRunning()) {
        fprintf(stderr, "thread still running [%d]\n", p1);
    }
    vgcl::VThread::Sleep(10);
    p1->Start();

    p1->Exit(0);

    vgcl::VThread::Sleep(30);
    while (p1->isRunning());

    delete p1;
    delete p2;

}
