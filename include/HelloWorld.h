#ifndef ANDROID_HELLOWORLD_H
#define ANDROID_HELLOWORLD_H

#include "IHelloWorld.h"
#include "IHelloWorldClient.h"
#include "IHelloWorldService.h"

namespace android {

class HelloWorld : public BnHelloWorldClient, public IBinder::DeathRecipient
{
public:
    static sp<HelloWorld> connect();

    status_t setHelloWorld();
    void disconnect();

    virtual     ~HelloWorld();

    virtual void notifyCallback(int32_t msgType, int32_t ext, int32_t ext2);

    virtual status_t onTransact( uint32_t code,
                                 const Parcel & data,
                                 Parcel * reply,
                                 uint32_t flags);

private:
    HelloWorld();
    static const sp<IHelloWorldService>& getHelloWorldService(); //get helloWorld service handler

    virtual void binderDied(const wp<IBinder>& who);

    class DeathNotifier: public IBinder::DeathRecipient
    {
    public:
        DeathNotifier() {
        }

        virtual void binderDied(const wp<IBinder>& who);
    };

    static sp<DeathNotifier> mDeathNotifier;

    sp<IHelloWorld> mHelloWorld;
    static sp<IHelloWorldService> mHelloWorldService;
    static Mutex mLock;

};

};

#endif

