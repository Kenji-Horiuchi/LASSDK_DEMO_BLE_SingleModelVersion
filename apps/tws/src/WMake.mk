LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_BUILTIN_APPNAME := base_app
LOCAL_BUILTIN_STACKSIZE := 2048

LOCAL_SRC_FILES :=
LOCAL_SRC_FILES += TwsBaseApp.cc

LOCAL_MOD_REQUIRE :=
LOCAL_MOD_REQUIRE += libeasel-headless
LOCAL_MOD_REQUIRE += libapp_shared
LOCAL_MOD_REQUIRE += libpst

include $(BUILD_BUILTIN_EXECUTABLE)
