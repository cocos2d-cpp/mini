#include "SchedulerTest.h"
#include "../testResource.h"

#include "2d/CCParticleExamples.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCScheduler.h"
#include "renderer/CCTextureCache.h"

using namespace cocos2d;
using namespace cocos2d::extension;

enum {
    kTagAnimationDance = 1,
};

SchedulerTests::SchedulerTests()
{
    ADD_TEST_CASE(SchedulerSpeedup);
    ADD_TEST_CASE(SchedulerAutoremove);
    ADD_TEST_CASE(SchedulerPauseResume);
    ADD_TEST_CASE(SchedulerPauseResumeAll);
    ADD_TEST_CASE(SchedulerPauseResumeAllUser);
    ADD_TEST_CASE(SchedulerUnscheduleAll);
    ADD_TEST_CASE(SchedulerUnscheduleAllUserLevel);
    ADD_TEST_CASE(SchedulerSchedulesAndRemove);
    ADD_TEST_CASE(SchedulerUpdate);
    ADD_TEST_CASE(SchedulerUpdateAndCustom);
    ADD_TEST_CASE(SchedulerUpdateFromCustom);
    ADD_TEST_CASE(RescheduleSelector);
    ADD_TEST_CASE(SchedulerDelayAndRepeat);
    ADD_TEST_CASE(SchedulerIssue2268);
    ADD_TEST_CASE(ScheduleCallbackTest);
    ADD_TEST_CASE(ScheduleUpdatePriority);
    ADD_TEST_CASE(SchedulerIssue10232);
};

//------------------------------------------------------------------
//
// SchedulerAutoremove
//
//------------------------------------------------------------------
static TimedJob::id_type SchedulerAutoremove_autoremove_JOBID = 0;

void SchedulerAutoremove::onEnter()
{
    SchedulerTestLayer::onEnter();

    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerAutoremove::autoremove, SchedulerAutoremove_autoremove_JOBID)
            .interval(0.5f)
    );
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerAutoremove::tick)
            .interval(0.5f)
    );

    accum = 0.0f;
}

void SchedulerAutoremove::autoremove(float dt)
{
    accum += dt;
    CCLOG("Message 1 should be stopped in %f sec. Time: %f", 3.0f - accum, accum);

    if( accum >= 3 )
    {
        Director::getInstance()->getScheduler().unscheduleTimedJob(this, SchedulerAutoremove_autoremove_JOBID);
        CCLOG("Message 1 has been stopped");
    }
}

void SchedulerAutoremove::tick(float /*dt*/)
{
    CCLOG("Messsage 2 message should never be stopped");
}

std::string SchedulerAutoremove::title() const
{
    return "Self-removed scheduler";
}

std::string SchedulerAutoremove::subtitle() const
{
    return "1 scheduler will be autoremoved in 3 seconds. See console";
}

//------------------------------------------------------------------
//
// SchedulerPauseResume
//
//------------------------------------------------------------------
void SchedulerPauseResume::onEnter()
{
    SchedulerTestLayer::onEnter();
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerPauseResume::tick1)
            .interval(0.5f)
            .delay(0.5f)
            .paused(isPaused())
    );
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerPauseResume::tick2)
            .interval(0.5f)
            .delay(0.5f)
            .paused(isPaused())
    );
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerPauseResume::pause)
            .interval(0.0f)
            .delay(3.0f)
            .paused(isPaused())
    );
}

void SchedulerPauseResume::tick1(float /*dt*/)
{
    CCLOG("tick1");
}

void SchedulerPauseResume::tick2(float /*dt*/)
{
    CCLOG("tick2");
}

void SchedulerPauseResume::pause(float dt)
{
    CCLOG("paused. tick1 and tick2 should have been called six times. Time %f", dt);
    Director::getInstance()->getScheduler().pauseAllForTarget(this);
}

std::string SchedulerPauseResume::title() const
{
    return "Pause / Resume";
}

std::string SchedulerPauseResume::subtitle() const
{
    return "Scheduler should be paused after 3 seconds. See console";
}

//------------------------------------------------------------------
//
// SchedulerPauseResumeAll
//
//------------------------------------------------------------------

SchedulerPauseResumeAll::SchedulerPauseResumeAll()
{
    
}

