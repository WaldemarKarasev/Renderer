#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace NK {
    #if 0
    Texture2D* Texture2D::Create(const TextureSpecification& specification)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    NK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLTexture2D(specification);
		}

		NK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Texture2D* Texture2D::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    NK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLTexture2D(filepath);
		}

		NK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
    #else
        Texture2D::Texture2D(const unsigned char* data, const unsigned int width, const unsigned int height)
        : m_Width_(width), m_Height_(height)
        {
            glCreateTextures(GL_TEXTURE_2D, 1, &m_Id_);
            const GLsizei mip_levels = static_cast<GLsizei>(std::log2(std::max(m_Width_, m_Height_))) + 1;
            glTextureStorage2D(m_Id_, mip_levels, GL_RGB8, m_Width_, m_Height_);
            glTextureSubImage2D(m_Id_, 0, 0, 0, m_Width_, m_Height_, GL_RGB, GL_UNSIGNED_BYTE, data);
            glTextureParameteri(m_Id_, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(m_Id_, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTextureParameteri(m_Id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTextureParameteri(m_Id_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glGenerateTextureMipmap(m_Id_);
            NK_CORE_TRACE("strlen(data)============", strlen((const char*)data));
        }

        Texture2D::~Texture2D()
        {
            glDeleteTextures(1, &m_Id_);
        }

        Texture2D& Texture2D::operator=(Texture2D&& texture) noexcept
        {
            glDeleteTextures(1, &m_Id_);

            m_Id_ =     texture.m_Id_;
            m_Width_  = texture.m_Width_;
            m_Height_ = texture.m_Height_;

            texture.m_Id_ = 0;

        }

        Texture2D::Texture2D(Texture2D&& texture) noexcept
        {
            m_Id_ = texture.m_Id_;
            m_Width_ = texture.m_Width_;
            m_Height_ = texture.m_Height_;
            texture.m_Id_ = 0;
        }

        void Texture2D::Bind(const unsigned int unit) const
        {
            //NK_TRACE("Texture2D::Bind(): {0}, {1}", unit, m_Id_);
            glBindTextureUnit(unit, m_Id_);
        }
        #endif
}