#include "FrameBuffer.h"
#include "core/application/Application.h"
#include <GL/glew.h>



FrameBuffer::FrameBuffer() {
	glGenFramebuffers(1, &this->frameBuffer);
}


FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1, &this->frameBuffer);
}

void FrameBuffer::bind(int32 width, int32 height) {
	if (width < 0 || height < 0) {
		int32 w, h;
		Application::getWindowSize(&w, &h);
		width = width < 0 ? w : width;
		height = height < 0 ? h : height;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);
	glViewport(0, 0, width, height);
}

void FrameBuffer::unbind() {
	int32 w, h;
	Application::getWindowSize(&w, &h);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, w, h);
}

void FrameBuffer::setDrawBuffers(uint32 n, uint32* buffers) {
	glDrawBuffers(n, buffers);
}

void FrameBuffer::createColourTextureAttachment(uint32 attachment, uint32 texture, uint32 target) {
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment, target, texture, 0);
}

void FrameBuffer::createDepthTextureAttachment(uint32 texture, uint32 target) {
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, target, texture, 0);
}

void FrameBuffer::createColourBufferAttachment(int32 width, int32 height, uint32 colourbuffer, uint32 attachment, uint32 format) {
	glBindRenderbuffer(GL_RENDERBUFFER, colourbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment, GL_RENDERBUFFER, colourbuffer);
}

void FrameBuffer::createDepthBufferAttachment(int32 width, int32 height, uint32 depthBuffer) {
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
}

void FrameBuffer::createColourBufferAttachmentMultisample(int32 width, int32 height, uint32 colourbuffer, uint32 attachment, uint32 format, uint32 samples) {
	glBindRenderbuffer(GL_RENDERBUFFER, colourbuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, format, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment, GL_RENDERBUFFER, colourbuffer);
}

void FrameBuffer::createDepthBufferAttachmentMultisample(int32 width, int32 height, uint32 depthBuffer, uint32 samples) {
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
}

bool FrameBuffer::checkStatus(bool printSuccess) {
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);
	int32 status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	bool valid = true;
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		valid = false;

		logInfo("Framebuffer failed validation with error \"%s\"",
			status == GL_FRAMEBUFFER_UNDEFINED ? "GL_FRAMEBUFFER_UNDEFINED" :
			status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT ? "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" :
			status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT ? "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" :
			status == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER ? "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" :
			status == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER ? "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" :
			status == GL_FRAMEBUFFER_UNSUPPORTED ? "GL_FRAMEBUFFER_UNSUPPORTED" :
			status == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE ? "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" :
			status == GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS ? "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS" :
			"UNKNOWN_ERROR"
		);

	} else if (printSuccess) {
		logInfo("Framebuffer successfully validated");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return valid;
}

uint32 FrameBuffer::genRenderBuffers() {
	uint32 renderBuffer; // Add render buffer to internal array for later cleanup? It was created here, so this class should be responsible for deleting it.
	glGenRenderbuffers(1, &renderBuffer);

	return renderBuffer;
}
