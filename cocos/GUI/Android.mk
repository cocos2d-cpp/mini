LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := cocos_extension_static

LOCAL_MODULE_FILENAME := libextension

LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := \
CCControlExtension/CCControl.cpp \
CCControlExtension/CCControlButton.cpp \
CCControlExtension/CCControlColourPicker.cpp \
CCControlExtension/CCControlHuePicker.cpp \
CCControlExtension/CCControlPotentiometer.cpp \
CCControlExtension/CCControlSaturationBrightnessPicker.cpp \
CCControlExtension/CCControlSlider.cpp \
CCControlExtension/CCControlStepper.cpp \
CCControlExtension/CCControlSwitch.cpp \
CCControlExtension/CCControlUtils.cpp \
CCControlExtension/CCInvocation.cpp \
CCScrollView/CCScrollView.cpp \
CCScrollView/CCTableView.cpp \
CCScrollView/CCTableViewCell.cpp

LOCAL_STATIC_LIBRARIES := cocos2dx_internal_static

LOCAL_CXXFLAGS += -fexceptions

include $(BUILD_STATIC_LIBRARY)
