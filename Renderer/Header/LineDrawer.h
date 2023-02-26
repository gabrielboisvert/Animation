#pragma once
#include "Camera.h"
#include "UniformBuffer.h"
#include "Maths/FVector3.h"

namespace Renderer
{
	struct DebugVertex
	{
		DebugVertex() {};
		DebugVertex(float pX, float pY, float pZ, float pR, float pG, float pB)
		{
			mPosition[0] = pX;
			mPosition[1] = pY;
			mPosition[2] = pZ;

			mColor[0] = pR;
			mColor[1] = pG;
			mColor[2] = pB;
		}

		float mPosition[3];
		float mColor[3];

		static VkVertexInputBindingDescription getBindingDescription();
		static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
	};

	struct Uniform
	{
		alignas(16) Maths::FMatrix4 mModel = Maths::FMatrix4(1);
		alignas(16) Maths::FMatrix4 mVp;
	};

	class LineDrawer
	{
		public:
			VKRenderer& mRenderer;
			Camera& mCamera;
			bool init = false;

			Uniform mUniform;

			VkPipeline mGraphicsPipeline = nullptr;
			VkPipelineLayout mPipelineLayout = nullptr;

			VkDescriptorSetLayout mObjectSetLayout;

			std::vector<DebugVertex> mLineVertice;

			UniformBuffer mUniBuffer;

			VkBuffer mVertexBuffer = nullptr;
			VkDeviceMemory mVertexBufferMemory = nullptr;

			LineDrawer(Camera& pCamera, VKRenderer& pRenderer, const char* pVertex, const char* pFragment);
			~LineDrawer();

			static std::vector<char> readFile(const std::string& pFilename);
			VkShaderModule createShaderModule(const std::vector<char>& pCode);
			void createGraphicsPipeline(const char* pVertex, const char* pFragment);
			void createDescriptorSetLayout();

			void drawLine(const Maths::FVector3& pFrom, const Maths::FVector3& pTo, const Maths::FVector3& pColor);
			void flushLines();
			void setVP(const VkDescriptorSet& pDescriptor, void* pUniformBuffer, void* pData, size_t pSize);

			void createVertexBuffer();
			void reset();
	};
}