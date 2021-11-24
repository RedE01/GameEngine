#include "Framebuffer.h"
#include "Texture.h"
#include "Renderbuffer.h"
#include <algorithm>
#include <GL/glew.h>

namespace GameEngine {

    unsigned int getOpenGLAttachmentPoint(FramebufferAttachmentType type);

    Framebuffer::Framebuffer() {
        glCreateFramebuffers(1, &m_framebufferID);
    }

    Framebuffer::~Framebuffer() {
        glDeleteFramebuffers(1, &m_framebufferID);
    }

    void Framebuffer::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
    }

    void Framebuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::setDrawBuffers(unsigned int* colorAttachments, unsigned int nColorAttachments) {
        bind();
        
        std::vector<unsigned int> glColorAttachments;
        for(unsigned int i = 0; i < nColorAttachments; ++i) {
            glColorAttachments.push_back(GL_COLOR_ATTACHMENT0 + colorAttachments[i]);
        }

        glDrawBuffers(nColorAttachments, glColorAttachments.data());
    }

    void Framebuffer::attachTexture(Texture* texture, FramebufferAttachmentType type) {
        bind();

        unsigned int attachmentPoint = getOpenGLAttachmentPoint(type);

        glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentPoint, texture->getTextureTypeID(), texture->getTextureID(), 0);
    }

    void Framebuffer::attachRenderbuffer(Renderbuffer* renderbuffer, FramebufferAttachmentType type) {
        bind();

        unsigned int attachmentPoint = getOpenGLAttachmentPoint(type);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentPoint, GL_RENDERBUFFER, renderbuffer->getID());
    }

    bool Framebuffer::isComplete() {
        bind();
        return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }

    unsigned int getOpenGLAttachmentPoint(FramebufferAttachmentType type) {
        switch(type) {
        case FramebufferAttachmentType::Depth: return GL_DEPTH_ATTACHMENT;
        case FramebufferAttachmentType::Stencil: return GL_STENCIL_ATTACHMENT;
        case FramebufferAttachmentType::DepthStencil: return GL_DEPTH_STENCIL_ATTACHMENT;
        default: return GL_COLOR_ATTACHMENT0 + static_cast<unsigned int>(type);
        }
    }

}