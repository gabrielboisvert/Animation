#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>

using namespace Renderer;

Model::Model(VKRenderer& pRenderer, const std::string& pFilePath) : mRenderer(pRenderer)
{
    loadModel(pFilePath);
}

Model::~Model()
{
    for (unsigned int i = 0; i < mMeshes.size(); i++)
        delete mMeshes[i];
}

void Model::loadModel(const std::string& pPath)
{
    Assimp::Importer importer;
    importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
    const aiScene* scene = importer.ReadFile(pPath, aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_GlobalScale );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    processNode(scene->mRootNode, scene);
}

void Model::processNode(const aiNode* pNode, const aiScene* pScene)
{
    for (unsigned int i = 0; i < pNode->mNumMeshes; i++)
    {
        aiMesh* mesh = pScene->mMeshes[pNode->mMeshes[i]];
        processMesh(mesh, pScene);
    }

    for (unsigned int i = 0; i < pNode->mNumChildren; i++)
        processNode(pNode->mChildren[i], pScene);
}

void Model::setVertexBoneDataToDefault(Mesh::Vertex& pVertex)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        pVertex.mBoneIDs[i] = -1;
        pVertex.mWeights[i] = 0.0f;
    }
}

void Model::processMesh(const aiMesh* pMesh, const aiScene* pScene)
{
    std::vector<Renderer::Mesh::Vertex> vertices;
    std::vector<uint32_t> indices;

    aiMaterial* material = pScene->mMaterials[pMesh->mMaterialIndex];

    // Color
    aiColor4D diffuse;
    Maths::FVector4 color = Maths::FVector4::One;
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
        color = Maths::FVector4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);

    //vertices
    for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
    {
        Renderer::Mesh::Vertex vertex;
        setVertexBoneDataToDefault(vertex);

        Maths::FVector3 vector;
        vector.x = pMesh->mVertices[i].x;
        vector.y = pMesh->mVertices[i].y;
        vector.z = pMesh->mVertices[i].z;
        vertex.mPosition = vector;
        
        // normals
        if (pMesh->HasNormals())
        {
            vector.x = pMesh->mNormals[i].x;
            vector.y = pMesh->mNormals[i].y;
            vector.z = pMesh->mNormals[i].z;
            vertex.mNormal = vector;
        }
        
        // texture coordinates
        if (pMesh->mTextureCoords[0])
        {
            Maths::FVector2 vec;
            vec.x = pMesh->mTextureCoords[0][i].x;
            vec.y = pMesh->mTextureCoords[0][i].y;
            vertex.mTextureUV = vec;
        }

        if (pMesh->HasTangentsAndBitangents())
        {
            Maths::FVector3 vec;
            vec.x = pMesh->mTangents[i].x;
            vec.y = pMesh->mTangents[i].y;
            vec.z = pMesh->mTangents[i].z;
            vertex.mTangent = vec;

            vec.x = pMesh->mBitangents[i].x;
            vec.y = pMesh->mBitangents[i].y;
            vec.z = pMesh->mBitangents[i].z;
            vertex.mBiTangent = vec;
        }

        vertex.mColor = color;

        vertices.push_back(vertex);
    }

    //faces
    for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
    {
        aiFace face = pMesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    extractBoneWeightForVertices(vertices, pMesh, pScene);

    mMeshes.emplace_back(new Renderer::Mesh(mRenderer, vertices, indices));
}

void Model::setVertexBoneData(Mesh::Vertex& pVertex, int pBoneId, float pWeight)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
        if (pVertex.mBoneIDs[i] < 0)
        {
            pVertex.mWeights[i] = pWeight;
            pVertex.mBoneIDs[i] = pBoneId;
            break;
        }
}

void Model::extractBoneWeightForVertices(std::vector<Mesh::Vertex>& pVertices, const aiMesh* pMesh, const aiScene* pScene)
{
    std::map<std::string, BoneInfo>& boneInfoMap = mBoneInfoMap;
    int& boneCount = mBoneCounter;

    for (int boneIndex = 0; boneIndex < pMesh->mNumBones; ++boneIndex)
    {
        int boneID = -1;
        std::string boneName = pMesh->mBones[boneIndex]->mName.C_Str();
        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            BoneInfo newBoneInfo;
            newBoneInfo.mId = boneCount;
            newBoneInfo.mOffset = convertMatrix(pMesh->mBones[boneIndex]->mOffsetMatrix);
            boneInfoMap[boneName] = newBoneInfo;
            boneID = boneCount;
            boneCount++;
        }
        else
            boneID = boneInfoMap[boneName].mId;
        
        if (boneID == -1)
            return;

        aiVertexWeight* weights = pMesh->mBones[boneIndex]->mWeights;
        int numWeights = pMesh->mBones[boneIndex]->mNumWeights;

        for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
        {
            int vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;

            if (vertexId <= pVertices.size())
                setVertexBoneData(pVertices[vertexId], boneID, weight);
        }
    }
}

Maths::FMatrix4 Model::convertMatrix(const aiMatrix4x4& pFrom)
{
    Maths::FMatrix4 to;
    to.data[0][0] = pFrom.a1; to.data[1][0] = pFrom.a2; to.data[2][0] = pFrom.a3; to.data[3][0] = pFrom.a4;
    to.data[0][1] = pFrom.b1; to.data[1][1] = pFrom.b2; to.data[2][1] = pFrom.b3; to.data[3][1] = pFrom.b4;
    to.data[0][2] = pFrom.c1; to.data[1][2] = pFrom.c2; to.data[2][2] = pFrom.c3; to.data[3][2] = pFrom.c4;
    to.data[0][3] = pFrom.d1; to.data[1][3] = pFrom.d2; to.data[2][3] = pFrom.d3; to.data[3][3] = pFrom.d4;
    return to;
}

void Model::draw()
{
    for (unsigned int i = 0; i < mMeshes.size(); i++)
        mMeshes[i]->draw();
}