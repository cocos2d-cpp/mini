#include "ExtensionsTest.h"
#include "../testResource.h"
#include "ControlExtensionTest/CCControlSceneManager.h"
#include "TableViewTest/TableViewTestScene.h"

ExtensionsTests::ExtensionsTests()
{
    addTest("CCControlButtonTest", [](){ return new (std::nothrow) ControlTests; });
    addTest("TableViewTest", [](){ return new (std::nothrow) TableViewTests; });
}
