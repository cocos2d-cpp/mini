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

#include "2d/CCScene.h"
#include "base/CCDirector.h"
#include "2d/CCCamera.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerCustom.h"
#include "base/ccUTF8.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCFrameBuffer.h"

namespace cocos2d {

Scene::Scene()
{
    _ignoreAnchorPointForPosition = true;
    setAnchorPoint(Vec2(0.5f, 0.5f));
    
    _cameraOrderDirty = true;
    
    //create default camera
    _defaultCameraId = addChild( make_node_ptr<Camera>() );
    
    _event = Director::getInstance()->getEventDispatcher()->addCustomEventListener(Director::EVENT_PROJECTION_CHANGED, std::bind(&Scene::onProjectionChanged, this, std::placeholders::_1));
    _event->retain();
    
    Camera::_visitingCamera = nullptr;
}

Scene::~Scene()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(_event);
    CC_SAFE_RELEASE(_event);
}

bool Scene::init()
{
    auto size = Director::getInstance()->getWinSize();
    return initWithSize(size);
}

bool Scene::initWithSize(const Size& size)
{
    setContentSize(size);
    return true;
}

Scene* Scene::create()
{
    Scene *ret = new (std::nothrow) Scene();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

Scene* Scene::createWithSize(const Size& size)
{
    Scene *ret = new (std::nothrow) Scene();
    if (ret && ret->initWithSize(size))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

std::string Scene::getDescription() const
{
    return StringUtils::format("<Scene | tag = %d>", _tag);
}

void Scene::onProjectionChanged(EventCustom* /*event*/)
{
    if (auto defaultCamera = Director::getInstance()->getNodeRegister().get<Camera>(_defaultCameraId))
    {
        defaultCamera->initDefault();
    }
}

static bool camera_cmp(const Camera* a, const Camera* b)
{
    return a->getRenderOrder() < b->getRenderOrder();
}

const std::vector<Camera*>& Scene::getCameras()
{
    if (_cameraOrderDirty)
    {
        stable_sort(_cameras.begin(), _cameras.end(), camera_cmp);
        _cameraOrderDirty = false;
    }
    return _cameras;
}

Camera* Scene::getDefaultCamera() const
{
    return Director::getInstance()->getNodeRegister().get<Camera>(_defaultCameraId);
}

void Scene::render(Renderer* renderer, const Mat4& eyeTransform, const Mat4* eyeProjection)
{
    render(renderer, &eyeTransform, eyeProjection, 1);
}

void Scene::render(Renderer* renderer, const Mat4* eyeTransforms, const Mat4* eyeProjections, unsigned int multiViewCount)
{
    auto director = Director::getInstance();
    const auto& transform = getNodeToParentTransform();

    for (const auto& camera : getCameras())
    {
        if (!camera->isVisible())
            continue;

        Camera::_visitingCamera = camera;

        // There are two ways to modify the "default camera" with the eye Transform:
        // a) modify the "nodeToParentTransform" matrix
        // b) modify the "additional transform" matrix
        // both alternatives are correct, if the user manually modifies the camera with a camera->setPosition()
        // then the "nodeToParent transform" will be lost.
        // And it is important that the change is "permanent", because the matrix might be used for calculate
        // culling and other stuff.
        for (unsigned int i = 0; i < multiViewCount; ++i) {
            if (eyeProjections)
                camera->setAdditionalProjection(eyeProjections[i] * camera->getProjectionMatrix().getInversed());
            if (eyeTransforms)
                camera->setAdditionalTransform(eyeTransforms[i].getInversed());
            director->pushProjectionMatrix(i);
            director->loadProjectionMatrix(Camera::_visitingCamera->getViewProjectionMatrix(), i);
        }

        camera->apply();
        //clear background with max depth
        camera->clearBackground();
        //visit the scene
        visit(renderer, transform, 0);

        renderer->render();
        camera->restore();

        for (unsigned int i = 0; i < multiViewCount; ++i)
            director->popProjectionMatrix(i);

        // we shouldn't restore the transform matrix since it could be used
        // from "update" or other parts of the game to calculate culling or something else.
//        camera->setNodeToParentTransform(eyeCopy);
    }

    Camera::_visitingCamera = nullptr;
//    experimental::FrameBuffer::applyDefaultFBO();
}

void Scene::removeAllChildren()
{
    auto defaultCamera = removeChild(_defaultCameraId, true);

    Node::removeAllChildren();

    if (defaultCamera)
    {
        _defaultCameraId = addChild( std::move(defaultCamera) );
    }
}

} // namespace cocos2d
