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

#ifndef __CCSCENE_H__
#define __CCSCENE_H__

#include <string>
#include "2d/CCNode.h"

namespace cocos2d {

class Camera;
class BaseLight;
class Renderer;
class EventListenerCustom;
class EventCustom;

/**
 * @addtogroup _2d
 * @{
 */

/** @class Scene
* @brief Scene is a subclass of Node that is used only as an abstract concept.

Scene and Node are almost identical with the difference that Scene has its
anchor point (by default) at the center of the screen.

For the moment Scene has no other logic than that, but in future releases it might have
additional logic.

It is a good practice to use a Scene as the parent of all your nodes.
 
Scene will create a default camera for you.
*/
class CC_DLL Scene : public Node
{
public:
    /** Creates a new Scene object. 
     *
     * @return An autoreleased Scene object.
     */
    static Scene *create();

    /** Creates a new Scene object with a predefined Size. 
     *
     * @param size The predefined size of scene.
     * @return An autoreleased Scene object.
     */
    static Scene *createWithSize(const Size& size);

    using Node::addChild;
    virtual std::string getDescription() const override;
    
    /** Get all cameras.
     * @return The vector of all cameras, ordered by camera depth.
     */
    const std::vector<Camera*>& getCameras();

    Camera* getDefaultCamera() const;

    /** Get lights.
     * @return The vector of lights.
     */
    const std::vector<BaseLight*>& getLights() const { return _lights; }

    /** Render the scene.
     * @param renderer The renderer use to render the scene.
     * @param eyeTransform The AdditionalTransform of camera.
     * @param eyeProjection The projection matrix of camera.
     */
    virtual void render(Renderer* renderer, const Mat4& eyeTransform, const Mat4* eyeProjection = nullptr);

    /** Render the scene.
     * @param renderer The renderer use to render the scene.
     * @param eyeTransforms The AdditionalTransform List of camera of multiView.
     * @param eyeProjections The projection matrix List of camera of multiView.
     * @param multiViewCount The number of multiView.
     */
    virtual void render(Renderer* renderer, const Mat4* eyeTransforms, const Mat4* eyeProjections, unsigned int multiViewCount);

    /** override function */
    virtual void removeAllChildren() override;
    
protected:
    Scene();
    virtual ~Scene();
    
    bool init() override;
    bool initWithSize(const Size& size);
    
    void setCameraOrderDirty() { _cameraOrderDirty = true; }
    
    void onProjectionChanged(EventCustom* event);

protected:
    friend class Node;
    friend class ProtectedNode;
    friend class SpriteBatchNode;
    friend class Camera;
    friend class BaseLight;
    friend class Renderer;
    
    std::vector<Camera*> _cameras; //weak ref to Camera
    NodeId               _defaultCameraId; //default camera created by scene, _cameras[0], Caution that the default camera can not be added to _cameras before onEnter is called
    bool                 _cameraOrderDirty; // order is dirty, need sort
    EventListenerCustom*       _event;

    std::vector<BaseLight *> _lights;
    
private:
    Scene(const Scene &) = delete;
    const Scene & operator=(const Scene &) = delete;
};

} // namespace cocos2d

#endif // __CCSCENE_H__
