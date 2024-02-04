#include "nkpch.h"

#include "VulkanRenderBackend.hpp"


#include <glad/glad.h>

namespace NK {

    // Zero initialization of Vukan context.
    VulkanRenderBackendContext* VulkanRenderBackend::s_vkRenderBackendContext_ = new VulkanRenderBackendContext();
    VulkanCurrentFrameContext*  VulkanRenderBackend::s_vkCurrentFrameContext_ = new VulkanCurrentFrameContext();
    VulkanRenderBackend*        VulkanRenderBackend::s_vkRenderBackend_ = nullptr;

	void VulkanRenderBackend::Init(Window* window)
	{   
        m_Window_ = window;
        m_Device_ = new VulkanDevice(m_Window_);
        RecreateSwapChain();
        CreateCommandBuffer();

        vkContext.device = m_Device_->device();        

        // Creating Global Descriptor Sets and Global Descriptor Layout
        InitGlobalDescroptorSets();

        //----- INITIALIZATION OF STATIC MEMBERS OF RENDERBACKEND CLASS AND GLOBAL STRUCT FOR VULKAN CONTEX -----------//
        // This step is using for initialization of global static pointers to Vulkan API instances for using in classes inmplemented with Vulkan API.

        // Initialization static vulkan context. It will be used for Buffer and Shader initialization.

        // Initialization of RenderBackend Vulkan Global context
        //s_vkRenderBackendContext_.NK_Device = m_Device_;
        NK_CORE_TRACE("{0}", m_GlobalDescriptorSets_.size());
        s_vkRenderBackendContext_->descriptorSets = m_GlobalDescriptorSets_;
        s_vkRenderBackendContext_->globalDescriptorSetLayout = m_GlobalSetLayout_->getDescriptorSetLayout();

        // Initialization of RenderBackend Vulkan Current Frame context
        s_vkCurrentFrameContext_->CurrentCommandBuffer = nullptr; // Because our frame isn't started yet.

        //Initializing pointer to unique instance of VulkanRenderBackend
        s_vkRenderBackend_ = this;
	}

