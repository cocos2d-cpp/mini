/*
 * Copyright (c) 2012 cocos2d-x.org
 * Copyright 2012 Yannick Loriot. All rights reserved.
 * Copyright (c) 2017 Iakov Sergeev <yahont@github>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#ifndef __CCCONTROLPOTENTIOMETER_H__
#define __CCCONTROLPOTENTIOMETER_H__

#include "CCControl.h"
#include "2d/CCProgressTimer.h"
#include "platform/CCPlatformDefine.h"

namespace cocos2d {
namespace extension {

/**
 * @addtogroup GUI
 * @{
 * @addtogroup control_extension
 * @{
 */

/** @class ControlPotentiometer Potentiometer control for Cocos2D. */
class CC_DLL ControlPotentiometer : public Control
{
public:
    ControlPotentiometer(const char* backgroundFile, const char* progressFile, const char* thumbFile);

    static ControlPotentiometer* create(const char* backgroundFile, const char* progressFile, const char* thumbFile)
    {
        auto pRet = new ControlPotentiometer(backgroundFile, progressFile, thumbFile);
        pRet->autorelease();
        return pRet;
    }

    void setValue(float value);
    float getValue();
     
    void setMinimumValue(float minimumValue);
    float getMinimumValue();

    void setMaximumValue(float maximumValue);
    float getMaximumValue();

    // Overrides
    virtual bool isTouchInside(Touch * touch) override;
    void setEnabled(bool enabled) override;
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent) override;
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent) override;
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent) override;

    /** Factorize the event dispatch into these methods. */
    void potentiometerBegan(Vec2 location);
    void potentiometerMoved(Vec2 location);
    void potentiometerEnded(Vec2 location);

    /** Returns the distance between the point1 and point2. */
    float distanceBetweenPointAndPoint(Vec2 point1, Vec2 point2);
    /** Returns the angle in degree between line1 and line2. */
    float angleInDegreesBetweenLineFromPoint_toPoint_toLineFromPoint_toPoint(
        Vec2 beginLineA, 
        Vec2 endLineA,
        Vec2 beginLineB,
        Vec2 endLineB);

protected:
    /** Contains the receiver’s current value. */
    float           _value;
    /** Contains the minimum value of the receiver.
     * The default value of this property is 0.0. */
    float           _minimumValue;
    /** Contains the maximum value of the receiver.
     * The default value of this property is 1.0. */
    float           _maximumValue;

private:
    NodeId _thumbSpriteId;
    NodeId _progressTimerId;
    Vec2   _previousLocation;
};

// end of GUI group
/// @}
/// @}

} // namespace extension
} // namespace cocos2d

#endif /* __CCCONTROLPOTENTIOMETER_H__ */
