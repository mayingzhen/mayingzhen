# This project file has been automatically generated.
# Manual edition is not recommended.

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := GLESRender

LOCAL_SRC_FILES := \
    ../../../../code/Plugin/GLESRender/Module.cpp \
    ../../../../code/Plugin/GLESRender/stdafx.cpp \

LOCAL_CFLAGS := -DGL_GLEXT_PROTOTYPES=1

LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/../../../../code/ \
		$(LOCAL_PATH)/../../../../code/Plugin/ \
		$(LOCAL_PATH)/../../../../extern/oisInput/Include \
		$(LOCAL_PATH)/../../../../extern/DevIL/include 

		
include $(BUILD_STATIC_LIBRARY)






