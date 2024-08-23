LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_BUILTIN_APPNAME := sensor_app
LOCAL_BUILTIN_STACKSIZE := 2048

LOCAL_SRC_FILES :=
LOCAL_SRC_FILES += SensorApp.cc
LOCAL_SRC_FILES += SensorAppActionDetection.cc
LOCAL_SRC_FILES += SensorAppTapDetection.cc
LOCAL_SRC_FILES += SensorAppUndefined.cc



LOCAL_SRC_FILES += BLEApp.cc # 2024/05/30 追加
LOCAL_SRC_FILES += SensorAppHeadGestureDetection.cc # 2024/05/30 追加



LOCAL_MOD_REQUIRE :=
LOCAL_MOD_REQUIRE += libapp_shared
LOCAL_MOD_REQUIRE += libeasel-headless
LOCAL_MOD_REQUIRE += libpst

include $(BUILD_BUILTIN_EXECUTABLE)
