#ifndef _SHADER_TEST2_H_
#define _SHADER_TEST2_H_

#include "../BaseTest.h"

namespace cocos2d {
class Sprite;
}

DEFINE_TEST_SUITE(Shader2Tests);

class ShaderTestDemo2 : public TestCase
{
public:
    virtual std::string title() const override { return "Effects on Sprites";}
};

//
// Effect
//
class EffectSprite;

class Effect : public cocos2d::Ref
{
public:
    cocos2d::GLProgramState* getGLProgramState() const { return _glprogramstate; }
    virtual void setTarget(EffectSprite *){}

protected:
    bool initGLProgramState(const std::string &fragmentFilename);
    Effect();
    virtual ~Effect();
    cocos2d::GLProgramState* _glprogramstate;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    std::string _fragSource;
    cocos2d::EventListenerCustom* _backgroundListener;
#endif
};

class EffectSpriteTest : public ShaderTestDemo2
{
public:
    static EffectSpriteTest* create()
    {
        auto ret = new EffectSpriteTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    EffectSpriteTest();
    virtual bool init();
    virtual std::string subtitle() const {return "Different effects on Sprite";}

protected:
    ssize_t _vectorIndex;
    std::vector<cocos2d::retaining_ptr<Effect>> _effects;
    EffectSprite *_sprite;
};

class EffectSpriteLamp : public ShaderTestDemo2
{
public:
    static EffectSpriteLamp* create()
    {
        auto ret = new EffectSpriteLamp;
        ret->init();
        ret->autorelease();
        return ret;
    }
    EffectSpriteLamp();
    virtual bool init();
    virtual std::string subtitle() const {return "Sprite Lamp effects";}
    //callback
public:
    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* unused_event);
    virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* unused_event);
    virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* unused_event);
protected:
    EffectSprite *_sprite;
    Effect* _effect;
    cocos2d::Sprite* _lightSprite;
};

#endif
