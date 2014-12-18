LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/../include

LOCAL_SRC_FILES:= \
    main_client.cpp

LOCAL_SHARED_LIBRARIES := \
    libutils \
    libcutils \
    libbinder \
    liblog \
    libhelloworld

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= myclient

include $(BUILD_EXECUTABLE)
