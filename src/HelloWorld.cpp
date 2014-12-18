#include <utils/Log.h>
#include <utils/Mutex.h>
#include <binder/IServiceManager.h>
#include "IHelloWorld.h"
#include "IHelloWorldClient.h"
#include "IHelloWorldService.h"
#include "HelloWorld.h"

namespace android {

Mutex HelloWorld::mLock;
sp<IHelloWorldService> HelloWorld::mHelloWorldService;
sp<HelloWorld::DeathNotifier> HelloWorld::mDeathNotifier;

const sp<IHelloWorldService>& HelloWorld::getHelloWorldService()
{
    Mutex::Autolock lock(mLock);

    if(mHelloWorldService.get() == 0)
    {
        sp<IServiceManager> sm = defaultServiceManager();
        sp<IBinder> binder;

        do {
            binder = sm->getService(String16("myserver.hello_world_service"));
            if(binder != 0)
            {
                break;
            }
            printf("HelloWorld Service is not working, waiting...\n");
            usleep(500000);
        } while(true);

        if (mDeathNotifier == NULL)
        {
            mDeathNotifier = new DeathNotifier();
        }
        binder->linkToDeath(mDeathNotifier);

        mHelloWorldService = interface_cast<IHelloWorldService>(binder);
    }

    return mHelloWorldService;
}

sp<HelloWorld> HelloWorld::connect()
{
    sp<HelloWorld> c = new HelloWorld();
    const sp<IHelloWorldService>& hw = getHelloWorldService();

    if(hw != 0)
    {
        c->mHelloWorld = hw->connect(c);
    }

    if (c->mHelloWorld != 0) 
    {
        c->mHelloWorld->asBinder()->linkToDeath(c);
    } else 
    {
        c.clear();
    }

    return c;
}

status_t HelloWorld::setHelloWorld()
{
    sp <IHelloWorld> c = mHelloWorld;

    return c->helloWorld("hi, renwei");
}

void HelloWorld::disconnect()
{
    sp <IHelloWorld> c = mHelloWorld;

    if (c != 0)
    {
        c->disconnect();
        c->asBinder()->unlinkToDeath(this);
        c = 0;
    }
}

void HelloWorld::notifyCallback(int32_t msgType, int32_t ext, int32_t ext2)
{
    printf("client receive callback!\n");
}

status_t HelloWorld::onTransact( uint32_t code, const Parcel & data, Parcel * reply, uint32_t flags)
{
    printf("HelloWorld onTransact\n");

    return BnHelloWorldClient::onTransact(code, data, reply, flags);
}

HelloWorld::HelloWorld()
{

}

HelloWorld::~HelloWorld()
{

}

void HelloWorld::binderDied(const wp<IBinder>& who)
{
    printf("IHelloWorld died\n");

}

void HelloWorld::DeathNotifier::binderDied(const wp<IBinder>& who)
{
    Mutex::Autolock lock(mLock);
    HelloWorld::mHelloWorldService.clear();
    printf("HelloWorld service died!\n");
}

};

