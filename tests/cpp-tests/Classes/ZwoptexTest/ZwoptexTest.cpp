#include "ZwoptexTest.h"
#include "../testResource.h"

#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"

using namespace cocos2d;

ZwoptexTests::ZwoptexTests()
{
    ADD_TEST_CASE(ZwoptexGenericTest);
}

//------------------------------------------------------------------
//
// ZwoptexGenericTest
//
//------------------------------------------------------------------
void ZwoptexGenericTest::onEnter()
{
    ZwoptexTest::onEnter();

    auto s = Director::getInstance()->getWinSize();

    _director->getSpriteFrameCache().addSpriteFramesWithFile("zwoptex/grossini.plist");
    _director->getSpriteFrameCache().addSpriteFramesWithFile("zwoptex/grossini-generic.plist");
    
    auto layer1 = LayerColor::create(Color4B(255, 0, 0, 255), 85, 121);
    layer1->setPosition(Vec2(s.width/2-80 - (85.0f * 0.5f), s.height/2 - (121.0f * 0.5f)));
    addChild(layer1);

    sprite1 = Sprite::create(_director->getSpriteFrameCache().getSpriteFrameByName("grossini_dance_01.png"));
    sprite1->setPosition(Vec2( s.width/2-80, s.height/2));
    addChild(sprite1);

    sprite1->setFlippedX(false);
    sprite1->setFlippedY(false);

    auto layer2 = LayerColor::create(Color4B(255, 0, 0, 255), 85, 121);
    layer2->setPosition(Vec2(s.width/2+80 - (85.0f * 0.5f), s.height/2 - (121.0f * 0.5f)));
    addChild(layer2);
    
    sprite2 = Sprite::create(_director->getSpriteFrameCache().getSpriteFrameByName("grossini_dance_generic_01.png"));
    sprite2->setPosition(Vec2( s.width/2 + 80, s.height/2));
    addChild(sprite2);

    sprite2->setFlippedX(false);
    sprite2->setFlippedY(false);

    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &ZwoptexGenericTest::startIn05Secs)
            .interval( 1.0f )
            .repeat(0)
            .delay( 5.0f )
            .paused( isPaused() )
    );
    
    sprite1->retain();
    sprite2->retain();

    counter = 0;
}

void ZwoptexGenericTest::startIn05Secs(float /*dt*/)
{
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &ZwoptexGenericTest::flipSprites)
            .interval( 0.5f )
            .delay( 0.5f )
            .paused( isPaused() )
    );
}

static int spriteFrameIndex = 0;
void ZwoptexGenericTest::flipSprites(float /*dt*/)
{
    counter++;

    bool fx = false;
    bool fy = false;
    int i = counter % 4;

    switch ( i )
    {
        case 0:
            fx = false;
            fy = false;
            break;
        case 1:
            fx = true;
            fy = false;
            break;
        case 2:
            fx = false;
            fy = true;
            break;
        case 3:
            fx = true;
            fy = true;
            break;
    }

    sprite1->setFlippedX(fx);
    sprite2->setFlippedX(fx);
    sprite1->setFlippedY(fy);
    sprite2->setFlippedY(fy);

    if(++spriteFrameIndex > 14)
    {
        spriteFrameIndex = 1;
    }

    char str1[32] = {0};
    char str2[32] = {0};
    sprintf(str1, "grossini_dance_%02d.png", spriteFrameIndex);
    sprintf(str2, "grossini_dance_generic_%02d.png", spriteFrameIndex);
    sprite1->setSpriteFrame(_director->getSpriteFrameCache().getSpriteFrameByName(str1));
    sprite2->setSpriteFrame(_director->getSpriteFrameCache().getSpriteFrameByName(str2));
}

ZwoptexGenericTest::~ZwoptexGenericTest()
{
    sprite1->release();
    sprite2->release();
    auto & cache = _director->getSpriteFrameCache();
    cache.removeSpriteFramesFromFile("zwoptex/grossini.plist");
    cache.removeSpriteFramesFromFile("zwoptex/grossini-generic.plist");
}

std::string ZwoptexGenericTest::title() const
{
    return "Zwoptex Tests";
}

std::string ZwoptexGenericTest::subtitle() const
{
    return "Coordinate Formats, Rotation, Trimming, flipX/Y";
}
