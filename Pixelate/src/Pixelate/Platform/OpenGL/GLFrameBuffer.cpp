#include "PXpch.h"
#include "GLFrameBuffer.h"

#include "glad/glad.h"

#include "GLCommon.h"
#include "Pixelate/Rendering/Renderer2D.h"

namespace Pixelate {



	GLFrameBuffer::GLFrameBuffer(const FrameBufferSpecs& specs) 
		:m_Specs(specs), m_RendererID(0)
	{
		Resize(m_Specs.Width, m_Specs.Height);
	}

	GLFrameBuffer::~GLFrameBuffer() {

		GLCall(glDeleteFramebuffers(1, &m_RendererID));
		GLCall(glDeleteTextures(1, &m_ColorAttachment));
		GLCall(glDeleteTextures(1, &m_DepthAttachment));

	}

	void GLFrameBuffer::Bind() const {

		if (m_Specs.Width == 0 || m_Specs.Height == 0 || m_Specs.Width > MaxFrameBufferSize ||
			m_Specs.Height > MaxFrameBufferSize) {
			PX_CORE_WARN("Framebuffer attempted to resize to %fx%f!\n", m_Specs.Width, m_Specs.Height);
			return;
		}
		GLCall(glViewport(0, 0, m_Specs.Width, m_Specs.Height));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	}

	void GLFrameBuffer::Unbind() const {
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void GLFrameBuffer::Resize(unsigned int width, unsigned int height) {
		m_Specs.Width = width;
		m_Specs.Height = height;
		if (width == 0 || height == 0 || width > MaxFrameBufferSize || height > MaxFrameBufferSize) {
			PX_CORE_WARN("Framebuffer attempted to resize to %fx%f!\n", width, height);
			return;
		}

		
		
		if (m_RendererID) {
			GLCall(glDeleteFramebuffers(1, &m_RendererID));
			GLCall(glDeleteTextures(1, &m_ColorAttachment));
			GLCall(glDeleteTextures(1, &m_DepthAttachment));
		}
		
		GLCall(glGenFramebuffers(1, &m_RendererID));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
		
		GLCall(glGenTextures(1, &m_ColorAttachment));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_ColorAttachment));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specs.Width, m_Specs.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		
		
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0));
		
		GLCall(glGenTextures(1, &m_DepthAttachment));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_DepthAttachment));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specs.Width, m_Specs.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0));
		
		PX_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "The framebuffer is incomplete!\n");
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));


	}

}