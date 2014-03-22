# This project file has been automatically generated.
# Manual edition is not recommended.

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := Application

LOCAL_SRC_FILES := \
    ../../../../code/Application/Module.cpp \
    ../../../../code/Application/stdafx.cpp 


LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/../../../../code/ \
		$(LOCAL_PATH)/../../../../code/plugin/ \
		$(LOCAL_PATH)/../../../../extern/oisInput/Include/ \
		

include $(BUILD_STATIC_LIBRARY)






