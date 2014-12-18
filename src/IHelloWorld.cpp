#include <binder/Parcel.h>
#include "IHelloWorld.h"

namespace android {

enum {
    DISCONNECT = IBinder::FIRST_CALL_TRANSACTION,
    HW_HELLOWORLD,
};

class BpHelloWorld : public BpInterface<IHelloWorld>
{
public:
    BpHelloWorld(const sp<IBinder>& impl)
        : BpInterface<IHelloWorld>(impl)
    {
    }

    // disconnect from helloWorld service
    virtual void disconnect()
    {
        printf("bp send disconnect.\n");
        Parcel data, reply;
        data.writeInterfaceToken(IHelloWorld::getInterfaceDescriptor());
        remote()->transact(DISCONNECT, data, &reply);
    }

    virtual status_t helloWorld(const char *str)
    {
        printf("bp send helloWorld.\n");
        Parcel data, reply;
        data.writeInterfaceToken(IHelloWorld::getInterfaceDescriptor());
        data.writeCString(str);
        remote()->transact(HW_HELLOWORLD, data, &reply);
        return reply.readInt32();
    }

};

IMPLEMENT_META_INTERFACE(HelloWorld, "android.myserver.IHelloWorld");

status_t BnHelloWorld::onTransact( uint32_t code,
                                 const Parcel & data,
                                 Parcel * reply,
                                 uint32_t flags)
{
    switch(code)
    {
        case DISCONNECT: {
            printf("bn disconnect.\n");
            CHECK_INTERFACE(IHelloWorld,data,reply);
            disconnect();
            return NO_ERROR;
        } break;
        case HW_HELLOWORLD: {
            printf("bn get helloWorld.\n");
            CHECK_INTERFACE(IHelloWorld,data,reply);
            const char *str;
            str = data.readCString();
            reply->writeInt32(helloWorld(str));
            return NO_ERROR;
        } break;
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }
}

}

