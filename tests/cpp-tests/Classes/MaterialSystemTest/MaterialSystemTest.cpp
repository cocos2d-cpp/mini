/****************************************************************************
 Copyright (c) 2012 cocos2d-x.org
 Copyright (c) 2013-2016 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "MaterialSystemTest.h"
#include "../testResource.h"

#include "2d/CCLabel.h"
#include "2d/CCLight.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCProperties.h"
#include "base/ccUTF8.h"
#include "renderer/CCMaterial.h"
#include "renderer/CCRenderer.h"
#include "ui/UISlider.h"

#include <ctime>

using namespace cocos2d;

static void printProperties(Properties* properties, int indent);


// MARK: Tests

MaterialSystemTest::MaterialSystemTest()
{
    ADD_TEST_CASE(Material_2DEffects);
    ADD_TEST_CASE(Material_AutoBindings);
    ADD_TEST_CASE(Material_parsePerformance);
}

std::string MaterialSystemBaseTest::title() const
{
    return "Material System";
}

//
// MARK: Material_2DEffects
//
void Material_2DEffects::onEnter()
{
    MaterialSystemBaseTest::onEnter();

    auto properties = Properties::createNonRefCounted("Materials/2d_effects.material#sample");

    // Print the properties of every namespace within this one.
    printProperties(properties, 0);

    Material *mat1 = Material::createWithProperties(properties);

    auto spriteBlur = Sprite::create("Images/grossini.png");
    spriteBlur->setPositionNormalized(Vec2(0.2f, 0.5f));
    this->addChild(spriteBlur);
    spriteBlur->setGLProgramState(mat1->getTechniqueByName("blur")->getPassByIndex(0)->getGLProgramState());

    auto spriteOutline = Sprite::create("Images/grossini.png");
    spriteOutline->setPositionNormalized(Vec2(0.4f, 0.5f));
    this->addChild(spriteOutline);
    spriteOutline->setGLProgramState(mat1->getTechniqueByName("outline")->getPassByIndex(0)->getGLProgramState());

    auto spriteNoise = Sprite::create("Images/grossini.png");
    spriteNoise->setPositionNormalized(Vec2(0.6f, 0.5f));
    this->addChild(spriteNoise);
    spriteNoise->setGLProgramState(mat1->getTechniqueByName("noise")->getPassByIndex(0)->getGLProgramState());

    auto spriteEdgeDetect = Sprite::create("Images/grossini.png");
    spriteEdgeDetect->setPositionNormalized(Vec2(0.8f, 0.5f));
    this->addChild(spriteEdgeDetect);
    spriteEdgeDetect->setGLProgramState(mat1->getTechniqueByName("edge_detect")->getPassByIndex(0)->getGLProgramState());

    // properties is not a "Ref" object
    CC_SAFE_DELETE(properties);
}

std::string Material_2DEffects::subtitle() const
{
    return "Testing effects on Sprite";
}

//
// MARK: Material_AutoBindings
//

/*
 * Custom material auto-binding resolver for terrain.
 */
class EffectAutoBindingResolver : public GLProgramState::AutoBindingResolver
{
    bool resolveAutoBinding(GLProgramState* glProgramState, Node*, const std::string& uniform, const std::string& autoBinding);

    void callbackRadius(GLProgram* glProgram, Uniform* uniform);
    void callbackColor(GLProgram* glProgram, Uniform* uniform);
};

bool EffectAutoBindingResolver::resolveAutoBinding(GLProgramState* glProgramState, Node*, const std::string& uniform, const std::string& autoBinding)
{
    if (autoBinding.compare("DYNAMIC_RADIUS")==0)
    {
        glProgramState->setUniformCallback(uniform, CC_CALLBACK_2(EffectAutoBindingResolver::callbackRadius, this));
        return true;
    }
    else if (autoBinding.compare("OUTLINE_COLOR")==0)
    {
        glProgramState->setUniformCallback(uniform, CC_CALLBACK_2(EffectAutoBindingResolver::callbackColor, this));
        return true;
    }
    return false;
}

void EffectAutoBindingResolver::callbackRadius(GLProgram* glProgram, Uniform* uniform)
{
    float f = CCRANDOM_0_1() * 10;
    glProgram->setUniformLocationWith1f(uniform->location, f);
}

void EffectAutoBindingResolver::callbackColor(GLProgram* glProgram, Uniform* uniform)
{
    float r = CCRANDOM_0_1();
    float g = CCRANDOM_0_1();
    float b = CCRANDOM_0_1();

    glProgram->setUniformLocationWith3f(uniform->location, r, g, b);
}

Material_AutoBindings::Material_AutoBindings()
{
    _resolver = new EffectAutoBindingResolver;
}

Material_AutoBindings::~Material_AutoBindings()
{
    delete _resolver;
}