SchedulerPauseResumeAll::~SchedulerPauseResumeAll()
{

}

void SchedulerPauseResumeAll::onEnter()
{
    SchedulerTestLayer::onEnter();

    auto sprite = Sprite::create("Images/grossinis_sister1.png");
    sprite->setPosition(VisibleRect::center());
    this->addChild(sprite);
    sprite->runAction(std::make_unique<RepeatForever>(std::make_unique<RotateBy>(3.0, 360)));
    sprite->setTag(123);
    Director::getInstance()->getScheduler().schedule(UpdateJob(this).paused(isPaused()));
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerPauseResumeAll::tick1, 0)
            .delay(0.5f)
            .interval(0.5f)
            .paused(isPaused())
    );
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerPauseResumeAll::tick2, 1)
            .delay(1.0f)
            .interval(1.0f)
            .paused(isPaused())
    );
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerPauseResumeAll::pause, 2)
            .delay(3.0f)
            .interval(3.0f)
            .paused(isPaused())
    );
}

void SchedulerPauseResumeAll::update(float /*delta*/)
{
    // do nothing
}

void SchedulerPauseResumeAll::onExit()
{
    Director::getInstance()->getScheduler().resumeAll();
    SchedulerTestLayer::onExit();
}

void SchedulerPauseResumeAll::tick1(float /*dt*/)
{
    log("tick1");
}

void SchedulerPauseResumeAll::tick2(float /*dt*/)
{
    log("tick2");
}

void SchedulerPauseResumeAll::pause(float /*dt*/)
{
    log("Pausing, tick1 should be called six times and tick2 three times");
    Director::getInstance()->getScheduler().pauseAll();

    // because target 'this' has been paused above, so use another node(tag:123) as target
    auto child123 = getChildByTag(123);
    auto f = [this](float dt) {
        this->resume(dt);
    };

    Director::getInstance()->getScheduler().schedule(
        TimedJob(child123, f, 3).delay(2.0f)
    );
}

void SchedulerPauseResumeAll::resume(float /*dt*/)
{
    log("Resuming");
    Director::getInstance()->getScheduler().resumeAll();
}

std::string SchedulerPauseResumeAll::title() const
{
    return "Pause / Resume All";
}

std::string SchedulerPauseResumeAll::subtitle() const
{
    return "Everything will pause after 3s, then resume at 5s. See console";
}

//------------------------------------------------------------------
//
// SchedulerPauseResumeAllUser
//
//------------------------------------------------------------------

SchedulerPauseResumeAllUser::SchedulerPauseResumeAllUser()
{

}

SchedulerPauseResumeAllUser::~SchedulerPauseResumeAllUser()
{

}

void SchedulerPauseResumeAllUser::onEnter()
{
    SchedulerTestLayer::onEnter();

    auto s = Director::getInstance()->getWinSize();

    auto sprite = Sprite::create("Images/grossinis_sister1.png");
    sprite->setPosition(Vec2(s.width/2, s.height/2));
    sprite->setTag(123);
    this->addChild(sprite);
    sprite->runAction(std::make_unique<RepeatForever>(std::make_unique<RotateBy>(3.0, 360)));

    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerPauseResumeAllUser::tick1)
            .delay(1.0f)
            .interval(1.0f)
            .paused(isPaused())
    );
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerPauseResumeAllUser::tick2)
            .delay(1.0f)
            .interval(1.0f)
            .paused(isPaused())
    );
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerPauseResumeAllUser::pause)
            .interval(3.0f)
            .delay(3.0f)
            .paused(isPaused())
    );
}

void SchedulerPauseResumeAllUser::onExit()
{
    SchedulerTestLayer::onExit();
}

void SchedulerPauseResumeAllUser::tick1(float /*dt*/)
{
    log("tick1");
}

void SchedulerPauseResumeAllUser::tick2(float /*dt*/)
{
    log("tick2");
}

void SchedulerPauseResumeAllUser::pause(float /*dt*/)
{
    log("Pausing, tick1 and tick2 should be called three times");
    auto director = Director::getInstance();
    director->getScheduler().pauseAll();
    // using another node(tag:123) as target
    auto child123 = getChildByTag(123);
    Director::getInstance()->getScheduler().schedule(
        TimedJob(child123, [this](float dt) { this->resume(dt); })
            .delay(2.0f)
            .repeat(0)
    );
}

