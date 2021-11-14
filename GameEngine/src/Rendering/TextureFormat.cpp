#include "TextureFormat.h"
#include <GL/glew.h>
#include <cassert>

namespace GameEngine {

	namespace internal {

		OpenGLTextureFormats getOpenGLTextureFormats(TextureFormat textureFormat) {
			int internalFormat, format;
			switch(textureFormat) {
				case TextureFormat::R: internalFormat = GL_RED; format = GL_RED; break;
				case TextureFormat::RG: internalFormat = GL_RG; format = GL_RG; break;
				case TextureFormat::RGB: internalFormat = GL_RGB; format = GL_RGB; break;
				case TextureFormat::RGBA: internalFormat = GL_RGBA; format = GL_RGBA; break;
				case TextureFormat::SRGB: internalFormat = GL_SRGB; format = GL_RGB; break;
				case TextureFormat::SRGBA: internalFormat = GL_SRGB_ALPHA; format = GL_RGBA; break;
				case TextureFormat::R8: internalFormat = GL_R8; format = GL_RED; break;
				case TextureFormat::R16: internalFormat = GL_R16; format = GL_RED; break;
				case TextureFormat::RG8: internalFormat = GL_RG8; format = GL_RG; break;
				case TextureFormat::RG16: internalFormat = GL_RG16; format = GL_RG; break;
				case TextureFormat::RGB8: internalFormat = GL_RGB8; format = GL_RGB; break;
				case TextureFormat::RGB12: internalFormat = GL_RGB12; format = GL_RGB; break;
				case TextureFormat::RGBA8: internalFormat = GL_RGBA8; format = GL_RGBA; break;
				case TextureFormat::RGBA16: internalFormat = GL_RGBA16; format = GL_RGBA; break;
				case TextureFormat::SRGB8: internalFormat = GL_SRGB8; format = GL_RGB; break;
				case TextureFormat::SRGBA8: internalFormat = GL_SRGB8_ALPHA8; format = GL_RGBA; break;
				case TextureFormat::R16F: internalFormat = GL_R16F; format = GL_RED; break;
				case TextureFormat::RG16F: internalFormat = GL_RG16F; format = GL_RG; break;
				case TextureFormat::RGB16F: internalFormat = GL_RGB16F; format = GL_RGB; break;
				case TextureFormat::RGBA16F: internalFormat = GL_RGBA16F; format = GL_RGBA; break;
				case TextureFormat::R32F: internalFormat = GL_R32F; format = GL_RED; break;
				case TextureFormat::RG32F: internalFormat = GL_RG32F; format = GL_RG; break;
				case TextureFormat::RGB32F: internalFormat = GL_RGB32F; format = GL_RGB; break;
				case TextureFormat::RGBA32F: internalFormat = GL_RGBA32F; format = GL_RGBA; break;
				case TextureFormat::R8I: internalFormat = GL_R8I; format = GL_RED_INTEGER; break;
				case TextureFormat::R8UI: internalFormat = GL_R8UI; format = GL_RED_INTEGER; break;
				case TextureFormat::R16I: internalFormat = GL_R16I; format = GL_RED_INTEGER; break;
				case TextureFormat::R16UI: internalFormat = GL_R16UI; format = GL_RED_INTEGER; break;
				case TextureFormat::R32I: internalFormat = GL_R32I; format = GL_RED_INTEGER; break;
				case TextureFormat::R32UI: internalFormat = GL_R32UI; format = GL_RED_INTEGER; break;
				case TextureFormat::RG8I: internalFormat = GL_RG8I; format = GL_RG_INTEGER; break;
				case TextureFormat::RG8UI: internalFormat = GL_RG8UI; format = GL_RG_INTEGER; break;
				case TextureFormat::RG16I: internalFormat = GL_RG16I; format = GL_RG_INTEGER; break;
				case TextureFormat::RG16UI: internalFormat = GL_RG16UI; format = GL_RG_INTEGER; break;
				case TextureFormat::RG32I: internalFormat = GL_RG32I; format = GL_RG_INTEGER; break;
				case TextureFormat::RG32UI: internalFormat = GL_RG32UI; format = GL_RG_INTEGER; break;
				case TextureFormat::RGB8I: internalFormat = GL_RGB8I; format = GL_RGB_INTEGER; break;
				case TextureFormat::RGB8UI: internalFormat = GL_RGB8UI; format = GL_RGB_INTEGER; break;
				case TextureFormat::RGB16I: internalFormat = GL_RGB16I; format = GL_RGB_INTEGER; break;
				case TextureFormat::RGB16UI: internalFormat = GL_RGB16UI; format = GL_RGB_INTEGER; break;
				case TextureFormat::RGB32I: internalFormat = GL_RGB32I; format = GL_RGB_INTEGER; break;
				case TextureFormat::RGB32UI: internalFormat = GL_RGB32UI; format = GL_RGB_INTEGER; break;
				case TextureFormat::RGBA8I: internalFormat = GL_RGBA8I; format = GL_RGBA_INTEGER; break;
				case TextureFormat::RGBA8UI: internalFormat = GL_RGBA8UI; format = GL_RGBA_INTEGER; break;
				case TextureFormat::RGBA16I: internalFormat = GL_RGBA16I; format = GL_RGBA_INTEGER; break;
				case TextureFormat::RGBA16UI: internalFormat = GL_RGBA16UI; format = GL_RGBA_INTEGER; break;
				case TextureFormat::RGBA32I: internalFormat = GL_RGBA32I; format = GL_RGBA_INTEGER; break;
				case TextureFormat::RGBA32UI: internalFormat = GL_RGBA32UI; format = GL_RGBA_INTEGER; break;
				case TextureFormat::STENCIL_INDEX: internalFormat = GL_STENCIL_INDEX; format = GL_STENCIL_INDEX; break; 
				case TextureFormat::DEPTH_COMPONENT: internalFormat = GL_DEPTH_COMPONENT; format = GL_DEPTH_COMPONENT; break; 
				case TextureFormat::DEPTH_STENCIL: internalFormat = GL_DEPTH24_STENCIL8; format = GL_DEPTH_STENCIL; break; 
				// // case TextureFormat::SRGB8: internalFormat = GL_SRGB8; format = GL_RGB; break;
				// case TextureFormat::SRGBA8: internalFormat = GL_SRGB8_ALPHA8; format = GL_RGBA; break;
				default: assert(false);
			}

			return { internalFormat, format };
		}
	}

}