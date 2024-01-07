#if 1
#include "nkpch.h"
#include "Platform/OpenGL/OpenGLTexture.h"


namespace NK {

	namespace Utils {

		static GLenum HazelImageFormatToGLDataFormat(ImageFormat format)
		{
			switch (format)
			{
				case ImageFormat::RGB8:  return GL_RGB;
				case ImageFormat::RGBA8: return GL_RGBA;
			}

			return 0;
		}
		
		static GLenum HazelImageFormatToGLInternalFormat(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RGB8:  return GL_RGB8;
			case ImageFormat::RGBA8: return GL_RGBA8;
			}

			return 0;
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification)
		: m_Specification(specification), m_Width(m_Specification.Width), m_Height(m_Specification.Height)
	{

		m_InternalFormat = Utils::HazelImageFormatToGLInternalFormat(m_Specification.Format);
		m_DataFormat = Utils::HazelImageFormatToGLDataFormat(m_Specification.Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
                //glGenerateTextureMipmap(m_RendererID);

	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
        NK_TRACE(m_Width);
        NK_TRACE(m_Height);
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(m_RendererID);

	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{

		glBindTextureUnit(slot, m_RendererID);
	}
}
#endif