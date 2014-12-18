#include <utils/Log.h>
#include <utils/Mutex.h>
#include "HelloWorldService.h"
#include "IHelloWorldClient.h"

namespace android {

enum {
    HW_CALLBACK_NOTICE = 0x1000,
};

Mutex HelloWorldService::mServiceLock;
int HelloWorldService::mClientId;

sp<IHelloWorld> HelloWorldService::connect(const sp<IHelloWorldClient>& helloWorldClient)
{
    sp<Client> client;

    Mutex::Autolock lock(mServiceLock);

    if (mClient[mClientId] != 0) 
    {
        client = mClient[mClientId].promote();
        if (client != 0)
        {
            if (helloWorldClient->asBinder() == client->getHelloWorldClient()->asBinder()) 
            {
                printf("helloWorldClient is existed\n");
                return client;
            } 
            else 
            {
                return NULL;
            }
       }
       mClient[mClientId].clear();
   }

   client = new Client(this, helloWorldClient);

   helloWorldClient->asBinder()->linkToDeath(this);

   mClient[mClientId] = client;
   mClientId++;

   return client;
}

void HelloWorldService::removeClient(const sp<IHelloWorldClient>& helloWorldClient)
{
    Mutex::Autolock lock(mServiceLock);

    int outIndex = 0;
    sp<Client> client = findClientUnsafe(helloWorldClient->asBinder(), outIndex);

    if (client != 0) 
    {
        // Found our helloWorld, clear and leave.
        mClient[outIndex].clear();
        client->unlinkToDeath(this);
    }
}

sp<HelloWorldService::Client> HelloWorldService::findClientUnsafe(const wp<IBinder>& helloWorldClient, int& outIndex)
{
    sp<Client> client;

    for (int i = 0; i <= mClientId; i++) 
    {

        // This happens when we have already disconnected (or this is
        // just another unused camera).
        if (mClient[i] == 0) continue;

        // Promote mClient. It can fail if we are called from this path:
        // Client::~Client() -> disconnect() -> removeClient().
        client = mClient[i].promote();

        // Clean up stale client entry
        if (client == NULL) {
            mClient[i].clear();
            continue;
        }

        if (helloWorldClient == client->getHelloWorldClient()->asBinder()) 
        {
            // Found our helloWorld
            outIndex = i;
            return client;
        }
    }

    outIndex = -1;
    return NULL;
}

HelloWorldService::HelloWorldService()
    : BnHelloWorldService()
{
    printf("HelloWorldService is created\n");
}

HelloWorldService::~HelloWorldService()
{
    printf("HelloWorldService is destroyed\n");
}

status_t HelloWorldService::onTransact( uint32_t code, const Parcel & data, Parcel * reply, uint32_t flags)
{
    printf("HelloWorldService onTransact\n");

    return BnHelloWorldService::onTransact(code, data, reply, flags);
}

HelloWorldService::Client::Client(const sp<HelloWorldService>& helloWorldService, const sp<IHelloWorldClient>& helloWorldClient)
{
    mHelloWorldService = helloWorldService;
    mHelloWorldClient = helloWorldClient;
}

HelloWorldService::Client::~Client() 
{
    // unconditionally disconnect. function is idempotent
    Client::disconnect();
}

void HelloWorldService::Client::disconnect()
{
    printf("service disconnect.\n");

    mHelloWorldService->removeClient(mHelloWorldClient);
}

status_t HelloWorldService::Client::helloWorld(const char *str)
{
    printf("service get:%s\n", str);

    Mutex::Autolock lock(mServiceLock);

    sp<IHelloWorldClient> c= mHelloWorldClient;

    if(c != NULL)
    {
        c->notifyCallback(HW_CALLBACK_NOTICE, 0, 0);
    }
    
    return NO_ERROR;
}

sp<HelloWorldService::Client> HelloWorldService::getClientByRemote(const wp<IBinder>& helloworldClient)
{
    sp<Client> client;

    Mutex::Autolock lock(mServiceLock);

    int outIndex;
    client = findClientUnsafe(helloworldClient, outIndex);

    return client;

}

void HelloWorldService::binderDied(const wp<IBinder> &who)
{
    printf("java clients' binder died\n");

    sp<Client> helloWorldClient = getClientByRemote(who);

    if (helloWorldClient == 0)
    {
        printf("java clients' binder death already cleaned up (normal case)\n");
        return;
    }

    helloWorldClient->disconnect();
}

};
