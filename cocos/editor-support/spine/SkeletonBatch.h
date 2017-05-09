/******************************************************************************
 * Spine Runtimes Software License
 * Version 2.3
 *
 * Copyright (c) 2013-2015, Esoteric Software
 * All rights reserved.
 *
 * You are granted a perpetual, non-exclusive, non-sublicensable and
 * non-transferable license to use, install, execute and perform the Spine
 * Runtimes Software (the "Software") and derivative works solely for personal
 * or internal use. Without the written permission of Esoteric Software (see
 * Section 2 of the Spine Software License Agreement), you may not (a) modify,
 * translate, adapt or otherwise create derivative works, improvements of the
 * Software or develop new applications using the Software or (b) remove,
 * delete, alter or obscure any trademarks or any copyright, trademark, patent
 * or other intellectual property or proprietary rights notices on or in the
 * Software, including any copy thereof. Redistributions in binary or source
 * form must include this license and terms.
 *
 * THIS SOFTWARE IS PROVIDED BY ESOTERIC SOFTWARE "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL ESOTERIC SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#ifndef SPINE_SKELETONBATCH_H_
#define SPINE_SKELETONBATCH_H_

#include "spine/spine.h"
#include "renderer/CCTrianglesCommand.h"

namespace cocos2d {
    class Renderer;
    class GLProgramState;
    class Mat4;
}

namespace spine {
    
    class SkeletonBatch {
    public:
        static SkeletonBatch* getInstance ();
        
        static void destroyInstance ();
        
        void update (float delta);
        
        void addCommand (cocos2d::Renderer* renderer, float globalOrder, GLuint textureID, cocos2d::GLProgramState* glProgramState,
                         cocos2d::BlendFunc blendType, const cocos2d::TrianglesCommand:: Triangles& triangles, const cocos2d::Mat4& mv, uint32_t flags);
        
    protected:
        SkeletonBatch ();
        virtual ~SkeletonBatch ();
        
        class Command {
        public:
            Command ();
            virtual ~Command ();
            
            cocos2d::TrianglesCommand* trianglesCommand;
            cocos2d::TrianglesCommand::Triangles* triangles;
            Command* next;
        };
        
        Command* _firstCommand;
        Command* _command;
    };
    
}

#endif // SPINE_SKELETONBATCH_H_