void SchedulerPauseResumeAllUser::resume(float /*dt*/)
{
    log("Resuming");
    Director::getInstance()->getScheduler().resumeAll();
}

std::string SchedulerPauseResumeAllUser::title() const
{
    return "Pause / Resume All User scheduler";
}

std::string SchedulerPauseResumeAllUser::subtitle() const
{
    return "ticks will pause after 3s, then resume at 5s. See console";
}


//------------------------------------------------------------------
//
// SchedulerUnscheduleAll
//
//------------------------------------------------------------------
void SchedulerUnscheduleAll::onEnter()
{
    SchedulerTestLayer::onEnter();

    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerUnscheduleAll::tick1)
            .interval(0.5f)
    );
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerUnscheduleAll::tick2)
            .interval(1.0f)
    );
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerUnscheduleAll::tick3)
            .interval(1.5f)
    );
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerUnscheduleAll::tick4)
            .interval(1.5f)
    );
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerUnscheduleAll::unscheduleAll)
            .delay(4.0f)
    );
}

void SchedulerUnscheduleAll::tick1(float /*dt*/)
{
    CCLOG("tick1");
}

void SchedulerUnscheduleAll::tick2(float /*dt*/)
{
    CCLOG("tick2");
}

void SchedulerUnscheduleAll::tick3(float /*dt*/)
{
    CCLOG("tick3");
}

void SchedulerUnscheduleAll::tick4(float /*dt*/)
{
    CCLOG("tick4");
}

void SchedulerUnscheduleAll::unscheduleAll(float /*dt*/)
{
    Director::getInstance()->getScheduler().unscheduleAllForTarget(this);
}

std::string SchedulerUnscheduleAll::title() const
{
    return "Unschedule All selectors";
}

std::string SchedulerUnscheduleAll::subtitle() const
{
    return "All scheduled selectors will be unscheduled in 4 seconds. See console";
}

//------------------------------------------------------------------
//
// SchedulerUnscheduleAllUserLevel
//
//------------------------------------------------------------------
void SchedulerUnscheduleAllUserLevel::onEnter()
{
    SchedulerTestLayer::onEnter();

    auto s = Director::getInstance()->getWinSize();

    auto sprite = Sprite::create("Images/grossinis_sister1.png");
    sprite->setPosition(Vec2(s.width/2, s.height/2));
    this->addChild(sprite);
    sprite->runAction(std::make_unique<RepeatForever>(std::make_unique<RotateBy>(3.0, 360)));

    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerUnscheduleAllUserLevel::tick1)
            .delay(0.5f)
            .interval(0.5f)
            .paused(isPaused())
    );
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerUnscheduleAllUserLevel::tick2)
            .delay(1.0f)
            .interval(1.0f)
            .paused(isPaused())
    );
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerUnscheduleAllUserLevel::tick3)
            .delay(1.5f)
            .interval(1.5f)
            .paused(isPaused())
    );
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerUnscheduleAllUserLevel::tick4)
            .delay(1.5f)
            .interval(1.5f)
            .paused(isPaused())
    );
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerUnscheduleAllUserLevel::unscheduleAll)
            .delay(4.0f)
            .interval(4.0f)
            .paused(isPaused())
    );
}

void SchedulerUnscheduleAllUserLevel::tick1(float /*dt*/)
{
    CCLOG("tick1");
}

void SchedulerUnscheduleAllUserLevel::tick2(float /*dt*/)
{
    CCLOG("tick2");
}

void SchedulerUnscheduleAllUserLevel::tick3(float /*dt*/)
{
    CCLOG("tick3");
}

void SchedulerUnscheduleAllUserLevel::tick4(float /*dt*/)
{
    CCLOG("tick4");
}

void SchedulerUnscheduleAllUserLevel::unscheduleAll(float /*dt*/)
{
    Director::getInstance()->getScheduler().unscheduleAll();
}

std::string SchedulerUnscheduleAllUserLevel::title() const
{
    return "Unschedule All user selectors";
}

std::string SchedulerUnscheduleAllUserLevel::subtitle() const
{
    return "Unschedules all user selectors after 4s. Action should not stop. See console";
}

