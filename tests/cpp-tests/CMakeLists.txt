set(APP_NAME cpp-tests)

# Use same method as in cocos library
if(LINUX)
  foreach(_pkg FONTCONFIG GTK3)
    cocos_find_package(${_pkg} ${_pkg} REQUIRED)
  endforeach()
endif()
cocos_find_package(FREETYPE FREETYPE REQUIRED)

if(WIN32)
  set(PLATFORM_SRC proj.win32/main.cpp)
  set(RES_PREFIX "")
elseif(MACOSX)
  set(PLATFORM_SRC proj.mac/main.cpp)

  file(GLOB_RECURSE RES_FILES Resources/*)
  cocos_mark_resources(FILES ${RES_FILES} BASEDIR Resources)
  list(APPEND PLATFORM_SRC ${RES_FILES})

elseif(LINUX)
  set(PLATFORM_SRC proj.linux/main.cpp)
  set(RES_PREFIX "/Resources")
elseif(ANDROID)
  set(PLATFORM_SRC
          Classes/JNITest/JNITest.cpp
          proj.android/jni/main.cpp)
  set(RES_PREFIX "/Resources")
else()
  message( FATAL_ERROR "Unsupported platform, CMake will exit" )
endif()

set(TESTS_SRC
  Classes/ActionManagerTest/ActionManagerTest.cpp
  Classes/ActionsEaseTest/ActionsEaseTest.cpp
  Classes/ActionsProgressTest/ActionsProgressTest.cpp
  Classes/ActionsTest/ActionsTest.cpp
  Classes/AllocatorTest/AllocatorTest.cpp
  Classes/AppDelegate.cpp
  Classes/BaseTest.cpp
  Classes/ClickAndMoveTest/ClickAndMoveTest.cpp
  Classes/ClippingNodeTest/ClippingNodeTest.cpp
  Classes/CocosDenshionTest/CocosDenshionTest.cpp
  Classes/ConfigurationTest/ConfigurationTest.cpp
  Classes/ConsoleTest/ConsoleTest.cpp
  Classes/CurrentLanguageTest/CurrentLanguageTest.cpp
  Classes/DrawPrimitivesTest/DrawPrimitivesTest.cpp
  Classes/EffectsAdvancedTest/EffectsAdvancedTest.cpp
  Classes/EffectsTest/EffectsTest.cpp
  Classes/ExtensionsTest/ControlExtensionTest/CCControlButtonTest/CCControlButtonTest.cpp
  Classes/ExtensionsTest/ControlExtensionTest/CCControlColourPicker/CCControlColourPickerTest.cpp
  Classes/ExtensionsTest/ControlExtensionTest/CCControlPotentiometerTest/CCControlPotentiometerTest.cpp
  Classes/ExtensionsTest/ControlExtensionTest/CCControlScene.cpp
  Classes/ExtensionsTest/ControlExtensionTest/CCControlSceneManager.cpp
  Classes/ExtensionsTest/ControlExtensionTest/CCControlSliderTest/CCControlSliderTest.cpp
  Classes/ExtensionsTest/ControlExtensionTest/CCControlStepperTest/CCControlStepperTest.cpp
  Classes/ExtensionsTest/ControlExtensionTest/CCControlSwitchTest/CCControlSwitchTest.cpp
  Classes/ExtensionsTest/ExtensionsTest.cpp
  Classes/ExtensionsTest/TableViewTest/CustomTableViewCell.cpp
  Classes/ExtensionsTest/TableViewTest/TableViewTestScene.cpp
  Classes/FileUtilsTest/FileUtilsTest.cpp
  Classes/FontTest/FontTest.cpp
  Classes/InputTest/MouseTest.cpp
  Classes/IntervalTest/IntervalTest.cpp
  Classes/LabelTest/LabelTest.cpp
  Classes/LabelTest/LabelTestNew.cpp
  Classes/LayerTest/LayerTest.cpp
  Classes/MaterialSystemTest/MaterialSystemTest
  Classes/MenuTest/MenuTest.cpp
  Classes/MotionStreakTest/MotionStreakTest.cpp
  Classes/MutiTouchTest/MutiTouchTest.cpp
  Classes/NewAudioEngineTest/NewAudioEngineTest.cpp
  Classes/NewEventDispatcherTest/NewEventDispatcherTest.cpp
  Classes/NewRendererTest/NewRendererTest.cpp
  Classes/NodeTest/NodeTest.cpp
  Classes/OpenURLTest/OpenURLTest.cpp
  Classes/ParallaxTest/ParallaxTest.cpp
  Classes/ParticleTest/ParticleTest.cpp
  Classes/ReleasePoolTest/ReleasePoolTest.cpp
  Classes/RenderTextureTest/RenderTextureTest.cpp
  Classes/RotateWorldTest/RotateWorldTest.cpp
  Classes/SceneTest/SceneTest.cpp
  Classes/SchedulerTest/SchedulerTest.cpp
  Classes/ShaderTest/ShaderTest.cpp
  Classes/ShaderTest/ShaderTest2.cpp
  Classes/SpritePolygonTest/SpritePolygonTest.cpp
  Classes/SpriteTest/SpriteTest.cpp
  Classes/TextInputTest/TextInputTest.cpp
  Classes/Texture2dTest/Texture2dTest.cpp
  Classes/TextureCacheTest/TextureCacheTest.cpp
  Classes/TexturePackerEncryptionTest/TextureAtlasEncryptionTest.cpp
  Classes/TileMapTest/TileMapTest.cpp
  Classes/TileMapTest/TileMapTest2.cpp
  Classes/TouchesTest/Ball.cpp
  Classes/TouchesTest/Paddle.cpp
  Classes/TouchesTest/TouchesTest.cpp
  Classes/TransitionsTest/TransitionsTest.cpp
  Classes/UnitTest/UnitTest.cpp
  Classes/UserDefaultTest/UserDefaultTest.cpp
  Classes/VisibleRect.cpp
  Classes/VibrateTest/VibrateTest.cpp
  Classes/ZwoptexTest/ZwoptexTest.cpp
  Classes/SpriteFrameCacheTest/SpriteFrameCacheTest.cpp
  Classes/controller.cpp
  Classes/testBasic.cpp
  ${PLATFORM_SRC}
)

if(WIN32 OR MACOSX OR LINUX)
  list(APPEND TESTS_SRC
    Classes/WindowTest/WindowTest.cpp
    )
endif()

include_directories(
  Classes
)

if(ANDROID)
  add_library(${APP_NAME} SHARED ${TESTS_SRC})
  if(CMAKE_BUILD_TYPE MATCHES RELEASE)
    ADD_CUSTOM_COMMAND(TARGET ${APP_NAME} POST_BUILD COMMAND ${CMAKE_STRIP} lib${APP_NAME}.so)
  endif()
else()
    # add the executable
    add_executable(${APP_NAME} ${TESTS_SRC})
endif()

target_link_libraries(${APP_NAME} cocos2d)

if(LINUX)
  foreach(_pkg FONTCONFIG GTK3)
    cocos_use_pkg(${APP_NAME} ${_pkg})
  endforeach()
endif()

foreach(pkg FREETYPE)
  cocos_use_pkg(${APP_NAME} ${pkg})
endforeach()

if(MACOSX OR APPLE)
  set_target_properties(${APP_NAME} PROPERTIES
      MACOSX_BUNDLE 1
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
elseif(ANDROID)
    set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin/${APP_NAME}")
    set_target_properties(${APP_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${APP_BIN_DIR}")
else()
    set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin/${APP_NAME}")
    set_target_properties(${APP_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${APP_BIN_DIR}")
    
    pre_build(${APP_NAME}
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${APP_BIN_DIR}${RES_PREFIX}
    )
endif()

