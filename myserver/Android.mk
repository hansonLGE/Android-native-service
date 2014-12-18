LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/../include \
    $(LOCAL_PATH)/../services/helloWorld

LOCAL_SRC_FILES:= \
    main_myserver.cpp

LOCAL_SHARED_LIBRARIES := \
    libutils \
    libcutils \
    libbinder \
    liblog \
    libhelloworld 

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= myserver

include $(BUILD_EXECUTABLE)
