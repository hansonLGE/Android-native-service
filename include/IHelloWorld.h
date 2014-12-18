#ifndef ANDROID_IHELLOWORLD_H
#define ANDROID_IHELLOWORLD_H

#include <binder/IInterface.h>

namespace android {

class IHelloWorld : public IInterface
{
public:
    DECLARE_META_INTERFACE(HelloWorld);

    virtual status_t helloWorld(const char *str) = 0;
    virtual void disconnect() = 0;
};

class BnHelloWorld : public BnInterface<IHelloWorld>
{
public:
    virtual status_t onTransact( uint32_t code,
                                 const Parcel & data,
                                 Parcel * reply,
                                 uint32_t flags);
};

};

#endif

