LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_BUILTIN_APPNAME := fw_update_app
LOCAL_BUILTIN_STACKSIZE := 2048

LOCAL_SRC_FILES :=
LOCAL_SRC_FILES += FwUpdateApp.cc

LOCAL_MOD_REQUIRE :=
LOCAL_MOD_REQUIRE += libfwupdatemanager
LOCAL_MOD_REQUIRE += libapp_shared
LOCAL_MOD_REQUIRE += libeasel-headless-daemon
LOCAL_MOD_REQUIRE += libpst

include $(BUILD_BUILTIN_EXECUTABLE)
