#pragma once

#include <vector>
#include <vulkan/vulkan.h>


#include "Nutckracker/Renderer/RendererAPI.hpp"
#include "Nutckracker/Renderer/RenderBackend.hpp"


#include "VKContext.hpp"
#include "VulkanBuffer.hpp"
#include "VulkanDevice.hpp"
#include "VulkanDescriptors.hpp"
#include "VulkanSwapChain.hpp"

namespace NK {

	class VulkanRenderBackend : public NK::RenderBackend
	{
	public:
		// Cherno
		virtual void Init(Window* window) override;
		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(uint32_t r, uint32_t g, uint32_t b, uint32_t alpha) override;
		virtual void Clear() override;
		virtual void EnableDepthTest() override;
		virtual void DisableDepthTest() override;
		virtual void Draw(const Model* model) override;

		// Me
		virtual void BeginScene() override;
		virtual bool BeginFrame() override;
		virtual void FrameProcessing(FrameInfo& frameInfo, GlobalUbo& ubo) override;
		virtual void BeginRendering() override;
		virtual void ProcessRenderingSystems(FrameInfo& frameInfo) override;
		virtual void EndRendering() override;
		virtual void EndFrame() override;
		virtual void EndScene() override;

		// Me
		virtual void AddRenderSystem(RenderSystem* system) override;
		virtual void AddRenderSurface(const Window* renderSurface) override;
		virtual float GetAspectRatio() override { return m_SwapChain_->extentAspectRatio(); }

	public:
		// Static Vulakan context. Will be inited in RenderBackend.
		// Using for Buffers, Textures, Swapchains, etc. wherer Vulkan handlders are needed. 
		static VulkanRenderBackendContext* s_vkRenderBackendContext_;
		static VulkanCurrentFrameContext* s_vkCurrentFrameContext_;
		static VulkanRenderBackend* s_vkRenderBackend_;

	public:
	// Getters and Setters
	VkRenderPass GetSwapChainRenderPass() { return m_SwapChain_->getRenderPass(); }
	VulkanDevice* GetDevice() { return m_Device_; }
	//std::unique_ptr<VkDescriptorSetLayout> GetGlobalDescriptorSetLayout() { return m_GlobalDescriptorSetLayout_; }

	private:
		Window* m_Window_;
		VulkanDevice* m_Device_;
		std::unique_ptr<VulkanSwapChain> m_SwapChain_;
		std::vector<VkCommandBuffer> m_CommandBuffer_;

		// Current Frame variables
		VkCommandBuffer m_CurrCommandBuffer_;
		// global descriptor sets that are will be used in render systems 
		// Maybe all RenderSystems should have their descriptos sets
		std::vector<VkDescriptorSet> m_DescriptorSets_;

		uint32_t m_CurrentImageIndex_ = 0;
        int m_currentFrameIndex_ = 0;
        bool m_IsFrameStarted_ = false;

	private:
		void RecreateSwapChain();
		void CreateCommandBuffer();

	private:
	// private functions for vulkan backend rendering for RenderBackend, xxx
		VkCommandBuffer VKBeginFrame();
		int GetFrameIndex();
		void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void VKEndFrame();

	private:
		// Helper functions for backend function of the VulkanRenderBackend
		VkCommandBuffer GetCurrentCommandBuffer();


	private:
		glm::vec4 m_ClearColor_ = {1.0f, 1.0f, 1.0f, 1.0f};
		glm::vec4 m_ViewPort_ = {0.0f, 0.0f, 0.0f, 0.0f};


	private:
	//----------------GLOBAL SET LAYOUT-----------------//

	std::vector<VkDescriptorSet> m_GlobalDescriptorSets_{VulkanSwapChain::MAX_FRAMES_IN_FLIGHT};
	//std::unique_ptr<VkDescriptorSetLayout> m_GlobalDescriptorSetLayout_ = nullptr;
	std::unique_ptr<VulkanDescriptorPool> m_GlobalPool_ = nullptr;
	std::unique_ptr<VulkanDescriptorSetLayout> m_GlobalSetLayout_ = nullptr;
	void InitGlobalDescroptorSets();

	//Global Ubo buffers
	std::vector<std::unique_ptr<VulkanBuffer>> m_UboBuffers_{VulkanSwapChain::MAX_FRAMES_IN_FLIGHT};


	private:
		// Renderer Systems
		//std::vector<LightRenderSystem*> m_LightRenderSystems_;
		//std::vector<ObjectRenderSystem*> m_ObjectRenderSystems_;
		// std::vector<SimpleRendererSystem> m_ObjSystems_;
		// std::vector<SimpleLightSystem>	 m_LightSystem_;
	};


}