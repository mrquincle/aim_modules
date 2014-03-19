# File: Android.mk
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# Name of the lib
LOCAL_MODULE    := DatasetModule
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../inc $(LOCAL_PATH)/../../../aim-core/inc
LOCAL_SRC_FILES := DatasetModule_wrap.cpp ../../../src/DatasetModuleExt.cpp ../../../aim-core/src/DatasetModule.cpp
LOCAL_CFLAGS    := -frtti

include $(BUILD_SHARED_LIBRARY)
