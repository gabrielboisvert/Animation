#pragma once
#include "Animation.h"

#define MAX_BONE 100

namespace Renderer
{
    class Animator
    {
        public:
            std::vector<Maths::FMatrix4> mFinalBoneMatrices;
            Animation* mCurrentAnimation = nullptr;
            float mCurrentTime = 0;
            float animTime1 = 0;
            float animTime2 = 0;

            float mFadeEslapse = 0;
            float mDuration = 0;

            Animator(Animation* pAnimation);
            ~Animator();
            void updateAnimation(float pDeltaTime);
            void Animator::calculateBoneTransform(const AssimpNodeData* pNode, Maths::FMatrix4 pParentTransform);


            void crossFade(Animation* pAnimation1, Animation* pAnimation2, float duration);
            void updateCrossFade(Animation* pAnimation1, Animation* pAnimation2, float pDeltaTime);

            void blendTwoAnimations(Animation* pAnimation1, Animation* pAnimation2, float& pFactor, float pDeltaTime);
            void calculateBlendedBoneTransform(
                Animation* pAnimationBase, AssimpNodeData* node,
                Animation* pAnimationLayer, AssimpNodeData* nodeLayered,
                float currentTimeBase, float currentTimeLayered,
                Maths::FMatrix4 parentTransform,
                float blendFactor);
    };
}