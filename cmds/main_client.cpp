#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include "HelloWorld.h"

using namespace android;

int main(int argc, char** argv)
{
    sp<HelloWorld> c =  HelloWorld::connect();
    c->setHelloWorld();

    ProcessState::self()->startThreadPool();
    printf("hello world client is starting now.\n");

    sleep(2);

    c->disconnect();

    IPCThreadState::self()->joinThreadPool();

    return 0;
}


