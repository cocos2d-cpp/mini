if(WINDOWS AND BUILD_SHARED_LIBS)
  ADD_DEFINITIONS (-D_USREXDLL)
endif()

set(COCOS_EXTENSIONS_SRC

  GUI/CCControlExtension/CCControl.cpp
  GUI/CCControlExtension/CCControlButton.cpp
  GUI/CCControlExtension/CCControlColourPicker.cpp
  GUI/CCControlExtension/CCControlHuePicker.cpp
  GUI/CCControlExtension/CCControlPotentiometer.cpp
  GUI/CCControlExtension/CCControlSaturationBrightnessPicker.cpp
  GUI/CCControlExtension/CCControlSlider.cpp
  GUI/CCControlExtension/CCControlStepper.cpp
  GUI/CCControlExtension/CCControlSwitch.cpp
  GUI/CCControlExtension/CCControlUtils.cpp
  GUI/CCControlExtension/CCInvocation.cpp
  GUI/CCScrollView/CCScrollView.cpp
  GUI/CCScrollView/CCTableView.cpp
  GUI/CCScrollView/CCTableViewCell.cpp
  ${PLATFORM_EXTENSIONS_SRC}
)
