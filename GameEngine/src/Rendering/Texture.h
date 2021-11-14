#pragma once
#include "TextureFormat.h"

namespace GameEngine {

    enum class TextureType {
        TEXTURE_2D
    };

    enum class TextureFilterMode {
        NEAREST, LINEAR
    };

    enum class TextureWrapMode {
        CLAMP_TO_EDGE, CLAMP_TO_BORDER, MIRRORED_REPEAT, REPEAT, MIRROR_CLAMP_TO_EDGE
    };

    class Texture {
    public:
        Texture(TextureType textureType);
        Texture();
        ~Texture();

        void textureImage2D(TextureFormat textureFormat, unsigned int width, unsigned int height, char* data);
        void textureImage2D(TextureFormat textureFormat, unsigned int width, unsigned int height, unsigned char* data);
        void textureImage2D(TextureFormat textureFormat, unsigned int width, unsigned int height, short* data);
        void textureImage2D(TextureFormat textureFormat, unsigned int width, unsigned int height, unsigned short* data);
        void textureImage2D(TextureFormat textureFormat, unsigned int width, unsigned int height, int* data);
        void textureImage2D(TextureFormat textureFormat, unsigned int width, unsigned int height, unsigned int* data);
        void textureImage2D(TextureFormat textureFormat, unsigned int width, unsigned int height, float* data);

        void bind();
        void unbind();

        void setFilterMode(TextureFilterMode filterMode);
        void setWrapModeS(TextureWrapMode wrapMode);
        void setWrapModeT(TextureWrapMode wrapMode);
        void setWrapModeR(TextureWrapMode wrapMode);

        unsigned int getTextureID() const { return m_textureID; }
        unsigned int getTextureTypeID() const { return m_textureTypeID; }
        TextureFormat getTextureFormat() const { return m_textureFormat; }

        unsigned int getWidth() const { return m_width; }
        unsigned int getHeight() const { return m_height; }

    private:
        void textureImage2dInternal(TextureFormat textureFormat, unsigned int width, unsigned int height, const void* data, int type);

    private:
        unsigned int m_textureID;
        unsigned int m_textureTypeID;
        TextureFormat m_textureFormat;
        unsigned int m_width;
        unsigned int m_height;
    };

}