LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libapp_shared

LOCAL_SRC_FILES += AppSdk.cc
LOCAL_SRC_FILES += AppSdkKEY.cc
LOCAL_SRC_FILES += AppSdkNVM.cc
LOCAL_SRC_FILES += AppSdkPWR.cc
LOCAL_SRC_FILES += AppSdkAUD.cc
LOCAL_SRC_FILES += AppSdkNET.cc
LOCAL_SRC_FILES += AppSdkSNS.cc
LOCAL_SRC_FILES += AppSdkSNSobserver.cc
LOCAL_SRC_FILES += AppSdkCDL.cc
LOCAL_SRC_FILES += AppSdkBAT.cc

LOCAL_MOD_EXPORT_INCLUDES += $(LOCAL_PATH)/../include

LOCAL_MOD_EXPORT_REQUIRE :=

LOCAL_MOD_REQUIRE += libpstbase
LOCAL_MOD_REQUIRE += libeasel-headless
LOCAL_MOD_REQUIRE += libsystemproperty
LOCAL_MOD_REQUIRE += libsettingmanager
LOCAL_MOD_REQUIRE += libvolatilesettingmanager
LOCAL_MOD_REQUIRE += libpowermgr
LOCAL_MOD_REQUIRE += libaudioclient
LOCAL_MOD_REQUIRE += libnetworkcommunicationclient
LOCAL_MOD_REQUIRE += libsensor
LOCAL_MOD_REQUIRE += libcradle
LOCAL_MOD_REQUIRE += libvolatilesettingmanager
LOCAL_MOD_REQUIRE += libbatterymgr

LOCAL_MOD_INTERNAL_INCLUDES :=
LOCAL_MOD_INTERNAL_INCLUDES += src/middleware/framework/include/cradle

LOCAL_CFLAGS += -DCONFIG_ENABLE_SYSTEM_PROPERTY
#ifeq ($(FEATURE_LED), 1)
#LOCAL_CFLAGS += -DFEATURE_LED
#endif

include $(BUILD_STATIC_LIBRARY)
