#ifndef ANDROID_HELLOWORLDSERVICE_H
#define ANDROID_HELLOWORLDSERVICE_H

#include <binder/Parcel.h>
#include <binder/BinderService.h>
#include "IHelloWorld.h"
#include "IHelloWorldClient.h"
#include "IHelloWorldService.h"

#define MAX_CLIENTS  5

namespace android {

class HelloWorldService :
    public BinderService<HelloWorldService>,
    public BnHelloWorldService,
    public IBinder::DeathRecipient
{
    friend class BinderService<HelloWorldService>; // for HelloWorldService()

public:
    static const char* getServiceName() { return "myserver.hello_world_service"; }

    void removeClient(const sp<IHelloWorldClient>& helloWorldClient);

    virtual sp<IHelloWorld> connect(const sp<IHelloWorldClient>& helloWorldClient);

    virtual status_t onTransact( uint32_t code,
                                 const Parcel & data,
                                 Parcel * reply,
                                 uint32_t flags);

    class Client : public BnHelloWorld
    {
    public:
        void disconnect();
        status_t helloWorld(const char *str);

        Client(const sp<HelloWorldService>& helloWorldService, const sp<IHelloWorldClient>& helloWorldClient);
        ~Client();

        const sp<IHelloWorldClient>& getHelloWorldClient()
        {
            return mHelloWorldClient;
        }

    protected:
        // these are initialized in the constructor.
        sp<HelloWorldService>               mHelloWorldService;
        sp<IHelloWorldClient>               mHelloWorldClient;
        int                                 mHelloWorldId;

    };

    virtual sp<Client> getClientByRemote(const wp<IBinder>& helloworldClient);

private:
    HelloWorldService();
    virtual ~HelloWorldService();
    sp<Client> findClientUnsafe(const wp<IBinder>& helloWorldClient, int& outIndex);

    static Mutex mServiceLock;
    static int mClientId;
    wp<Client> mClient[MAX_CLIENTS];

    // IBinder::DeathRecipient implementation
    virtual void binderDied(const wp<IBinder> &who);
};

};
#endif

