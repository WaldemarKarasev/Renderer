#pragma once 

#include "Nutckracker/Renderer/Texture.h"
#include <glad/glad.h>
#if 1

namespace NK {
    
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(const TextureSpecification& specification);
        OpenGLTexture2D(const std::string& filepath);
        virtual ~OpenGLTexture2D();

        virtual const TextureSpecification& GetSpecification() const override { return m_Specification_; }
    
    	virtual uint32_t GetWidth() const override { return m_Width_;  }
		virtual uint32_t GetHeight() const override { return m_Height_; }
		virtual uint32_t GetRendererID() const override { return m_RendererID_; }

		virtual const std::string& GetPath() const override { return m_Filepath_; }
		
		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool IsLoaded() const override { return m_IsLoaded_; }

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID_ == other.GetRendererID();
		}

    
    private:
    //public:
        TextureSpecification m_Specification_;

        std::string m_Filepath_;
        bool m_IsLoaded_ = false;
        uint32_t m_Width_, m_Height_;
        uint32_t m_RendererID_;
        GLenum m_InternalFormat_, m_DataFormat_;
    };

}
#endif