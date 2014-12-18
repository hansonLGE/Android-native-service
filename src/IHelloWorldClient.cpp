#include <binder/Parcel.h>
#include "IHelloWorldClient.h"

namespace android {

enum {
    NOTIFY_CALLBACK = IBinder::FIRST_CALL_TRANSACTION,
};

class BpHelloWorldClient : public BpInterface<IHelloWorldClient>
{
public:
    BpHelloWorldClient(const sp<IBinder>& impl)
        : BpInterface<IHelloWorldClient>(impl)
    {
    }

    // generic callback from helloWorld service to app
    virtual void notifyCallback(int32_t msgType, int32_t ext1, int32_t ext2)
    {
        printf("bp notifyCallback.\n");
        Parcel data, reply;
        data.writeInterfaceToken(IHelloWorldClient::getInterfaceDescriptor());
        data.writeInt32(msgType);
        data.writeInt32(ext1);
        data.writeInt32(ext2);
        remote()->transact(NOTIFY_CALLBACK, data, &reply, IBinder::FLAG_ONEWAY);
    }   
};

IMPLEMENT_META_INTERFACE(HelloWorldClient, "android.myserver.IHelloWorldClient");

status_t BnHelloWorldClient::onTransact( uint32_t code,
                                 const Parcel & data,
                                 Parcel * reply,
                                 uint32_t flags)
{
    switch(code)
    {
        case NOTIFY_CALLBACK: {
            printf("bn notifyCallback.\n");
            CHECK_INTERFACE(IHelloWorldClient,data,reply);
            int32_t msgType = data.readInt32();
            int32_t ext1 = data.readInt32();
            int32_t ext2 = data.readInt32();
            notifyCallback(msgType, ext1, ext2);
            return NO_ERROR;
        } break;
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }
}

}

