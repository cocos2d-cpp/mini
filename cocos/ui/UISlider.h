/****************************************************************************
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

#ifndef __UISLIDER_H__
#define __UISLIDER_H__

#include "ui/UIWidget.h"
#include "ui/GUIExport.h"

namespace cocos2d {

/**
 * @addtogroup ui
 * @{
 */

class Sprite;

namespace ui {

class Scale9Sprite;
    
/** 
 * @brief UI Slider widget.
 */
class CC_GUI_DLL Slider : public Widget
{
    
    DECLARE_CLASS_GUI_INFO
    
public:
    /** EventType Slider event type.
     */
    enum class EventType
    {
        ON_PERCENTAGE_CHANGED,
        //@since v3.7
        ON_SLIDEBALL_DOWN,
        ON_SLIDEBALL_UP,
        ON_SLIDEBALL_CANCEL
    };
    typedef std::function<void(Ref*,EventType)> ccSliderCallback;

    Slider();

    Slider(const std::string& barTextureName,
           const std::string& normalBallTextureName,
           TextureResType resType = TextureResType::LOCAL);
    
    virtual ~Slider();
    
    static Slider* create();
    
    static Slider* create(const std::string& barTextureFileName,
                          const std::string& normalBallTextureFileName,
                          TextureResType resType = TextureResType::LOCAL);
    
    /**
     * Load texture for slider bar.
     *
     * @param fileName   File name of texture.
     * @param resType    @see TextureResType .
     */
    void loadBarTexture(const std::string& fileName,TextureResType resType = TextureResType::LOCAL);
    
    /**
     * Sets if slider is using scale9 renderer.
     * @param able True that using scale9 renderer, false otherwise.
     */
    void setScale9Enabled(bool able);
    
    /**
     * Gets If slider is using scale9 renderer.
     * @return True that using scale9 renderer, false otherwise.
     */
    bool isScale9Enabled()const;
    
    /**
     * Sets capinsets for slider, if slider is using scale9 renderer.
     * @param capInsets Capinsets for slider.
     */
    void setCapInsets(const Rect &capInsets);
    
    /**
     * Sets capinsets for bar slider, if slider is using scale9 renderer.
     * @param capInsets Capinsets for bar slider.
     */
    void setCapInsetsBarRenderer(const Rect &capInsets);
    
    /**
     * Gets capinsets for bar slider, if slider is using scale9 renderer.
     * @return capInsets Capinsets for bar slider.
     */
    const Rect& getCapInsetsBarRenderer()const;
    
    /**
     * Sets capinsets for progress bar slider, if slider is using scale9 renderer.
     * @param capInsets Capinsets for progress bar slider.
     * @js NA
     */
    void setCapInsetProgressBarRenderer(const Rect &capInsets);
    
    /**
     * Gets capinsets for progress bar slider, if slider is using scale9 renderer.
     * @return Capinsets for progress bar slider.
     * @js NA
     */
    const Rect& getCapInsetsProgressBarRenderer()const;
    
    /**
     * Load textures for slider ball.
     *
     * @param normal     Normal state texture.
     * @param pressed    Pressed state texture.
     * @param disabled    Disabled state texture.
     * @param texType    @see TextureResType .
     */
    void loadSlidBallTextures(const std::string& normal,
                              const std::string& pressed = "",
                              const std::string& disabled = "",
                              TextureResType texType = TextureResType::LOCAL);
    
    /**
     * Load normal state texture for slider ball.
     *
     * @param normal    Normal state texture.
     * @param resType    @see TextureResType .
     */
    void loadSlidBallTextureNormal(const std::string& normal,TextureResType resType = TextureResType::LOCAL);
    
    /**
     * Load pressed state texture for slider ball.
     *
     * @param pressed    Pressed state texture.
     * @param resType    @see TextureResType .
     */
    void loadSlidBallTexturePressed(const std::string& pressed,TextureResType resType = TextureResType::LOCAL);
    
    /**
     * Load disabled state texture for slider ball.
     *
     * @param disabled   Disabled state texture.
     * @param resType    @see TextureResType .
     */
    void loadSlidBallTextureDisabled(const std::string& disabled,TextureResType resType = TextureResType::LOCAL);
    
    /**
     * Load dark state texture for slider progress bar.
     *
     * @param fileName   File path of texture.
     * @param resType    @see TextureResType .
     */
    void loadProgressBarTexture(const std::string& fileName, TextureResType resType = TextureResType::LOCAL);
    
