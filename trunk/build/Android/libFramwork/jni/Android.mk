# This project file has been automatically generated.
# Manual edition is not recommended.

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := Framwork

LOCAL_SRC_FILES := \
		../../../../code/Framwork/Module.cpp \
    ../../../../code/Framwork/stdafx.cpp \

LOCAL_C_INCLUDES := \
			$(LOCAL_PATH)/../../../../code/ \
			$(LOCAL_PATH)/../../../../extern/oisInput/Include
			


include $(BUILD_STATIC_LIBRARY)

