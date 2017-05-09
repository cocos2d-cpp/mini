/*
 * Copyright (C) 2009 Matt Oswald
 * Copyright (c) 2009-2010 Ricardo Quesada
 * Copyright (c) 2010-2012 cocos2d-x.org
 * Copyright (c) 2011      Zynga Inc.
 * Copyright (c) 2011      Marco Tillemans
 * Copyright (c) 2013-2016 Chukong Technologies Inc.
 * Copyright (c) 2017      Iakov Sergeev <yahont@github>
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
#ifndef __CCPARTICLEBATCHNODE_H__
#define __CCPARTICLEBATCHNODE_H__

#include "2d/CCNode.h"
#include "base/CCProtocols.h"
#include "renderer/CCBatchCommand.h"

namespace cocos2d {

class Texture2D;
class TextureAtlas;
class ParticleSystem;

/**
 * @addtogroup _2d
 * @{
 */

#define kParticleDefaultCapacity 500

/** @class ParticleBatchNode
 * @brief ParticleBatchNode is like a batch node: if it contains children, it will draw them in 1 single OpenGL call
 * (often known as "batch draw").
 *
 * A ParticleBatchNode can reference one and only one texture (one image file, one texture atlas).
 * Only the ParticleSystems that are contained in that texture can be added to the SpriteBatchNode.
 * All ParticleSystems added to a SpriteBatchNode are drawn in one OpenGL ES draw call.
 * If the ParticleSystems are not added to a ParticleBatchNode then an OpenGL ES draw call will be needed for each one, which is less efficient.
 *
 *
 * Limitations:
 * - At the moment only ParticleSystemQuad is supported
 * - All systems need to be drawn with the same parameters, blend function, aliasing, texture
 *
 * Most efficient usage
 * - Initialize the ParticleBatchNode with the texture and enough capacity for all the particle systems
 * - Initialize all particle systems and add them as child to the batch node
 * @since v1.1
 */

class CC_DLL ParticleBatchNode : public Node
{
public:
    /** Create the particle system with Texture2D, a capacity of particles, which particle system to use.
     *
     * @param tex A given texture.
     * @param capacity A capacity of particles.
     * @return An autoreleased ParticleBatchNode object.
     * @js NA
     */
    static ParticleBatchNode* createWithTexture(const Texture2D *tex, int capacity = kParticleDefaultCapacity);

    /** Create the particle system with the name of a file on disk (for a list of supported formats look at the Texture2D class), a capacity of particles.
     *
     * @param fileImage A given file name.
     * @param capacity A capacity of particles.
     * @return An autoreleased ParticleBatchNode object.
     */
    static ParticleBatchNode* create(const std::string& fileImage, int capacity = kParticleDefaultCapacity);

    /** Inserts a child into the ParticleBatchNode.
     *
     * @param system A given particle system.
     * @param index The insert index.
     */
    void insertChild(ParticleSystem* system, int index);

    /** Remove a child of the ParticleBatchNode.
     *
     * @param index The index of the child.
     * @param doCleanup True if all actions and callbacks on this node should be removed, false otherwise.
     */
    void removeChildAtIndex(int index, bool doCleanup);
    void removeAllChildrenWithCleanup(bool doCleanup) override;

    /** Disables a particle by inserting a 0'd quad into the texture atlas.
     *
     * @param particleIndex The index of the particle.
     */
    void disableParticle(int particleIndex);

    /** Gets the texture atlas used for drawing the quads.
     *
     * @return The texture atlas used for drawing the quads.
     */
    TextureAtlas* getTextureAtlas() const { return _textureAtlas; }
    
    /** Sets the texture atlas used for drawing the quads.
     *
     * @param atlas The texture atlas used for drawing the quads.
     */
    void setTextureAtlas(TextureAtlas* atlas) { _textureAtlas = atlas; }
    
    // Overrides
    virtual void visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;

    using Node::addChild;
    virtual NodeId addChild(node_ptr<Node> child, int zOrder, int tag) override;
    virtual NodeId addChild(node_ptr<Node> child, int zOrder, const std::string &name) override;
    CC_DEPRECATED_ATTRIBUTE node_ptr<Node> removeChild(Node* child, bool cleanup = true) override
    {
        if (child != nullptr)
            return removeChild(child->getNodeId(), cleanup);
        return node_ptr<Node>();
    }
    virtual node_ptr<Node> removeChild(NodeId id, bool cleanup) override;
    virtual void reorderChild(Node * child, int zOrder) override;
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    virtual const Texture2D* getTexture(void) const;
    virtual void setTexture(const Texture2D *texture);
    /**
    * @code
    * When this function bound into js or lua,the parameter will be changed
    * In js: var setBlendFunc(var src, var dst)
    */
    virtual void setBlendFunc(const BlendFunc &blendFunc);

    virtual const BlendFunc& getBlendFunc(void) const;
    
protected:
    ParticleBatchNode();

    virtual ~ParticleBatchNode();
    
    /** initializes the particle system with Texture2D, a capacity of particles */
    bool initWithTexture(const Texture2D *tex, int capacity);
    
    /** initializes the particle system with the name of a file on disk (for a list of supported formats look at the Texture2D class), a capacity of particles */
    bool initWithFile(const std::string& fileImage, int capacity);
    
private:

    void updateAllAtlasIndexes();
    void increaseAtlasCapacityTo(ssize_t quantity);
    int searchNewPositionInChildrenForZ(int z);
    void getCurrentIndex(int* oldIndex, int* newIndex, Node* child, int z);
    NodeId addChildByTagOrName(node_ptr<ParticleSystem> child, int z, int tag, const std::string &name, bool setTag);
    int addChildHelper(node_ptr<ParticleSystem> child, int z, int aTag, const std::string &name, bool setTag);
    void updateBlendFunc(void);
    /** the texture atlas used for drawing the quads */
    TextureAtlas* _textureAtlas;

    /** the blend function used for drawing the quads */
    BlendFunc _blendFunc;
    // quad command
    BatchCommand _batchCommand;
};

// end of _2d group
/// @}

} // namespace cocos2d

#endif /* __CCPARTICLEBATCHNODE_H__ */
