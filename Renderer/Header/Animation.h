#pragma once
#include "Model.h"
#include "Maths/FMatrix4.h"
#include <assimp/Importer.hpp>

namespace Renderer
{
    class Model;

    struct AssimpNodeData
    {
        Maths::FMatrix4 mTransformation = Maths::FMatrix4(1);
        Maths::FMatrix4 mWorldTransf = Maths::FMatrix4(1);
        std::string mName;
        int mChildrenCount;
        std::vector<AssimpNodeData*> mChildren;
        AssimpNodeData* mParent = nullptr;

        ~AssimpNodeData();
    };

    class Animation : public IResource
    {
        public:
            float mDuration = 0;
            int mTicksPerSecond = 0;
            std::vector<Bone> mBones;
            AssimpNodeData mRootNode;
            std::map<std::string, BoneInfo> mBoneInfoMap;
            Model** mModel = nullptr;
            Assimp::Importer* mImporter;
            const aiScene* mScene;
            bool mIsInit = false;

            Animation(const std::string& pAnimationPath, Model** pModel);
            void init();
            ~Animation();

            void initMatriceSkelete(AssimpNodeData& pNode);
            void initLineSkelete(AssimpNodeData& pNode);

            Bone* findBone(const std::string& pName);
            void readMissingBones(const aiAnimation* pAnimation, Model& pModel);
            void readHeirarchyData(AssimpNodeData& pDest, const aiNode* pSrc);
    };
}