//------------------------------------------------------------------
//
// SchedulerSchedulesAndRemove
//
//------------------------------------------------------------------
void SchedulerSchedulesAndRemove::onEnter()
{
    SchedulerTestLayer::onEnter();

    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerSchedulesAndRemove::tick1, 1)
            .delay(0.5f)
            .interval(0.5f)
            .paused(isPaused())
    );
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerSchedulesAndRemove::tick2, 2)
            .delay(1.0f)
            .interval(1.0f)
            .paused(isPaused())
    );
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerSchedulesAndRemove::scheduleAndUnschedule, 3)
            .delay(4.0f)
            .interval(4.0f)
            .paused(isPaused())
    );
}

void SchedulerSchedulesAndRemove::tick1(float /*dt*/)
{
    CCLOG("tick1");
}

void SchedulerSchedulesAndRemove::tick2(float /*dt*/)
{
    CCLOG("tick2");
}

void SchedulerSchedulesAndRemove::tick3(float /*dt*/)
{
    CCLOG("tick3");
}

void SchedulerSchedulesAndRemove::tick4(float /*dt*/)
{
    CCLOG("tick4");
}

std::string SchedulerSchedulesAndRemove::title() const
{
    return "Schedule from Schedule";
}

std::string SchedulerSchedulesAndRemove::subtitle() const
{
    return "Will unschedule and schedule selectors in 4s. See console";
}

void SchedulerSchedulesAndRemove::scheduleAndUnschedule(float /*dt*/)
{
    Director::getInstance()->getScheduler().unscheduleTimedJob(this, 1);
    Director::getInstance()->getScheduler().unscheduleTimedJob(this, 2);
    Director::getInstance()->getScheduler().unscheduleTimedJob(this, 3);

    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerSchedulesAndRemove::tick3, 4)
            .delay(1.0f)
            .interval(1.0f)
            .paused(isPaused())
    );
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerSchedulesAndRemove::tick4, 5)
            .delay(1.0f)
            .interval(1.0f)
            .paused(isPaused())
    );
}

//------------------------------------------------------------------
//
// TestNode
//
//------------------------------------------------------------------
void TestNode::initWithString(const std::string& str, int priority)
{
    _string = str;
    Director::getInstance()->getScheduler().schedule(UpdateJob(this, priority).paused(isPaused()));
}

TestNode::~TestNode()
{
}

void TestNode::update(float /*dt*/)
{
    log("%s", _string.c_str());
}

//------------------------------------------------------------------
//
// SchedulerUpdate
//
//------------------------------------------------------------------
void SchedulerUpdate::onEnter()
{
    SchedulerTestLayer::onEnter();

    auto d = new (std::nothrow) TestNode();
    d->initWithString("---", 50);
    addChild(d);
    d->release();

    auto b = new (std::nothrow) TestNode();
    b->initWithString("3rd", 8);
    addChild(b);
    b->release();

    auto a = new (std::nothrow) TestNode();
    a->initWithString("1st", 0);
    addChild(a);
    a->release();

    auto c = new (std::nothrow) TestNode();
    c->initWithString("4th", 9);
    addChild(c);
    c->release();

    auto e = new (std::nothrow) TestNode();
    e->initWithString("5th", 20);
    addChild(e);
    e->release();

    auto f = new (std::nothrow) TestNode();
    f->initWithString("2nd", 5);
    addChild(f);
    f->release();

    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerUpdate::removeUpdates)
            .delay(4.0)
    );
}

void SchedulerUpdate::removeUpdates(float /*dt*/)
{
    auto& children = getChildren();

    for (auto& c : children)
    {
        auto obj = static_cast<Ref*>(c.get());
        auto node = static_cast<Node*>(obj);
        
        if (! node)
        {
            break;
        }
        Director::getInstance()->getScheduler().unscheduleAllForTarget(node);
    }
    Director::getInstance()->getScheduler().unscheduleAllForTarget(this);
}

std::string SchedulerUpdate::title() const
{
    return "Schedule update with priority";
}

std::string SchedulerUpdate::subtitle() const
{
    return "3 scheduled updates. Priority should work. Stops in 4s. See console";
}

