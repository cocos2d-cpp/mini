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

#ifndef __UIHBox_H__
#define __UIHBox_H__

#include "ui/UILayout.h"
#include "ui/GUIExport.h"

namespace cocos2d {

/**
 * @addtogroup ui
 * @{
 */
namespace ui {

/**
 * HBox is just a convenient wrapper class for horizontal layout type.
 * HBox lays out its children in a single horizontal row.
 */
class CC_GUI_DLL HBox : public Layout{
public:
    
    /**
     * Default constructor
     * @js ctor
     * @lua new
     */
    HBox();
    
    /**
     * Default destructor
     * @js NA
     * @lua NA
     */
    virtual ~HBox();
    
    /**
     * Create a empty HBox instance.
     * @return A HBox instance pointer.
     */
    static HBox* create();
    
    /**
     *Create a HBox with a certain size.
     *@param size The content size of the layout.
     *@return A HBox instance pointer.
     */
    static HBox* create(const Size& size);
    
protected:
    //initializes state of widget.
    virtual bool init() override;
    virtual bool initWithSize(const Size& size);
};
    
}

// end of ui group
/// @}
} // namespace cocos2d

#endif /* defined(__UIHBox__) */