    /**
     * Changes the progress direction of slider.
     * @param percent  Percent value from 1 to 100.
     */
    void setPercent(int percent);
    
    /**
     * Gets the progress direction of slider.
     *
     * @return percent Percent value from 1 to 100.
     */
    int getPercent()const;

    /**
     * Set a large value could give more control to the precision.
     * @since v3.7
     * @param percent The max percent of Slider.
     */
    void setMaxPercent(int percent);

    /**
     * Query the maximum percent of Slider. The default value is 100.
     * @since v3.7
     * @return The maximum percent of the Slider.
     */
    int getMaxPercent()const;

    /**
     * Add call back function called when slider's percent has changed to slider.
     *
     * @param callback An given call back function called when slider's percent has changed to slider.
     */
    void addEventListener(const ccSliderCallback& callback);

    virtual bool onTouchBegan(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchMoved(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchEnded(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchCancelled(Touch *touch, Event *unusedEvent) override;
    
    //override "getVirtualRendererSize" method of widget.
    virtual Size getVirtualRendererSize() const override;
    
    //override "getVirtualRenderer" method of widget.
    virtual Node* getVirtualRenderer() override;
    
    //override "ignoreContentAdaptWithSize" method of widget.
    virtual void ignoreContentAdaptWithSize(bool ignore) override;
    
    //override the widget's hitTest function to perform its own
    virtual bool hitTest(const Vec2 &pt, const Camera* camera, Vec3 *p) const override;
    /**
     * Returns the "class name" of widget.
     */
    virtual std::string getDescription() const override;
    
    /** When user pressed the button, the button will zoom to a scale.
     * The final scale of the button  equals (button original scale + _zoomScale)
     * @since v3.3
     */
    void setZoomScale(float scale);
    /**
     * @brief Return a zoom scale
     * @since v3.3
     */
    float getZoomScale()const;

private:

    virtual void initRenderer() override;
    float getPercentWithBallPos(const Vec2 &pt) const;
    void percentChangedEvent(EventType event);
    virtual void onPressStateChangedToNormal() override;
    virtual void onPressStateChangedToPressed() override;
    virtual void onPressStateChangedToDisabled() override;
    virtual void onSizeChanged() override;

    void setupBarTexture();
    void loadBarTexture(SpriteFrame* spriteframe);
    void setupProgressBarTexture();
    void loadProgressBarTexture(SpriteFrame* spriteframe);
    void loadSlidBallTextureNormal(SpriteFrame* spriteframe);
    void loadSlidBallTexturePressed(SpriteFrame* spriteframe);
    void loadSlidBallTextureDisabled(SpriteFrame* spriteframe);
   
    void barRendererScaleChangedWithSize();
    void progressBarRendererScaleChangedWithSize();
    virtual Widget* createCloneInstance() const override;
    virtual void copySpecialProperties(const Widget* model) override;
    virtual void adaptRenderers() override;

protected:
    Scale9Sprite*  _barRenderer;
    Scale9Sprite* _progressBarRenderer;
    Size _barTextureSize;
    Size _progressBarTextureSize;
    
    NodeId _slidBallNormalRendererId;
    NodeId _slidBallPressedRendererId;
    NodeId _slidBallDisabledRendererId;
    Node* _slidBallRenderer;
    
    float _barLength;
    int _percent;
    int _maxPercent;
    
    bool _scale9Enabled;
    bool _prevIgnoreSize;
    
    float _zoomScale;
    float _sliderBallNormalTextureScaleX;
    float _sliderBallNormalTextureScaleY;

    bool _isSliderBallPressedTextureLoaded;
    bool _isSliderBallDisabledTexturedLoaded;

    Rect _capInsetsBarRenderer;
    Rect _capInsetsProgressBarRenderer;

    Ref*       _sliderEventListener;

    ccSliderCallback  _eventCallback;

    TextureResType _barTexType;
    TextureResType _progressBarTexType;
    TextureResType _ballNTexType;
    TextureResType _ballPTexType;
    TextureResType _ballDTexType;
    bool _barRendererAdaptDirty;
    bool _progressBarRendererDirty;

    std::string _textureFile;
    std::string _progressBarTextureFile;
    std::string _slidBallNormalTextureFile;
    std::string _slidBallPressedTextureFile;
    std::string _slidBallDisabledTextureFile;
};

}

// end of ui group
/// @}

} // namespace cocos2d

#endif /* defined(__CocoGUI__Slider__) */