//------------------------------------------------------------------
//
// SchedulerUpdateAndCustom
//
//------------------------------------------------------------------
float SchedulerUpdateAndCustom::getDuration() const
{
    return 0.15f;
}

void SchedulerUpdateAndCustom::onEnter()
{
    SchedulerTestLayer::onEnter();

    Director::getInstance()->getScheduler().schedule(UpdateJob(this).paused(isPaused()));
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerUpdateAndCustom::tick)
            .paused(isPaused())
    );
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerUpdateAndCustom::stopSelectors)
            .delay(0.1f)
            .paused(isPaused())
    );
}

void SchedulerUpdateAndCustom::update(float dt)
{
    CCLOG("update called:%f", dt);
}

void SchedulerUpdateAndCustom::tick(float dt)
{
    CCLOG("custom selector called:%f",dt);
}

void SchedulerUpdateAndCustom::stopSelectors(float /*dt*/)
{
    log("SchedulerUpdateAndCustom::stopSelectors");
    Director::getInstance()->getScheduler().unscheduleAllForTarget(this);
}

std::string SchedulerUpdateAndCustom::title() const
{
    return "Schedule Update + custom selector";
}

std::string SchedulerUpdateAndCustom::subtitle() const
{
    return "Update + custom selector at the same time. Stops in 0.1 s. See console";
}

//------------------------------------------------------------------
//
// SchedulerUpdateFromCustom
//
//------------------------------------------------------------------
void SchedulerUpdateFromCustom::onEnter()
{
    SchedulerTestLayer::onEnter();

    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerUpdateFromCustom::schedUpdate)
            .repeat(0)
            .delay(2.0f)
            .paused(isPaused())
    );
}

void SchedulerUpdateFromCustom::update(float dt)
{
    CCLOG("update called:%f", dt);
}

void SchedulerUpdateFromCustom::schedUpdate(float /*dt*/)
{
    Director::getInstance()->getScheduler().schedule(UpdateJob(this).paused(isPaused()));
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerUpdateFromCustom::stopUpdate, 0)
            .delay(2.0f)
            .interval(2.0f)
            .paused(isPaused())
    );
}

void SchedulerUpdateFromCustom::stopUpdate(float /*dt*/)
{
    Director::getInstance()->getScheduler().unscheduleUpdateJob(this);
    Director::getInstance()->getScheduler().unscheduleTimedJob(this, 0);
}

std::string SchedulerUpdateFromCustom::title() const
{
    return "Schedule Update in 2 sec";
}

std::string SchedulerUpdateFromCustom::subtitle() const
{
    return "Update schedules in 2 secs. Stops 2 sec later. See console";
}

//------------------------------------------------------------------
//
// RescheduleSelector
//
//------------------------------------------------------------------
void RescheduleSelector::onEnter()
{
    SchedulerTestLayer::onEnter();

    _interval = 1.0f;
    _ticks    = 0;
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &RescheduleSelector::schedUpdate, 0)
            .delay(_interval)
            .interval(_interval)
            .paused(isPaused())
    );
}

std::string RescheduleSelector::title() const
{
    return "Reschedule Selector";
}

std::string RescheduleSelector::subtitle() const
{
    return "Interval is 1 second, then 2, then 3...";
}

void RescheduleSelector::schedUpdate(float dt)
{
    _ticks++;

    CCLOG("schedUpdate: %.4f", dt);
    if ( _ticks > 3 )
    {
        _interval += 1.0f;
        Director::getInstance()->getScheduler().schedule(
            TimedJob(this, &RescheduleSelector::schedUpdate, 0)
                .delay(_interval)
                .interval(_interval)
                .paused(isPaused())
        );
        _ticks = 0;
    }
}

// SchedulerDelayAndRepeat

void SchedulerDelayAndRepeat::onEnter()
{
    SchedulerTestLayer::onEnter();
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerDelayAndRepeat::update, 0)
            .delay(3.0f)
            .repeat(4)
            .paused(isPaused())
    );
    CCLOG("update is scheduled should begin after 3 seconds");
}

std::string SchedulerDelayAndRepeat::title() const
{
    return "Schedule with delay of 3 sec, repeat 4 times";
}

std::string SchedulerDelayAndRepeat::subtitle() const
{
    return "After 5 x executed, method unscheduled. See console";
}

