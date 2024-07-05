#!/bin/bash

## Run into root project folder
## Prerequisites
#export ANDROID_NDK_ROOT=$ANDROID_NDK
#export ANDROID_SDK_ROOT=$$SDK$$
#export ANDROID_API_VERSION=android-34
#export BUILD_FOR_ABI=$1
#export OPENSSL_NDK=$$SDK$$/ndk/21.0.6113669
#export BUILD_PATH=$(pwd)/build

#$1 = $$VERSION$$
#$2 = $$VERCODE$$
#$3 = $$Qt5$$
#$4 = $$NDK$$
#$5 = $$SDK$$
#$6 = android-34
#$7 = $$SDK$$/ndk/21.0.6113669
#$8 = $(pwd)/build

export ANDROID_NDK=$4
export ANDROID_NDK_ROOT=$4
export ANDROID_SDK_ROOT=$5
export ANDROID_API_VERSION=$6
export OPENSSL_NDK=$7
export BUILD_PATH=$8


cd external/android_openssl
./build_ssl.sh
echo "Building Qt Host"
mkdir $BUILD_PATH/build-qthost
cd $BUILD_PATH/build-qthost
$3/configure -prefix $BUILD_PATH/qthost-output -nomake tests -nomake examples -qt-libjpeg -opensource -confirm-license -DCMAKE_INSTALL_MESSAGE=never -DCMAKE_MESSAGE_LOG_LEVEL="WARNING"  > output.log
cmake --build . --parallel -- --quiet
set +x
cmake --install . >/dev/null
set -x
echo "Building Qt for Android for ${BUILD_FOR_ABI}"
mkdir $BUILD_PATH/build-qtandroid
cd $BUILD_PATH/build-qtandroid
echo "Cleaning unused build memory from Qt Host"
rm -rf $BUILD_PATH/build-qthost
$3/configure -prefix $BUILD_PATH/qtandroid-output -qt-host-path $BUILD_PATH/qthost-output -android-abis $BUILD_FOR_ABI -android-sdk $ANDROID_SDK_ROOT -android-ndk $ANDROID_NDK_ROOT -nomake tests -nomake examples -DCMAKE_INSTALL_MESSAGE=never -DCMAKE_MESSAGE_LOG_LEVEL="WARNING"  > output.log
cmake --build . --parallel -- --quiet
set +x
cmake --install . >/dev/null
set -x
echo "Building the application"
export Qt_DIR=$BUILD_PATH/qtandroid-output
export Qt6_DIR=$BUILD_PATH/qtandroid-output
export PATH=$Qt_DIR/bin:$PATH
cd $BUILD_PATH
$Qt_DIR/bin/qt-cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo --log-level=WARNING -DANDROID_VERSION_CODE=$2 -DANDROID_VERSION_NAME=$1
cmake --build . --target all --parallel
