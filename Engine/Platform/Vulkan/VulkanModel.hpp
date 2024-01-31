#pragma once

#include "Nutckracker/Renderer/Model.hpp"

#include "VulkanDevice.hpp"
#include "VulkanBuffer.hpp"

namespace NK {

    class VulkanModel : public Model
    {
    public:
        VulkanModel(const Builder& builder);
        virtual ~VulkanModel();

        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual void Draw() const override;

        virtual void BuildModel(const Builder& builder)  override;

        private:

        void createVertexBuffers(const std::vector<NK::Vertex>& vertices) ;
        void createIndexBuffers(const std::vector<uint32_t>& indices) ;

        VulkanDevice* m_Device_;

        std::unique_ptr<VulkanBuffer> m_VertexBuffer_;
        uint32_t m_VertexCount_;

        bool m_HasIndexBuffer_ = false;
        std::unique_ptr<VulkanBuffer> m_IndexBuffer_;
        uint32_t m_IndexCount_;
    };

}