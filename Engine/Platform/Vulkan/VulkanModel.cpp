#include "nkpch.h"

#include "VulkanModel.hpp"
#include "VulkanRenderBackend.hpp"
#include "VKContext.hpp"

namespace NK {

    VulkanModel::VulkanModel(const Builder& builder)
    {
        // Initialization Vulkan Device from VKContext, inited in VulkanRenderBackend::Init()
        m_Device_ =VulkanRenderBackend::s_vkRenderBackend_->GetDevice();

        // Building VulkanModel from Builder
        BuildModel(builder);
    }

    VulkanModel::~VulkanModel()
    {

    }

    void VulkanModel::Bind() const
    {
        VkBuffer buffers[] = {m_VertexBuffer_->getBuffer()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(VulkanRenderBackend::s_vkCurrentFrameContext_->CurrentCommandBuffer, 0, 1, buffers, offsets);

        if(m_HasIndexBuffer_)
        {
            vkCmdBindIndexBuffer(VulkanRenderBackend::s_vkCurrentFrameContext_->CurrentCommandBuffer, m_IndexBuffer_->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
        }

    }

    void VulkanModel::Unbind() const
    {

    }

    void VulkanModel::Draw() const
    {

    }

    void VulkanModel::BuildModel(const Builder& builder) 
    {
        createVertexBuffers(builder.m_Vertices_);
        createIndexBuffers(builder.m_Indices_);
    }


    void VulkanModel::createVertexBuffers(const std::vector<NK::Vertex>& vertices)  
    {
        m_VertexCount_ = static_cast<uint32_t>(vertices.size());
        NK_CORE_ASSERT(m_VertexCount_ >= 3, "Vertex count must be at least 3");//assert(m_VertexCount_ >= 3 && "Vertex count must be at least 3");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * m_VertexCount_; 

        uint32_t vertexSize = sizeof(vertices[0]);

        VulkanBuffer stagingBuffer{
            m_Device_,
            vertexSize,
            m_VertexCount_,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT            
        };

        stagingBuffer.map();
        stagingBuffer.writeToBuffer((void*)vertices.data());

        m_VertexBuffer_ = std::make_unique<VulkanBuffer>(
            m_Device_,
            vertexSize,
            m_VertexCount_,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        m_Device_->copyBuffer(stagingBuffer.getBuffer(), m_VertexBuffer_->getBuffer(), bufferSize);

    }

    void VulkanModel::createIndexBuffers(const std::vector<uint32_t>& indices) 
    {
        m_IndexCount_ = static_cast<uint32_t>(indices.size());
        m_HasIndexBuffer_ = m_IndexCount_ > 0; 
        
        if(!m_HasIndexBuffer_)
        {
            return;
        }
        
        VkDeviceSize bufferSize = sizeof(indices[0]) * m_IndexCount_;     
        uint32_t indexSize = sizeof(indices[0]);

        VulkanBuffer stagingBuffer{
            m_Device_,
            indexSize,
            m_IndexCount_,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        };

        stagingBuffer.map();
        stagingBuffer.writeToBuffer((void*)indices.data());

        m_IndexBuffer_ = std::make_unique<VulkanBuffer>(
            m_Device_,
            indexSize,
            m_IndexCount_,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        m_Device_->copyBuffer(stagingBuffer.getBuffer(), m_IndexBuffer_->getBuffer(), bufferSize);
    }


}