void SchedulerDelayAndRepeat::update(float dt)
{
    log("update called:%f", dt);
}

// SchedulerSpeedup

ControlSlider* SchedulerSpeedup::sliderCtl()
{
    ControlSlider * slider = ControlSlider::create("GUI/sliderTrack2.png","GUI/sliderProgress2.png" ,"GUI/sliderThumb.png");

    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(SchedulerSpeedup::sliderAction), Control::EventType::VALUE_CHANGED);

    slider->setMinimumValue(-9.01f); // 10x slower
    slider->setMaximumValue(9.0f);   // 10x faster
    slider->setValue(0.0f);

    return slider;
}

void SchedulerSpeedup::sliderAction(Ref* sender, Control::EventType /*controlEvent*/)
{
    ControlSlider* pSliderCtl = static_cast<ControlSlider*>(sender);
    float scale;
    scale = pSliderCtl->getValue();
    if (scale < 0.0f) {
        scale = 1.0f / (-scale + 1.0f);
    } else {
        scale += 1.0f;
    }

    Director::getInstance()->getScheduler().setSpeedup(scale);
}

void SchedulerSpeedup::onEnter()
{
    SchedulerTestLayer::onEnter();

    auto s = Director::getInstance()->getWinSize();

    // rotate and jump
    auto jump1 = std::make_unique<JumpBy>(4, Vec2(-s.width+80,0), 100, 4);
    auto jump2 = std::unique_ptr<JumpBy>(jump1->reverse());
    auto rot1 = std::make_unique<RotateBy>(4, 360*2);
    auto rot2 = std::unique_ptr<RotateBy>(rot1->reverse());

    auto seq3_1 = std::make_unique<Sequence>(
        std::move(jump2),
        std::move(jump1)
    );
    auto seq3_2 = std::make_unique<Sequence>(
        std::move(rot1),
        std::move(rot2)
    );
    auto spawn = std::make_unique<Spawn>( std::move(seq3_1), std::move(seq3_2));
    auto action = std::make_unique<Repeat>( std::move(spawn), 50);

    auto action2 = std::unique_ptr<Repeat>(action->clone());
    auto action3 = std::unique_ptr<Repeat>(action->clone());

    auto grossini = Sprite::create("Images/grossini.png");
    auto tamara = Sprite::create("Images/grossinis_sister1.png");
    auto kathia = Sprite::create("Images/grossinis_sister2.png");

    grossini->setPosition(Vec2(40,80));
    tamara->setPosition(Vec2(40,80));
    kathia->setPosition(Vec2(40,80));

    addChild(grossini);
    addChild(tamara);
    addChild(kathia);

    grossini->runAction(std::make_unique<Speed>(std::move(action), 0.5f));
    tamara->runAction(std::make_unique<Speed>(std::move(action2), 1.5f));
    kathia->runAction(std::make_unique<Speed>(std::move(action3), 1.0f));

    auto emitter = ParticleFireworks::create();
    emitter->setTexture( Director::getInstance()->getTextureCache()->addImage(s_stars1) );
    addChild(emitter);

    _sliderCtl = sliderCtl();
    _sliderCtl->setPosition(Vec2(s.width / 2.0f, s.height / 3.0f));

    addChild(_sliderCtl);
}

void SchedulerSpeedup::onExit()
{
    // restore scale
    Director::getInstance()->getScheduler().setSpeedup(1);
    SchedulerTestLayer::onExit();
}

std::string SchedulerSpeedup::title() const
{
    return "Scheduler timeScale Test";
}

std::string SchedulerSpeedup::subtitle() const
{
    return "Fast-forward and rewind using scheduler.timeScale";
}

class TestNode2 : public Node
{
public:
    static TestNode2* create()
    {
        auto ret = new TestNode2;
        ret->init();
        ret->autorelease();
        return ret;
    }

	~TestNode2() {
		cocos2d::log("Delete TestNode (should not crash)");
		Director::getInstance()->getScheduler().unscheduleAllForTarget(this);
	}

	void update(float /*dt*/) {
	}
};

