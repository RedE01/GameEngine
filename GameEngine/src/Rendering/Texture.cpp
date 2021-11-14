#include "Texture.h"
#include <GL/glew.h>

#include <iostream>

namespace GameEngine {

    unsigned int getOpenGLTextureType(TextureType textureType);
    unsigned int getOpenGLFilterMode(TextureFilterMode filterMode);
    unsigned int getOpenGLWrapMode(TextureWrapMode wrapMode);

    Texture::Texture() : Texture(TextureType::TEXTURE_2D) {
    }

    Texture::Texture(TextureType textureType) {
        glGenTextures(1, &m_textureID);

        m_textureTypeID = getOpenGLTextureType(textureType);
        m_width = 0;
        m_height = 0;

        bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    Texture::~Texture() {
        glDeleteTextures(1, &m_textureID);    
    }

    void Texture::textureImage2D(TextureFormat textureFormat, unsigned int width, unsigned int height, char* data) {
        textureImage2dInternal(textureFormat, width, height, data, GL_BYTE);
    }

    void Texture::textureImage2D(TextureFormat textureFormat, unsigned int width, unsigned int height, unsigned char* data) {
        textureImage2dInternal(textureFormat, width, height, data, GL_UNSIGNED_BYTE);
    }

    void Texture::textureImage2D(TextureFormat textureFormat, unsigned int width, unsigned int height, short* data) {
        textureImage2dInternal(textureFormat, width, height, data, GL_SHORT);
    }

    void Texture::textureImage2D(TextureFormat textureFormat, unsigned int width, unsigned int height, unsigned short* data) {
        textureImage2dInternal(textureFormat, width, height, data, GL_UNSIGNED_SHORT);
    }

    void Texture::textureImage2D(TextureFormat textureFormat, unsigned int width, unsigned int height, int* data) {
        textureImage2dInternal(textureFormat, width, height, data, GL_INT);
    }

    void Texture::textureImage2D(TextureFormat textureFormat, unsigned int width, unsigned int height, unsigned int* data) {
        textureImage2dInternal(textureFormat, width, height, data, GL_UNSIGNED_INT);
    }

    void Texture::textureImage2D(TextureFormat textureFormat, unsigned int width, unsigned int height, float* data) {
        textureImage2dInternal(textureFormat, width, height, data, GL_FLOAT);
    }

    void Texture::bind() {
        glBindTexture(m_textureTypeID, m_textureID);
    }

    void Texture::unbind() {
        glBindTexture(m_textureTypeID, 0);
    }

    void Texture::setFilterMode(TextureFilterMode filterMode) {
        unsigned int filterModeID = getOpenGLFilterMode(filterMode);

        bind();
        glTexParameteri(m_textureTypeID, GL_TEXTURE_MIN_FILTER, filterModeID);
        glTexParameteri(m_textureTypeID, GL_TEXTURE_MAG_FILTER, filterModeID);
    }

    void Texture::setWrapModeS(TextureWrapMode wrapMode) {
        bind();
        glTexParameteri(m_textureTypeID, GL_TEXTURE_WRAP_S, getOpenGLWrapMode(wrapMode));
    }

    void Texture::setWrapModeT(TextureWrapMode wrapMode) {
        bind();
        glTexParameteri(m_textureTypeID, GL_TEXTURE_WRAP_T, getOpenGLWrapMode(wrapMode));
    }

    void Texture::setWrapModeR(TextureWrapMode wrapMode) {
        bind();
        glTexParameteri(m_textureTypeID, GL_TEXTURE_WRAP_R, getOpenGLWrapMode(wrapMode));
    }

    void Texture::textureImage2dInternal(TextureFormat textureFormat, unsigned int width, unsigned int height, const void* data, int type) {
        bind();
        internal::OpenGLTextureFormats openGLformats = internal::getOpenGLTextureFormats(textureFormat);

        if(textureFormat == TextureFormat::DEPTH_STENCIL) {
            type = GL_UNSIGNED_INT_24_8;
        }

        glTexImage2D(m_textureTypeID, 0, openGLformats.internalFormat, width, height, 0, openGLformats.format, type, data);
        m_textureFormat = textureFormat;
        m_width = width;
        m_height = height;
    }

    unsigned int getOpenGLTextureType(TextureType textureType) {
        switch(textureType) {
            case TextureType::TEXTURE_2D: return GL_TEXTURE_2D;
            default: std::cout << "ERROR: Texture type not supported" << std::endl; break;
        }
        return 0;
    }

    unsigned int getOpenGLFilterMode(TextureFilterMode filterMode) {
        switch(filterMode) {
            case TextureFilterMode::NEAREST: return GL_NEAREST;
            case TextureFilterMode::LINEAR: return GL_LINEAR;
            default: std::cout << "Error: Texture filter mode not supported" << std::endl;
        }
        return 0;
    }

    unsigned int getOpenGLWrapMode(TextureWrapMode wrapMode) {
        switch(wrapMode) {
            case TextureWrapMode::CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
            case TextureWrapMode::CLAMP_TO_BORDER: return GL_CLAMP_TO_BORDER;
            case TextureWrapMode::MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
            case TextureWrapMode::REPEAT: return GL_REPEAT;
            case TextureWrapMode::MIRROR_CLAMP_TO_EDGE: return GL_MIRROR_CLAMP_TO_EDGE;
            default: std::cout << "Error: Texture wrap mode not supported" << std::endl;
        }
        return 0;
    }

}