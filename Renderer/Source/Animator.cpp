#include "Animator.h"
#include "Maths/Utils.h"
#include "Application.h"

using namespace Renderer;

Animator::Animator(Animation* pAnimation)
{
    mCurrentTime = 0.0;
    mCurrentAnimation = pAnimation;

    mFinalBoneMatrices.reserve(MAX_BONE);
    for (int i = 0; i < MAX_BONE; i++)
        mFinalBoneMatrices.push_back(Maths::FMatrix4(1.0f));
}

Animator::~Animator()
{

}

void Animator::updateAnimation(float pDeltaTime)
{
    if (mCurrentAnimation)
    {
        mCurrentTime += mCurrentAnimation->mTicksPerSecond * pDeltaTime;
        mCurrentTime = std::fmod(mCurrentTime, mCurrentAnimation->mDuration);
        calculateBoneTransform(&mCurrentAnimation->mRootNode, Maths::FMatrix4(1.0f));
    }
}

void Animator::calculateBoneTransform(const AssimpNodeData* pNode, Maths::FMatrix4 pParentTransform)
{
    std::string nodeName = pNode->mName;
    Maths::FMatrix4 nodeTransform = pNode->mTransformation;

    Bone* Bone = mCurrentAnimation->findBone(nodeName);

    if (Bone)
    {
        Bone->update(mCurrentTime);
        nodeTransform = Bone->mLocalTransform;
    }

    Maths::FMatrix4 globalTransformation = pParentTransform * nodeTransform;

    std::map<std::string, BoneInfo>& boneInfoMap = mCurrentAnimation->mBoneInfoMap;
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        int index = boneInfoMap[nodeName].mId;
        Maths::FMatrix4 offset = boneInfoMap[nodeName].mOffset;
        mFinalBoneMatrices[index] = globalTransformation * offset;
    }

    for (int i = 0; i < pNode->mChildrenCount; i++)
        calculateBoneTransform(pNode->mChildren[i], globalTransformation);
}

void Renderer::Animator::crossFade(Animation* pAnimation1, Animation* pAnimation2, float duration)
{
    mFadeEslapse = 0;
    mDuration = duration;
    //animTime1 = mCurrentAnimation
}

void Renderer::Animator::updateCrossFade(Animation* pAnimation1, Animation* pAnimation2, float pDeltaTime)
{
    float t = 0;
    if (mDuration != 0)
    {
        mFadeEslapse += pDeltaTime;
        t = Maths::unLerp(0, mDuration, mFadeEslapse);
    }

    Application::mLineDrawer->reset();

    blendTwoAnimations(pAnimation1, pAnimation2, t, pDeltaTime);

    Application::mLineDrawer->createVertexBuffer();
}

void Animator::blendTwoAnimations(Animation* pBaseAnimation, Animation* pLayeredAnimation, float& pFactor, float pDeltaTime)
{
    pFactor = Maths::clamp(pFactor, 0, 1);

    //Synchronize speed
    float b = pBaseAnimation->mDuration / pLayeredAnimation->mDuration;
    const float animSpeedMultiplierUp = Maths::lerp(1, b, pFactor);

    float a = pLayeredAnimation->mDuration / pBaseAnimation->mDuration;
    const float animSpeedMultiplierDown = Maths::lerp(a, 1, pFactor);


    //update animation time
    animTime1 += pBaseAnimation->mTicksPerSecond * pDeltaTime * animSpeedMultiplierUp;
    animTime1 = fmod(animTime1, pBaseAnimation->mDuration);

    
    animTime2 += pLayeredAnimation->mTicksPerSecond * pDeltaTime * animSpeedMultiplierDown;
    animTime2 = fmod(animTime2, pLayeredAnimation->mDuration);

    calculateBlendedBoneTransform(pBaseAnimation, &pBaseAnimation->mRootNode, pLayeredAnimation, &pLayeredAnimation->mRootNode, animTime1, animTime2, Maths::FMatrix4(1.0f), pFactor);
}

void Animator::calculateBlendedBoneTransform(Animation* pAnimationBase, AssimpNodeData* node,
                Animation* pAnimationLayer, AssimpNodeData* nodeLayered,
                float currentTimeBase, float currentTimeLayered,
                Maths::FMatrix4 parentTransform,
                float blendFactor)
{
    const std::string& nodeName = node->mName;

    Maths::FMatrix4 nodeTransform = node->mTransformation;
    Bone* pBone = pAnimationBase->findBone(nodeName);

    if (pBone)
    {
        pBone->update(currentTimeBase);
        nodeTransform = pBone->mLocalTransform;
    }

    Maths::FMatrix4 layeredNodeTransform = nodeLayered->mTransformation;
    pBone = pAnimationLayer->findBone(nodeName);
    if (pBone)
    {
        pBone->update(currentTimeLayered);
        layeredNodeTransform = pBone->mLocalTransform;
    }

    //Blend matrix
    const Maths::FQuaternion rot0 = Maths::FQuaternion(nodeTransform);
    const Maths::FQuaternion rot1 = Maths::FQuaternion(layeredNodeTransform);
    const Maths::FQuaternion finalRot = Maths::FQuaternion::slerp(rot0, rot1, blendFactor);
    Maths::FMatrix4 blendedMat = Maths::FQuaternion::toMatrix4(finalRot);
    blendedMat.data[3] = Maths::FVector4::lerp(nodeTransform.data[3], layeredNodeTransform.data[3], blendFactor);

    Maths::FMatrix4 globalTransformation = parentTransform * blendedMat;

    node->mWorldTransf = globalTransformation;
    nodeLayered->mWorldTransf = globalTransformation;

    if (&pAnimationBase->mRootNode != node && pAnimationBase->mRootNode.mChildren[0] != node && pAnimationBase->mRootNode.mChildren[0]->mChildren[0] != node)
    {
        Maths::FVector3 pos(node->mWorldTransf.data[3].x, node->mWorldTransf.data[3].y, node->mWorldTransf.data[3].z);
        Maths::FVector3 Parentpos(node->mParent->mWorldTransf.data[3].x, node->mParent->mWorldTransf.data[3].y, node->mParent->mWorldTransf.data[3].z);
        Application::mLineDrawer->drawLine({ pos }, { Parentpos }, { 0,1,0 });
    }

    const auto& boneInfoMap = pAnimationBase->mBoneInfoMap;
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        const int index = boneInfoMap.at(nodeName).mId;
        const Maths::FMatrix4& offset = boneInfoMap.at(nodeName).mOffset;

        mFinalBoneMatrices[index] = globalTransformation * offset;
    }

    for (size_t i = 0; i < node->mChildren.size(); ++i)
        calculateBlendedBoneTransform(pAnimationBase, node->mChildren[i], pAnimationLayer, nodeLayered->mChildren[i], currentTimeBase, currentTimeLayered, globalTransformation, blendFactor);
}