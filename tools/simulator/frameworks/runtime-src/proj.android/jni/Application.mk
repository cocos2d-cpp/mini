APP_STL := gnustl_static

APP_CPPFLAGS := -frtti -std=c++14 -fsigned-char
APP_LDFLAGS := -latomic


ifeq ($(NDK_DEBUG),1)
  APP_CPPFLAGS += -DCOCOS2D_DEBUG=1
  APP_OPTIM := debug
else
  APP_CPPFLAGS += -DNDEBUG
  APP_OPTIM := release
endif

COCOS_SIMULATOR_BUILD := 1
USE_ARM_MODE := 1
