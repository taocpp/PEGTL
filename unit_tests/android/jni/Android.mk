LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# build unit test and native activity
LOCAL_MODULE     := native-activity
LOCAL_SRC_FILES  := $(TESTS) main.cc
LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/../.. $(LOCAL_PATH)/../../..
LOCAL_CFLAGS     := -std=c++11
LOCAL_CPPFLAGS   := -std=c++11
LOCAL_LDLIBS     := -llog -landroid
LOCAL_STATIC_LIBRARIES += android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
