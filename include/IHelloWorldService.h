#ifndef ANDROID_IHELLOWORLDSERVICE_H
#define ANDROID_IHELLOWORLDSERVICE_H

#include <binder/IInterface.h>

namespace android {

class IHelloWorldService : public IInterface
{
public:
    DECLARE_META_INTERFACE(HelloWorldService);

    virtual sp<IHelloWorld> connect(const sp<IHelloWorldClient>& helloWorldClient) = 0;
};

class BnHelloWorldService : public BnInterface<IHelloWorldService>
{
public:
    virtual status_t onTransact( uint32_t code,
                                 const Parcel & data,
                                 Parcel * reply,
                                 uint32_t flags);
};

};

#endif