void SchedulerIssue2268::onEnter()
{
	SchedulerTestLayer::onEnter();

	testNode = TestNode2::create();
	testNode->retain();
	Director::getInstance()->getScheduler().schedule(
        TimedJob(static_cast<TestNode2*>(testNode), &TestNode2::update, 0)
            .paused(isPaused())
    );

	this->addChild(testNode);

	Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerIssue2268::update, 0)
            .delay(0.25f)
            .paused(isPaused())
    );
}

void SchedulerIssue2268::update(float /*dt*/)
{
	if ( testNode != nullptr ) {
		// do something with testNode

		// at some point we are done, pause the nodes actions and schedulers
		testNode->removeFromParentAndCleanup(false);

		// at some other point we are completely done with the node and want to clear it
		Director::getInstance()->getScheduler().unscheduleAllForTarget(testNode);
		testNode->release();
		testNode = nullptr;

	}
}
SchedulerIssue2268::~SchedulerIssue2268()
{
    CC_SAFE_RELEASE(testNode);
}

std::string SchedulerIssue2268::title() const
{
    return "Issue #2268";
}

std::string SchedulerIssue2268::subtitle() const
{
    return "Should not crash";
}

// ScheduleCallbackTest

ScheduleCallbackTest::~ScheduleCallbackTest()
{
    
}

std::string ScheduleCallbackTest::title() const
{
    return "ScheduleCallbackTest";
}

std::string ScheduleCallbackTest::subtitle() const
{
    return "\n\n\n\nPlease see console.\n\
schedule(TimedJob(this, lambda) ...)\n\
schedule(TimedJob(this, &XXX::member) ...)\n\
schedule(TimedJob(this, global_function) ...)\n\
";
}

static void ScheduleCallbackTest_global_callback(float dt)
{
    log("In the callback of schedule(TimedJob(this, global_function) ...), dt = %f", dt);
}

void ScheduleCallbackTest::onEnter()
{
    SchedulerTestLayer::onEnter();

    auto lambda = [](float dt){
        log("In the callback of schedule(TimedJob(this, lambda) ...), dt = %f", dt);
    };
    
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, lambda).delay(1.0f).interval(1.0f)
    );
    
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &ScheduleCallbackTest::callback).delay(1.0f).interval(1.0f)
    );
    
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, ScheduleCallbackTest_global_callback).delay(1.0f).interval(1.0f)
    );
}

void ScheduleCallbackTest::callback(float dt)
{
    log("In the callback of schedule(TimedJob(this, &XXX::member) ...), dt = %f", dt);
}


// ScheduleUpdatePriority

std::string ScheduleUpdatePriority::title() const
{
    return "ScheduleUpdatePriorityTest";
}

std::string ScheduleUpdatePriority::subtitle() const
{
    return "click to change update priority with random value";
}

bool ScheduleUpdatePriority::onTouchBegan(Touch* /*touch*/, Event* /*event*/)
{
    int priority = static_cast<int>(CCRANDOM_0_1() * 11);
    CCLOG("change update priority to %d", priority);
    Director::getInstance()->getScheduler().schedule(UpdateJob(this, priority).paused(isPaused()));
    return true;
}

void ScheduleUpdatePriority::onEnter()
{
    SchedulerTestLayer::onEnter();
    
    Director::getInstance()->getScheduler().schedule(UpdateJob(this).paused(isPaused()));

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(ScheduleUpdatePriority::onTouchBegan, this);
    _director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void ScheduleUpdatePriority::onExit()
{
    Node::onExit();
    Director::getInstance()->getScheduler().unscheduleUpdateJob(this);
}

void ScheduleUpdatePriority::update(float /*dt*/)
{
}

void SchedulerIssue10232::onEnter()
{
    SchedulerTestLayer::onEnter();

    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SchedulerIssue10232::update).delay(0.25f).repeat(0).paused(isPaused())
    );
    
    auto lambda = [](float /*dt*/){
            log("SchedulerIssue10232:Schedules a lambda function");
    };

    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, lambda).delay(0.25f).repeat(0).paused(isPaused())
    );
}

void SchedulerIssue10232::update(float /*dt*/)
{
    log("SchedulerIssue10232:Schedules a selector");
}

std::string SchedulerIssue10232::title() const
{
    return "Issue #10232";
}

std::string SchedulerIssue10232::subtitle() const
{
    return "Should not crash";
}
