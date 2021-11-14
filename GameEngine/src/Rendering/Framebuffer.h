#pragma once

namespace GameEngine {

    class Texture;
    class Renderbuffer;

    enum class FramebufferAttachmentType {
        Color0 = 0, Color1, Color2, Color3, Color4, Color5, Color6, Color7, Color8, Color9, Color10, Color11, Color12, Color13, Color14, Color15,
        Depth, Stencil, DepthStencil
    };

    class Framebuffer {
    public:
        Framebuffer();
        Framebuffer(const Framebuffer& other) = delete;
        Framebuffer(Framebuffer&& other) = delete;
        ~Framebuffer();

        void bind();
        void unbind();

        void attachTexture(Texture* texture, FramebufferAttachmentType type);
        void attachRenderbuffer(Renderbuffer* renderbuffer, FramebufferAttachmentType type);

        unsigned int getFramebufferID() const { return m_framebufferID; }
        bool isComplete();

        Framebuffer& operator=(const Framebuffer& other) = delete;
        Framebuffer& operator=(Framebuffer&& other) = delete;

    private:
        unsigned int m_framebufferID;
    };

}