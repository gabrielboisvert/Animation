#pragma once
#include <VKRenderer.h>

namespace Renderer
{

	class StorageBuffer
	{
		public:
			VKRenderer& mRenderer;
			std::vector<VkBuffer> mDirectionalLightStorageBuffers;
			std::vector<VkDeviceMemory> mDirectionalLightStorageBuffersMemory;
			std::vector<void*> mDirectionalLightStorageBuffersMapped;

			std::vector<VkDescriptorSet> DescriptorSets;

			StorageBuffer(VKRenderer& pRenderer);
			~StorageBuffer();

			void init(VkShaderStageFlagBits pStage);
			void createStorageBuffers();
			void createDescriptor(VkShaderStageFlagBits pStage);
	};
}