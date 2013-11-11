# This project file has been automatically generated.
# Manual edition is not recommended.

LOCAL_PATH := $(call my-dir)



include $(CLEAR_VARS)

LOCAL_MODULE := SampleBrowser
				

LOCAL_SRC_FILES := \
		../../../../code/Samples/main-windows.cpp \
		../../../../code/Samples/SampleBrowser.cpp \
		../../../../code/Samples/stdafx.cpp\

LOCAL_C_INCLUDES := \
			$(LOCAL_PATH)/../../../../code/ \
			$(LOCAL_PATH)/../../../../code/plugin \
			$(LOCAL_PATH)/../../../../extern/oisInput/Include


LOCAL_LDLIBS := -lGLESv2 -llog -ldl -landroid -lEGL

LOCAL_STATIC_LIBRARIES := Common Engine Render GLESRender BulletPhysics Animation UI
  

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := Common
LOCAL_SRC_FILES := ../../lib/libCommon.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := Engine
LOCAL_SRC_FILES := ../../lib/libEngine.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := Render
LOCAL_SRC_FILES := ../../lib/libRender.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := GLESRender
LOCAL_SRC_FILES := ../../lib/libGLESRender.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := BulletPhysics
LOCAL_SRC_FILES := ../../lib/libBulletPhysics.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := Animation
LOCAL_SRC_FILES := ../../lib/libAnimation.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := UI
LOCAL_SRC_FILES := ../../lib/libUI.a
include $(PREBUILT_STATIC_LIBRARY)

