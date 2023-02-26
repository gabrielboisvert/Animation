#include "Animation.h"
#include "Application.h"
#include <assimp/postprocess.h>
#include <iostream>

using namespace Renderer;

Renderer::AssimpNodeData::~AssimpNodeData()
{
    for (AssimpNodeData* child : mChildren)
        delete child;
}

Animation::Animation(const std::string& pAnimationPath, Model** pModel) : mModel(pModel)
{
    if (pModel == nullptr)
        return;

    mImporter = new Assimp::Importer;
    mImporter->SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
    const aiScene* scene = mImporter->ReadFile(pAnimationPath, aiProcess_ConvertToLeftHanded | aiProcess_GlobalScale);
    
    if (!scene || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP:: " << mImporter->GetErrorString() << std::endl;
        delete mImporter;
        return;
    }

    mScene = scene;
}

Animation::~Animation() 
{
    delete mImporter;

}

void Animation::init()
{
    if (mIsInit)
        return;

    if (mModel != nullptr && *mModel != nullptr)
    {
        if (mScene->HasAnimations())
        {
            aiAnimation* animation = mScene->mAnimations[0];
            mDuration = animation->mDuration;
            mTicksPerSecond = animation->mTicksPerSecond;
            readHeirarchyData(mRootNode, mScene->mRootNode);
            readMissingBones(animation, **mModel);

            initMatriceSkelete(mRootNode);
            for (AssimpNodeData* child : mRootNode.mChildren[0]->mChildren[0]->mChildren)
                initLineSkelete(*child);
        }

        mIsInit = true;
    }
}

void Renderer::Animation::initMatriceSkelete(AssimpNodeData& pNode)
{
    for (AssimpNodeData* child : pNode.mChildren)
    {
        child->mWorldTransf = pNode.mWorldTransf * child->mTransformation;
        initMatriceSkelete(*child);
    }
}

void Renderer::Animation::initLineSkelete(AssimpNodeData& pNode)
{
    Maths::FVector3 pos(pNode.mWorldTransf.data[3].x, pNode.mWorldTransf.data[3].y, pNode.mWorldTransf.data[3].z);
    Maths::FVector3 Parentpos(pNode.mParent->mWorldTransf.data[3].x, pNode.mParent->mWorldTransf.data[3].y, pNode.mParent->mWorldTransf.data[3].z);
    Application::mLineDrawer2->drawLine(pos, Parentpos, { 0, 1, 0 });
    
    for (AssimpNodeData* child : pNode.mChildren)
        initLineSkelete(*child);
}

Bone* Animation::findBone(const std::string& pName)
{
    std::vector<Bone>::iterator iter = std::find_if(mBones.begin(), mBones.end(),
        [&](const Bone& Bone) {
            return Bone.mName == pName;
        }
    );

    if (iter == mBones.end())
        return nullptr;
    else
        return &(*iter);
}

void Animation::readMissingBones(const aiAnimation* pAnimation, Model& pModel)
{
    int size = pAnimation->mNumChannels;

    std::map<std::string, BoneInfo>& boneInfoMap = pModel.mBoneInfoMap;
    int& boneCount = pModel.mBoneCounter;


    for (int i = 0; i < size; i++)
    {
        aiNodeAnim* channel = pAnimation->mChannels[i];
        std::string boneName = channel->mNodeName.data;

        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            boneInfoMap[boneName].mId = boneCount;
            boneCount++;
        }
        mBones.push_back(Bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].mId, channel));
    }

    mBoneInfoMap = boneInfoMap;
}

void Animation::readHeirarchyData(AssimpNodeData& pDest, const aiNode* pSrc)
{
    if (pSrc == nullptr)
        return;

    pDest.mName = pSrc->mName.data;
    pDest.mTransformation = Model::convertMatrix(pSrc->mTransformation);
    pDest.mChildrenCount = pSrc->mNumChildren;

    pDest.mChildren.reserve(pSrc->mNumChildren);
    for (int i = 0; i < pSrc->mNumChildren; i++)
    {
        AssimpNodeData* newData = new AssimpNodeData;
        newData->mParent = &pDest;
        
        readHeirarchyData(*newData, pSrc->mChildren[i]);
        pDest.mChildren.push_back(newData);
    }

}