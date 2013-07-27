#!/bin/sh

echo "进入当前Shell程序的目录"
cd `dirname $0`
pwd
echo "================================================================"


#指定"-r"编译Release版
if [ "$1" = "-r" ];then
echo "----编译Release版本的AnimMoudule"
export DEBUG_BUILD=0
else
echo "----编译Debug版本的AnimMoudule"
export DEBUG_BUILD=1
fi

#指定"-b"完全重新编译
export CLEAN_BUILD
if [ "$1" = "-b" -o "$2" = "-b" ];then
echo "----完全重新编译AnimMoudule"
export CLEAN_BUILD=-B
fi

cd lib
rm *.a
cd ..

echo "1.开始编译libCommon"
ndk-build NDK_DEBUG=$DEBUG_BUILD $CLEAN_BUILD -C libCommon
echo "================================================================"

echo "2.开始编译libEngine"
ndk-build NDK_DEBUG=$DEBUG_BUILD $CLEAN_BUILD -C libEngine
echo "================================================================"

echo "3.开始编译libFramwork"
ndk-build NDK_DEBUG=$DEBUG_BUILD $CLEAN_BUILD -C libFramwork
echo "================================================================"

echo "4.开始编译libAnimation"
ndk-build NDK_DEBUG=$DEBUG_BUILD $CLEAN_BUILD -C libAnimation
echo "================================================================"

echo "5.开始编译libGLESRender"
ndk-build NDK_DEBUG=$DEBUG_BUILD $CLEAN_BUILD -C libGLESRender
echo "================================================================"

echo "6.开始编译libBulletPhysics"
ndk-build NDK_DEBUG=$DEBUG_BUILD $CLEAN_BUILD -C libBulletPhysics
echo "================================================================"

echo "7.开始编译libUI"
ndk-build NDK_DEBUG=$DEBUG_BUILD $CLEAN_BUILD -C libUI
echo "================================================================"


echo "8.复制生成结果到lib目录"
cp libCommon/obj/local/armeabi/libCommon.a lib/
cp libEngine/obj/local/armeabi/libEngine.a lib/
cp libFramwork/obj/local/armeabi/libFramwork.a lib/
cp libAnimation/obj/local/armeabi/libAnimation.a lib/
cp libGLESRender/obj/local/armeabi/libGLESRender.a lib/
cp libBulletPhysics/obj/local/armeabi/libBulletPhysics.a lib/
cp libUI/obj/local/armeabi/libUI.a lib/

echo "9.复制Externlib"
cp ../../extern/Bullet-2.81/build/ndk/libPhyxBullet.a lib/
cp ../../extern/DevIL/lib/Android/libDevIL.a lib/
cp ../../extern/DevIL/lib/Android/libjpeg.a lib/
cp ../../extern/DevIL/lib/Android/libPNG.a lib/
cp ../../extern/DevIL/lib/Android/libzlib.a lib/
cp ../../extern/oisInput/Lib/Android/libOIS.a lib/
echo "================================================================"


cd lib
echo "10.开始合并libBulletPhysics.a libPhyxBullet.a"
ar -x libBulletPhysics.a
ar -x libPhyxBullet.a
ar cru libBulletPhysics2.a *.o
rm *.o
rm libPhyxBullet.a
mv libBulletPhysics2.a libBulletPhysics.a
ranlib libBulletPhysics.a

echo "11.开始合并libGLESRender.a libDevIL.a"
ar -x libGLESRender.a
ar -x libDevIL.a
ar -x libjpeg.a
ar -x libPNG.a
ar -x libzlib.a
ar cru libGLESRende2.a *.o
rm *.o
rm libDevIL.a libjpeg.a libPNG.a libzlib.a
mv libGLESRende2.a libGLESRender.a
ranlib libGLESRender.a

echo "12.开始合并libEngine.a libOIS.a"
ar -x libEngine.a
ar -x libOIS.a
ar cru libEngine2.a *.o
rm *.o
rm libOIS.a
mv libEngine2.a libEngine.a
ranlib libEngine.a



echo "================================================================"
echo "16.完成编译,可以关闭终端了."
