#pragma once



/*      TEXTURE GENERATION EXAMPLE



		//Texture initialization
		unsigned char* data = new unsigned char[m_TextureWidth_ * m_TextureHeight_ * m_TextureChannels];
		
		TextureSpecification TexSpec;
		TexSpec.Width = m_TextureWidth_;
		TexSpec.Height = m_TextureHeight_;
		TexSpec.GenerateMips = true;
		TexSpec.data = data;
		
		generate_smile_texture(data, m_TextureWidth_, m_TextureHeight_);
		m_SmileTexture_.reset(Texture2D::Create(TexSpec));
		m_SmileTexture_->SetData(data, m_TextureWidth_ * m_TextureHeight_ * m_TextureChannels);
		//m_SmileTexture_->Bind(0);
		
		generate_quads_texture(data, m_TextureWidth_, m_TextureHeight_);
		m_QuadsTexture_.reset(Texture2D::Create(TexSpec));
		//m_QuadsTexture_->Bind(1);
		
		delete[] data;



*/

void generate_circle(unsigned char* data,
                         const unsigned int width,
                         const unsigned int height,
                         const unsigned int center_x,
                         const unsigned int center_y,
                         const unsigned int radius,
                         const unsigned char color_r,
                         const unsigned char color_g,
                         const unsigned char color_b)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            for (unsigned int y = 0; y < height; ++y)
            {
                if ((x - center_x) * (x - center_x) + (y - center_y) * (y - center_y) < radius * radius)
                {
                    data[3 * (x + width * y) + 0] = color_r;
                    data[3 * (x + width * y) + 1] = color_g;
                    data[3 * (x + width * y) + 2] = color_b;
                }
            }
        }
    }

    void generate_smile_texture(unsigned char* data,
                                const unsigned int width,
                                const unsigned int height)
    {
        // background
        for (unsigned int x = 0; x < width; ++x)
        {
            for (unsigned int y = 0; y < height; ++y)
            {
                data[3 * (x + width * y) + 0] = 200;
                data[3 * (x + width * y) + 1] = 191;
                data[3 * (x + width * y) + 2] = 231;
            }
        }

        // face
        generate_circle(data, width, height, width * 0.5, height * 0.5, width * 0.4, 255, 255, 0);

        // smile
        generate_circle(data, width, height, width * 0.5, height * 0.4, width * 0.2, 0, 0, 0);
        generate_circle(data, width, height, width * 0.5, height * 0.45, width * 0.2, 255, 255, 0);

        // eyes
        generate_circle(data, width, height, width * 0.35, height * 0.6, width * 0.07, 255, 0, 255);
        generate_circle(data, width, height, width * 0.65, height * 0.6, width * 0.07, 0, 0, 255);
    }

    void generate_quads_texture(unsigned char* data,
                                const unsigned int width,
                                const unsigned int height)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            for (unsigned int y = 0; y < height; ++y)
            {
                if ((x < width / 2 && y < height / 2) || x >= width / 2 && y >= height / 2)
                {
                    data[3 * (x + width * y) + 0] = 0;
                    data[3 * (x + width * y) + 1] = 0;
                    data[3 * (x + width * y) + 2] = 0;
                }
                else
                {
                    data[3 * (x + width * y) + 0] = 255;
                    data[3 * (x + width * y) + 1] = 255;
                    data[3 * (x + width * y) + 2] = 255;
                }
            }
        }
    }