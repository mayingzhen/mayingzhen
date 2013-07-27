# This project file has been automatically generated.
# Manual edition is not recommended.

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := Engine


LOCAL_SRC_FILES := \
    ../../../../code/Engine/Module.cpp \
    ../../../../code/Engine/stdAfx.cpp \


LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../../../code/ \
	$(LOCAL_PATH)/../../../../extern/oisInput/Include


include $(BUILD_STATIC_LIBRARY)