	void VulkanRenderBackend::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
        m_ViewPort_ = {x, y, width, height};
    }

	void VulkanRenderBackend::SetClearColor(uint32_t r, uint32_t g, uint32_t b, uint32_t alpha)
	{
        m_ClearColor_ = {r, g, b, alpha};
	}

	void VulkanRenderBackend::Clear()
	{
        // TODO or empty
	}

	void VulkanRenderBackend::EnableDepthTest()
	{
        // TODO
	}

	void VulkanRenderBackend::DisableDepthTest()
	{
        // TODO
	}

	void VulkanRenderBackend::Draw(const Model* model)
	{
        // TODO
	}

    void VulkanRenderBackend::AddRenderSystem(RenderSystem* system)
    {
        system->Init();
        if (system->IsLightSystem())
        {
            m_LightRenderSystems_.push_back(dynamic_cast<LightRenderSystem*>(system));
        }
        else
        {
            m_ObjectRenderSystems_.push_back(dynamic_cast<ObjectRenderSystem*>(system));
        }

    }

    void VulkanRenderBackend::AddRenderSurface(const Window* renderSurface)
    {
        // TODO
    }

    void VulkanRenderBackend::BeginScene()
    {
        // empty
    }

    bool VulkanRenderBackend::BeginFrame()
    {   
        m_CurrCommandBuffer_ = VKBeginFrame();
        if (m_CurrCommandBuffer_ != nullptr)
        {
            m_IsFrameStarted_ = true;
            s_vkCurrentFrameContext_->CurrentCommandBuffer = m_CurrCommandBuffer_;
        }
        else
        {
            m_IsFrameStarted_ = false;
            s_vkCurrentFrameContext_->CurrentCommandBuffer = nullptr;
        }

        return m_IsFrameStarted_;
    }

    void VulkanRenderBackend::FrameProcessing(FrameInfo& frameInfo, GlobalUbo& ubo)
    {
        int frameIndex = GetFrameIndex();
        frameInfo.frameIndex = frameIndex;

        // Updating Light System
        std::for_each(m_LightRenderSystems_.begin(), m_LightRenderSystems_.end(), [&frameInfo, &ubo](LightRenderSystem* system){
            system->Update(frameInfo, ubo);
        });

        m_UboBuffers_[frameIndex]->writeToBuffer(&ubo);
        m_UboBuffers_[frameIndex]->flush();
    }

    void VulkanRenderBackend::BeginRendering()
    {
        BeginSwapChainRenderPass(m_CurrCommandBuffer_);
    }

    void VulkanRenderBackend::ProcessRenderingSystems(FrameInfo& frameInfo)
    {
        // Note: Order here matters
        // Rendering Game objects
        std::for_each(m_ObjectRenderSystems_.begin(), m_ObjectRenderSystems_.end(), [&frameInfo](const ObjectRenderSystem* renderSystem){
            renderSystem->Render(frameInfo);
        });

        // Rendering Lights
        std::for_each(m_LightRenderSystems_.begin(), m_LightRenderSystems_.end(), [&frameInfo](const LightRenderSystem* renderSystem){
            renderSystem->Render(frameInfo);
        });
    }

    void VulkanRenderBackend::EndRendering()
    {
        EndSwapChainRenderPass(m_CurrCommandBuffer_);
    }

    void VulkanRenderBackend::EndFrame()
    {
        VKEndFrame();

        // for unabling reusing m_Command Buffer after pipeline finished all work.
        //m_CommandBuffer_[m_CurrentImageIndex_] = nullptr; // it is wrong???
        m_IsFrameStarted_ = false;
        s_vkCurrentFrameContext_->CurrentCommandBuffer = nullptr;
    }

    void VulkanRenderBackend::EndScene()
    {
        // empty
    }


    
    //========================== PRIVATE RENDERBACKEND INIT FUNCTIONS IMPLEMENTATION ==========================//

    void VulkanRenderBackend::RecreateSwapChain()
    {
        VkExtent2D extent;

        extent.width = m_Window_->GetWidth();
        extent.height = m_Window_->GetHeight();

        // auto extent = VkExtent2D({static_cast<uint32_t>(m_Window_->GetWidth()), static_cast<uint32_t>(m_Window_->GetHeight())});
        while (extent.width == 0 || extent.height == 0)
        {
            std::cout << "extent.width == 0 || extent.height == 0\n";
            extent.width = m_Window_->GetWidth();
            extent.height = m_Window_->GetHeight();
            //extent = m_lveWindow_.getExtent();
            m_Window_->WaitEvents();//glfwWaitEvents();
        }
                    
        vkDeviceWaitIdle(vkContext.device);

        if(m_SwapChain_ == nullptr)
        {
            m_SwapChain_ = std::make_unique<VulkanSwapChain>(m_Device_, extent);
            //m_SwapChain_ = new VulkanSwapChain(m_Device_, extenr)
        }
        else
        {
            std::shared_ptr<VulkanSwapChain> oldSwapChain = std::move(m_SwapChain_);
            m_SwapChain_ = std::make_unique<VulkanSwapChain>(m_Device_, extent, oldSwapChain);

            if(!oldSwapChain->compareSwapFormat(*m_SwapChain_.get()))
            {
                throw std::runtime_error("Swap chain image (or depth) format has changed!");
            }
        }    
    }

    void VulkanRenderBackend::CreateCommandBuffer()
    {
        m_CommandBuffer_.resize(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_Device_->getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffer_.size());

        if (vkAllocateCommandBuffers(vkContext.device, &allocInfo, m_CommandBuffer_.data()) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    //================================================ END ====================================================//



    //========================== PRIVATE FUNCTION IMPLEMENTATION ==========================//
    
    //====== HELPER FUNCTIONS IMPLEMENTATION ======//

    VkCommandBuffer VulkanRenderBackend::GetCurrentCommandBuffer()
    {
        NK_CORE_ASSERT(m_IsFrameStarted_, "Cannot get command buffer when frame is not in progress!");//assert(m_isFrameStarted_ && "Cannot get command buffer when frame is not in progress!");
        return m_CommandBuffer_[m_currentFrameIndex_];
    }

    //=================== END =====================//

    VkCommandBuffer VulkanRenderBackend::VKBeginFrame()
    {
        NK_CORE_ASSERT(!m_IsFrameStarted_, "Can't call begin frame while already in progress!");//assert(!m_IsFrameStarted_ && "Can't call begin frame while already in progress!");
        auto result = m_SwapChain_->acquireNextImage(&m_CurrentImageIndex_);

        if(result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            RecreateSwapChain();
            return nullptr;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) 
        {
            throw std::runtime_error("failed to acquire swap chain image!");
        } 

        m_IsFrameStarted_ = true;

        auto commandBuffer = GetCurrentCommandBuffer();

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to beginFrame() recording command buffer!");
        }   
        return commandBuffer;
    }

    int VulkanRenderBackend::GetFrameIndex()
    {
        NK_CORE_ASSERT(m_IsFrameStarted_,"Cannot get current frame when frame is not in progress!");
        return m_currentFrameIndex_;

    }
    void VulkanRenderBackend::BeginSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        NK_CORE_ASSERT(m_IsFrameStarted_,"Can't call beginSwapChainRenderPass() while already in progress!");
        NK_CORE_ASSERT(commandBuffer == GetCurrentCommandBuffer(), "commandBuffer == getCurrentCommandBuffer(). Can't begin render pass on command buffer from a different frame!");
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_SwapChain_->getRenderPass();
        renderPassInfo.framebuffer = m_SwapChain_->getFrameBuffer(m_CurrentImageIndex_);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = m_SwapChain_->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(m_SwapChain_->getSwapChainExtent().width);
        viewport.height = static_cast<float>(m_SwapChain_->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, m_SwapChain_->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }
    void VulkanRenderBackend::EndSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        NK_CORE_ASSERT(m_IsFrameStarted_, "Can't call endSwapChainRenderPass() while already in progress!");
        NK_CORE_ASSERT(commandBuffer == GetCurrentCommandBuffer(), "Can't end render pass on command buffer from a different frame!");

        vkCmdEndRenderPass(commandBuffer);

    }
    void VulkanRenderBackend::VKEndFrame()
    {
        NK_CORE_ASSERT(m_IsFrameStarted_, "Can't call endFrame() while already in progress!");
        auto commandBuffer = GetCurrentCommandBuffer();
        
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to record command buffer!");
        }

        auto result = m_SwapChain_->submitCommandBuffers(&commandBuffer, &m_CurrentImageIndex_);
        if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR )//m_Window_->wasWindowResized())
        {
            //.resetWindowResizedFlag();
            RecreateSwapChain();
        }
        else if (result != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to present swap chain image!");
        }

        m_IsFrameStarted_ = false;
        m_currentFrameIndex_ = (m_currentFrameIndex_ + 1) % VulkanSwapChain::MAX_FRAMES_IN_FLIGHT;

    }

    void VulkanRenderBackend::InitGlobalDescroptorSets()
    {
        m_GlobalPool_ =  
        VulkanDescriptorPool::Builder()
        .setMaxSets(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT)
        .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VulkanSwapChain::MAX_FRAMES_IN_FLIGHT)
        .build();

        //std::vector<std::unique_ptr<VulkanBuffer>> uboBuffers(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);

        //m_UboBuffers_.reserve(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);

        for(size_t i = 0; i < m_UboBuffers_.size(); ++i)
        {
            m_UboBuffers_[i] = std::make_unique<VulkanBuffer>(
                m_Device_,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT// | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
            );
            m_UboBuffers_[i]->map();
        }


        // layout for uniform
        m_GlobalSetLayout_ = 
        VulkanDescriptorSetLayout::Builder()
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
            .build();

        //m_GlobalDescriptorSets_.reserve(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < m_GlobalDescriptorSets_.size(); ++i)
        {
            auto bufferInfo = m_UboBuffers_[i]->descriptorInfo();
            VulkanDescriptorWriter(*m_GlobalSetLayout_, *m_GlobalPool_)
                .writeBuffer(0, &bufferInfo)
                .build(m_GlobalDescriptorSets_[i]);
        } 

    }

}