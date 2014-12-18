#ifndef ANDROID_IHELLOWORLDCLIENT_H
#define ANDROID_IHELLOWORLDCLIENT_H

#include <binder/IInterface.h>

namespace android {

class IHelloWorldClient : public IInterface
{
public:
    DECLARE_META_INTERFACE(HelloWorldClient);

    virtual void notifyCallback(int32_t msgType, int32_t ext1, int32_t ext2) = 0;
};

class BnHelloWorldClient : public BnInterface<IHelloWorldClient>
{
public:
    virtual status_t onTransact( uint32_t code,
                                 const Parcel & data,
                                 Parcel * reply,
                                 uint32_t flags);
};

};

#endif

