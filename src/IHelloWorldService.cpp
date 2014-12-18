#include <binder/Parcel.h>
#include "IHelloWorld.h"
#include "IHelloWorldClient.h"
#include "IHelloWorldService.h"

namespace android {

enum {
    HW_CONNECT = IBinder::FIRST_CALL_TRANSACTION,
};

class BpHelloWorldService : public BpInterface<IHelloWorldService>
{
public:
    BpHelloWorldService(const sp<IBinder>& impl)
        : BpInterface<IHelloWorldService>(impl)
    {
    }

    virtual sp<IHelloWorld> connect(const sp<IHelloWorldClient>& helloWorldClient)
    {
        printf("bp send connect.\n");
        Parcel data, reply;
        data.writeInterfaceToken(IHelloWorldService::getInterfaceDescriptor());    
        data.writeStrongBinder(helloWorldClient->asBinder());
        remote()->transact(HW_CONNECT, data, &reply);
        return interface_cast<IHelloWorld>(reply.readStrongBinder());
    }
};

IMPLEMENT_META_INTERFACE(HelloWorldService, "android.myserver.IHelloWorldService");

status_t BnHelloWorldService::onTransact( uint32_t code,
                                 const Parcel & data,
                                 Parcel * reply,
                                 uint32_t flags)
{
    switch(code)
    {
        case HW_CONNECT: {
            printf("bn get connect.\n");
            CHECK_INTERFACE(IHelloWorldService,data,reply);
            sp<IHelloWorldClient> helloWorldClient = interface_cast<IHelloWorldClient>(data.readStrongBinder());
            sp<IHelloWorld> helloworld = connect(helloWorldClient);
            reply->writeStrongBinder(helloworld->asBinder());
            return NO_ERROR;
        } break;
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }
}

};
