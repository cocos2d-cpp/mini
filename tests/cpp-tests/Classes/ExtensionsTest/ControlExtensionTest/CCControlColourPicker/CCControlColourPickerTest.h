#include "../CCControlScene.h"
#include "GUI/CCControlExtension/CCControlExtensions.h"

class ControlColourPickerTest : public ControlScene
{
public:
    ControlColourPickerTest();
    virtual ~ControlColourPickerTest();
    virtual bool init() override;
    /** Callback for the change value. */
    void colourValueChanged(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent);

    CC_SYNTHESIZE_RETAIN(cocos2d::Label*, _colorLabel, ColorLabel)

    static ControlColourPickerTest* create()
    {
        auto ret = new ControlColourPickerTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
};

