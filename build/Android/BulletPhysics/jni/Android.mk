# This project file has been automatically generated.
# Manual edition is not recommended.

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := BulletPhysics

LOCAL_SRC_FILES := \
    ../../../../code/plugin/BulletPhysics/Module.cpp \
    ../../../../code/plugin/BulletPhysics/stdafx.cpp 


LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/../../../../code/ \
		$(LOCAL_PATH)/../../../../code/plugin/ \
		$(LOCAL_PATH)/../../../../extern/Bullet-2.81/src/ \
		$(LOCAL_PATH)/../../../../extern/oisInput/Include/ \
		

include $(BUILD_STATIC_LIBRARY)






