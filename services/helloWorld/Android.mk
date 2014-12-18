LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/../../include

LOCAL_SRC_FILES:= \
    ../../src/IHelloWorldService.cpp \
    ../../src/IHelloWorldClient.cpp \
    ../../src/IHelloWorld.cpp \
    ../../src/HelloWorld.cpp \
    HelloWorldService.cpp

LOCAL_SHARED_LIBRARIES := \
    libutils \
    libcutils \
    libbinder \
    liblog

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= libhelloworld

include $(BUILD_SHARED_LIBRARY)
