#pragma once

namespace NK {

    class Texture2D {
    public:
        Texture2D(const unsigned char* data, const unsigned int width, const unsigned int height);
        ~Texture2D();

        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;
        Texture2D& operator=(Texture2D&& texture) noexcept;
        Texture2D(Texture2D&& texture) noexcept;

        void Bind(const unsigned int unit) const;

    private:
        unsigned int m_Id_ = 0;
        unsigned int m_Width_ = 0;
        unsigned int m_Height_ = 0;
    };

}