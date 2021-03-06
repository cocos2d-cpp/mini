#ifndef _TESTS_H_
#define _TESTS_H_

#if (CC_TARGET_PLATFORM != CC_PLATFORM_MARMALADE)
#include "ClippingNodeTest/ClippingNodeTest.h"
#endif
#include "NewAudioEngineTest/NewAudioEngineTest.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "JNITest/JNITest.h"
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
#include "WindowTest/WindowTest.h"
#endif

// sort them alphabetically. thanks
#include "ActionManagerTest/ActionManagerTest.h"
#include "ActionsEaseTest/ActionsEaseTest.h"
#include "ActionsProgressTest/ActionsProgressTest.h"
#include "ActionsTest/ActionsTest.h"
#include "AllocatorTest/AllocatorTest.h"
#include "ClickAndMoveTest/ClickAndMoveTest.h"
#include "CocosDenshionTest/CocosDenshionTest.h"
#include "ConfigurationTest/ConfigurationTest.h"
#include "ConsoleTest/ConsoleTest.h"
#include "CurrentLanguageTest/CurrentLanguageTest.h"
#include "DrawPrimitivesTest/DrawPrimitivesTest.h"
#include "EffectsAdvancedTest/EffectsAdvancedTest.h"
#include "EffectsTest/EffectsTest.h"
#include "ExtensionsTest/ExtensionsTest.h"
#include "FileUtilsTest/FileUtilsTest.h"
#include "FontTest/FontTest.h"
#include "InputTest/MouseTest.h"
#include "IntervalTest/IntervalTest.h"
#include "LabelTest/LabelTest.h"
#include "LabelTest/LabelTestNew.h"
#include "LayerTest/LayerTest.h"
#include "MaterialSystemTest/MaterialSystemTest.h"
#include "MenuTest/MenuTest.h"
#include "MotionStreakTest/MotionStreakTest.h"
#include "MutiTouchTest/MutiTouchTest.h"
#include "NewEventDispatcherTest/NewEventDispatcherTest.h"
#include "NewRendererTest/NewRendererTest.h"
#include "NodeTest/NodeTest.h"
#include "OpenURLTest/OpenURLTest.h"
#include "ParallaxTest/ParallaxTest.h"
#include "ParticleTest/ParticleTest.h"
#include "ReleasePoolTest/ReleasePoolTest.h"
#include "RenderTextureTest/RenderTextureTest.h"
#include "RotateWorldTest/RotateWorldTest.h"
#include "SceneTest/SceneTest.h"
#include "SchedulerTest/SchedulerTest.h"
#include "ShaderTest/ShaderTest.h"
#include "ShaderTest/ShaderTest2.h"
#include "SpritePolygonTest/SpritePolygonTest.h"
#include "SpriteTest/SpriteTest.h"
#include "TextInputTest/TextInputTest.h"
#include "Texture2dTest/Texture2dTest.h"
#include "TextureCacheTest/TextureCacheTest.h"
#include "TexturePackerEncryptionTest/TextureAtlasEncryptionTest.h"
#include "TileMapTest/TileMapTest.h"
#include "TileMapTest/TileMapTest2.h"
#include "TouchesTest/TouchesTest.h"
#include "TransitionsTest/TransitionsTest.h"
#include "UnitTest/UnitTest.h"
#include "UserDefaultTest/UserDefaultTest.h"
#include "VibrateTest/VibrateTest.h"
#include "ZwoptexTest/ZwoptexTest.h"
#include "SpriteFrameCacheTest/SpriteFrameCacheTest.h"

#endif