void Material_AutoBindings::onEnter()
{
    MaterialSystemBaseTest::onEnter();

//    auto properties = Properties::createNonRefCounted("Materials/2d_effects.material#sample");
    auto properties = Properties::createNonRefCounted("Materials/auto_binding_test.material#sample");

    // Print the properties of every namespace within this one.
    printProperties(properties, 0);

    Material *mat1 = Material::createWithProperties(properties);

    auto spriteBlur = Sprite::create("Images/grossini.png");
    spriteBlur->setPositionNormalized(Vec2(0.2f, 0.5f));
    this->addChild(spriteBlur);
    spriteBlur->setGLProgramState(mat1->getTechniqueByName("blur")->getPassByIndex(0)->getGLProgramState());

    auto spriteOutline = Sprite::create("Images/grossini.png");
    spriteOutline->setPositionNormalized(Vec2(0.4f, 0.5f));
    this->addChild(spriteOutline);
    spriteOutline->setGLProgramState(mat1->getTechniqueByName("outline")->getPassByIndex(0)->getGLProgramState());

    auto spriteNoise = Sprite::create("Images/grossini.png");
    spriteNoise->setPositionNormalized(Vec2(0.6f, 0.5f));
    this->addChild(spriteNoise);
    spriteNoise->setGLProgramState(mat1->getTechniqueByName("noise")->getPassByIndex(0)->getGLProgramState());

    auto spriteEdgeDetect = Sprite::create("Images/grossini.png");
    spriteEdgeDetect->setPositionNormalized(Vec2(0.8f, 0.5f));
    this->addChild(spriteEdgeDetect);
    spriteEdgeDetect->setGLProgramState(mat1->getTechniqueByName("edge_detect")->getPassByIndex(0)->getGLProgramState());

    // properties is not a "Ref" object
    CC_SAFE_DELETE(properties);
}

std::string Material_AutoBindings::subtitle() const
{
    return "Testing auto-bindings uniforms";
}

//
//
//
const int SHOW_LEBAL_TAG = 114;

void Material_parsePerformance::onEnter()
{
    MaterialSystemBaseTest::onEnter();
    
    _maxParsingCoumt = 5e3;
    
    auto screenSize = Director::getInstance()->getWinSize();
    
    ui::Slider* slider = ui::Slider::create();
    slider->loadBarTexture("cocosui/sliderTrack.png");
    slider->loadSlidBallTextures("cocosui/sliderThumb.png", "cocosui/sliderThumb.png", "");
    slider->loadProgressBarTexture("cocosui/sliderProgress.png");
    slider->setPercent(50);
    
    slider->setPosition(Vec2(screenSize.width / 2.0f, screenSize.height / 3.0f));
    slider->addEventListener([&](Ref* sender, ui::Slider::EventType type) {
        
        if (type == ui::Slider::EventType::ON_SLIDEBALL_UP)
        {
            ui::Slider* slider = dynamic_cast<ui::Slider*>(sender);
            float p = slider->getPercent() / 100.0f;
            slider->setTouchEnabled(false);
            CCLOG("Will parsing material %d times", (int)(p * _maxParsingCoumt));
            Label* label = dynamic_cast<Label*>(this->getChildByTag(SHOW_LEBAL_TAG));
            if(label)
            {
                label->setString("Testing start!");
            }
            auto schedule_test_parsing = [this, p, slider](float) {
                this->parsingTesting(p * _maxParsingCoumt);
                slider->setTouchEnabled(true);
            };
            Director::getInstance()->getScheduler().schedule(
                TimedJob(this, schedule_test_parsing, 0)
                    .delay(1.0f)
                    .repeat(0)
                    .paused(isPaused())
            );
        }
    });
    
    addChild(slider);
    
    auto label = Label::createWithSystemFont("Max parsing count is 10000, which may crash because of high memory comsumption.", "Helvetica", 10);
    label->setPosition(Vec2(screenSize.width / 2.0f, screenSize.height / 2.0f - 20));
    addChild(label);
    label = Label::createWithSystemFont("Slide to test parsing performance", "Helvetica", 10);
    label->setPosition(Vec2(screenSize.width / 2.0f, screenSize.height / 2.0f));
    addChild(label);
    
    label = Label::createWithSystemFont("", "Helvetica", 10);
    label->setPosition(Vec2(screenSize.width / 2.0f, screenSize.height / 2.0f + 20));
    label->setTag(SHOW_LEBAL_TAG);
    addChild(label);

}

void Material_parsePerformance::parsingTesting(unsigned int count)
{
    std::clock_t begin = std::clock();
    
    for (unsigned int i = 0; i < count; i++)
    {
        Material::createWithFilename("Materials/2d_effects.material");
        Material::createWithFilename("Materials/3d_effects.material");
    }
    
    std::clock_t end = std::clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    Label* label = dynamic_cast<Label*>(this->getChildByTag(SHOW_LEBAL_TAG));
    if(label)
    {
        std::string str = StringUtils::format("Testing completed! Took: %.3f seconds for parsing material %d times.", elapsed_secs, count);
        label->setString(str);
        
        CCLOG("Took: %.3f seconds for parsing material %d times.", elapsed_secs, count);
    }
}

std::string Material_parsePerformance::subtitle() const
{
    return "Testing parsing performance";
}

// MARK: Helper functions

static void printProperties(Properties* properties, int indent)
{
    // Print the name and ID of the current namespace.
    const char* spacename = properties->getNamespace();
    const char* id = properties->getId();
    char chindent[64];
    int i=0;
    for(i=0; i<indent*2;i++)
        chindent[i] = ' ';
    chindent[i] = '\0';

    log("%sNamespace: %s  ID: %s\n%s{", chindent, spacename, id, chindent);

    // Print all properties in this namespace.
    const char* name = properties->getNextProperty();
    const char* value = NULL;
    while (name != NULL)
    {
        value = properties->getString(name);
        log("%s%s = %s", chindent, name, value);
        name = properties->getNextProperty();
    }

    Properties* space = properties->getNextNamespace();
    while (space != NULL)
    {
        printProperties(space, indent+1);
        space = properties->getNextNamespace();
    }

    log("%s}\n",chindent);
}
