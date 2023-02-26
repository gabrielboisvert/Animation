#pragma once
#include "Maths/FVector3.h"
#include "Maths/FVector2.h"
#include "Maths/FVector4.h"
#include "Shader.h"

#define MAX_BONE_INFLUENCE 4

namespace Renderer
{
	class Mesh
	{
	public:
		struct Vertex
		{
			Maths::FVector3 mPosition;
			Maths::FVector3 mNormal;
			Maths::FVector2 mTextureUV;
			Maths::FVector4 mColor;
			Maths::FVector3 mTangent;
			Maths::FVector3 mBiTangent;

			int mBoneIDs[MAX_BONE_INFLUENCE];
			float mWeights[MAX_BONE_INFLUENCE];

			static VkVertexInputBindingDescription getBindingDescription();
			static std::array<VkVertexInputAttributeDescription, 8> getAttributeDescriptions();
		};

		std::vector<Vertex> mPosition;
		std::vector<uint32_t> mIndices;

		VkBuffer mVertexBuffer;
		VkDeviceMemory mVertexBufferMemory;

		VkBuffer mIndexBuffer;
		VkDeviceMemory mIndexBufferMemory;

		VKRenderer& mRenderer;

		Mesh(VKRenderer& pRenderer, const std::vector<Vertex>& pVertex, const std::vector<uint32_t>& pIndice);
		void init();
		void createVertexBuffer();
		void createIndexBuffer();
		void draw();
		~Mesh();
	};
}