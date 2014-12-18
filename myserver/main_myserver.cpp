#define LOG_TAG "myserver"

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <utils/Log.h>

#include "HelloWorldService.h"

using namespace android;

int main(int argc, char** argv)
{
    HelloWorldService::instantiate();
    ProcessState::self()->startThreadPool();
    printf("my server is starting now.\n");
    IPCThreadState::self()->joinThreadPool();
    return 0;
}

