# This project file has been automatically generated.
# Manual edition is not recommended.

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := S3AUTRendererC3

LOCAL_SRC_FILES := \
    ../../../code/RendererC3/MeshC3.cpp \
    ../../../code/RendererC3/RendererC3.cpp \
    ../../../code/RendererC3/RendererC3Module.cpp \
    ../../../code/RendererC3/RendererC3PCH.cpp \
    ../../../code/RendererC3/RenderQueueC3.cpp \
    ../../../code/RendererC3/ShaderC3.cpp \
    ../../../code/RendererC3/TextureC3.cpp \
    ../../../code/RendererC3/XMC3Effect.cpp \
    ../../../code/RendererC3/XMC3ResourceModel.cpp \
    ../../../code/RendererC3/XMC3SimpleObj.cpp \


LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/../../../code/ \
		$(LOCAL_PATH)/../../../extern/Android/ \
		$(LOCAL_PATH)/../../../extern/Android/C3/ \
		$(LOCAL_PATH)/../../../extern/include/ \
		$(LOCAL_PATH)/../../../extern/include/gl/ \
		$(LOCAL_PATH)/../../../extern/DevIL_1.6/include/
		
LOCAL_LDLIBS := -lGLESv2 -llog -ldl -landroid -lEGL		

#LOCAL_STATIC_LIBRARIES := S3ACommon S3Animation S3Locomotion S3AUT \
#	Graphic GraphicData Role3D C3Core DataThread DevIL Freetype iconv jpeg PNG RoleView Tqpackage zlib

include $(BUILD_STATIC_LIBRARY)






