/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2016 Chukong Technologies Inc.
Copyright (c) 2017      Iakov Sergeev <yahont@github>

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

#include "2d/CCAction.h"
#include "base/CCDirector.h"

#include <sstream>

namespace cocos2d {
//
// Action Base Class
//

Action::Action()
    : _target(nullptr)
    , _tag(Action::INVALID_TAG)
    , _flags(0)
    , _hasStopped(true)
{
}

Action::~Action()
{
    CCLOGINFO("deallocing Action: %p - tag: %i", this, _tag);
}

std::string Action::description() const
{
    std::ostringstream os;
    os << "Action | Tag = " << _tag;
    return os.str();
}

void Action::startWithTarget(Node *aTarget)
{
    CC_ASSERT(aTarget);
    _target = aTarget;
    _hasStopped = false;
}

void Action::stop()
{
    if (!_hasStopped)
    {
        at_stop();
        _hasStopped = true;
    }
}

//
// Follow
//

Follow::Follow(Node* followedNode, float xOffset, float yOffset, const Rect& rect)
    : _followedNode(to_node_ptr(followedNode))
    , _boundarySet(!rect.equals(Rect::ZERO))
    , _boundaryFullyCovered(false)
    , _leftBoundary(0.0)
    , _rightBoundary(0.0)
    , _topBoundary(0.0)
    , _bottomBoundary(0.0)
    , _offsetX(xOffset)
    , _offsetY(yOffset)
    , _worldRect(rect)
{
    CC_ASSERT(followedNode);
 
    Size winSize = Director::getInstance()->getWinSize();
    _fullScreenSize.set(winSize.width, winSize.height);
    _halfScreenSize = _fullScreenSize * 0.5f;
    _halfScreenSize.x += _offsetX;
    _halfScreenSize.y += _offsetY;
    
    if (_boundarySet)
    {
        _leftBoundary = -((rect.origin.x+rect.size.width) - _fullScreenSize.x);
        _rightBoundary = -rect.origin.x ;
        _topBoundary = -rect.origin.y;
        _bottomBoundary = -((rect.origin.y+rect.size.height) - _fullScreenSize.y);

        if(_rightBoundary < _leftBoundary)
        {
            // screen width is larger than world's boundary width
            //set both in the middle of the world
            _rightBoundary = _leftBoundary = (_leftBoundary + _rightBoundary) / 2;
        }
        if(_topBoundary < _bottomBoundary)
        {
            // screen width is larger than world's boundary width
            //set both in the middle of the world
            _topBoundary = _bottomBoundary = (_topBoundary + _bottomBoundary) / 2;
        }

        if( (_topBoundary == _bottomBoundary) && (_leftBoundary == _rightBoundary) )
        {
            _boundaryFullyCovered = true;
        }
    }
}

Follow* Follow::clone() const
{
    return new Follow(_followedNode.get(), _offsetX, _offsetY, _worldRect);
}

Follow* Follow::reverse() const
{
    return new Follow(_followedNode.get(), _offsetX, _offsetY, _worldRect);
}

void Follow::update(float /*dt*/)
{
    if(_boundarySet)
    {
        // whole map fits inside a single screen, no need to modify the position - unless map boundaries are increased
        if(_boundaryFullyCovered)
        {
            return;
        }

        Vec2 tempPos = _halfScreenSize - _followedNode->getPosition();

        getTarget()->setPosition(clampf(tempPos.x, _leftBoundary, _rightBoundary),
                                   clampf(tempPos.y, _bottomBoundary, _topBoundary));
    }
    else
    {
        getTarget()->setPosition(_halfScreenSize - _followedNode->getPosition());
    }
}

bool Follow::isDone() const
{
    return ( _followedNode->isPaused() );
}

void Follow::step(float)
{
}

void Follow::at_stop()
{
}

} // namespace cocos2d


