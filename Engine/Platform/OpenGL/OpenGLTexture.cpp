#include "nkpch.h"
#include "OpenGLTexture.h"

namespace NK {

    namespace detail {
        static GLenum NutckrackerImageFormatToGLDataFormat(ImageFormat format)
        {
            switch (format)
            {
                case ImageFormat::RGB8:  return GL_RGB;
                case ImageFormat::RGBA8: return GL_RGBA;
            }

            NK_CORE_ASSERT(false, "Invalid Nutckracker Image Format");
            return 0;
        }

        static GLenum NutckrackerImageFormatToGLInternalFormat(ImageFormat format)
        {
            switch (format)
            {
                case ImageFormat::RGB8: return GL_RGB8;
                case ImageFormat::RGBA8: return GL_RGBA8;
            }

            NK_CORE_ASSERT(false, "Invalid Nutckracker Image Format");
            return 0;
        }
    }

    OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification)
        : m_Specification_(specification), m_Width_(m_Specification_.Width), m_Height_(m_Specification_.Height)
    {
        m_InternalFormat_ = detail::NutckrackerImageFormatToGLInternalFormat(m_Specification_.Format);
        m_DataFormat_ = detail::NutckrackerImageFormatToGLDataFormat(m_Specification_.Format);

        GLsizei mip_levels = 1;
        if (specification.GenerateMips)
        {
            mip_levels = static_cast<GLsizei>(std::log2(std::max(m_Width_, m_Height_))) + 1;
        }
        NK_CORE_TRACE("Texture({0}): mip_levels = {1}", m_RendererID_, mip_levels);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID_);
        glTextureStorage2D(m_RendererID_, mip_levels, m_InternalFormat_, m_Width_, m_Height_);

        glTextureParameteri(m_RendererID_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(m_RendererID_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_RendererID_, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID_, GL_TEXTURE_WRAP_T, GL_REPEAT);   
    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath) // TODO: Add stbi_image library into project
        : m_Filepath_(filepath)
    {
        int width, height, channels;
        //stbi_set_flip_verticaly_on_load(1);
        //stbi_uc* data = nullptr;
        {
            //data = stbi_load(path.c_str(), &width, &height, channels, 0);
        }

        if (0)//data)
        {
            m_IsLoaded_ = true;

            m_Width_ = width;
            m_Height_ = height;

            GLenum internalFormat = 0, dataFormat = 0;
            if (channels == 4)
            {
                internalFormat = GL_RGB8;
                dataFormat = GL_RGBA;
            }
            if (channels == 3)
            {
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
            }

            m_InternalFormat_ = internalFormat;
            m_DataFormat_ = dataFormat;

            NK_CORE_ASSERT(internalFormat & dataFormat, "Format not supported");

            glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID_);
			glTextureStorage2D(m_RendererID_, 1, internalFormat, m_Width_, m_Height_);

			glTextureParameteri(m_RendererID_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTextureParameteri(m_RendererID_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureParameteri(m_RendererID_, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID_, GL_TEXTURE_WRAP_T, GL_REPEAT);

            //glTextureSubImage2D(m_RendererID_, 0, 0, 0, m_Width_, m_Height_, m_DataFormat_, GL_UNSIGNED_BYTE, data);

            //stbi_image_free(data);
        }
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &m_RendererID_);
    }

    void OpenGLTexture2D::SetData(void* data, uint32_t size)
    {
        uint32_t bpp = m_DataFormat_ == GL_RGBA ? 4 : 3;
        NK_CORE_ASSERT(size == m_Width_ * m_Height_ * bpp, "Data must be entire texture!");
        glTextureSubImage2D(m_RendererID_, 0, 0, 0, m_Width_, m_Height_, m_DataFormat_, GL_UNSIGNED_BYTE, data);
        if (m_Specification_.GenerateMips)
        {
            //Mipmap levels is already caculated in constructor
            glGenerateTextureMipmap(m_RendererID_);
        }
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const
    {
        glBindTextureUnit(slot, m_RendererID_);
    }

}