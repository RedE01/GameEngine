#pragma once

namespace GameEngine {

    enum class TextureFormat {
        R, RG, RGB, RGBA, SRGB, SRGBA,
        R8, R16, RG8, RG16, RGB8, RGB12, RGBA8, RGBA16, SRGB8, SRGBA8, R16F, RG16F, RGB16F, RGBA16F, R32F, RG32F, RGB32F,
        RGBA32F, R8I, R8UI, R16I, R16UI, R32I, R32UI, RG8I, RG8UI, RG16I, RG16UI, RG32I, RG32UI, RGB8I, RGB8UI, RGB16I,
        RGB16UI, RGB32I, RGB32UI, RGBA8I, RGBA8UI, RGBA16I, RGBA16UI, RGBA32I, RGBA32UI, STENCIL_INDEX, DEPTH_COMPONENT, DEPTH_STENCIL
    };

	namespace internal {

		struct OpenGLTextureFormats {
			int internalFormat;
			int format;
		};

		OpenGLTextureFormats getOpenGLTextureFormats(TextureFormat textureFormat);
	}


}