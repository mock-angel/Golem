/*
 * FrameBuffer.cpp
 *
 *  Created on: 10-May-2022
 *      Author: anantha
 */

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>

#include <SOIL/SOIL.h>

#include <Core/Graphics/FrameBuffer.h>
#include <Core/Graphics/Texture/Texture.h>
#include <Utils/Debug.h>

FrameBuffer::FrameBuffer()
{
	// TODO Auto-generated constructor stub
}

FrameBuffer::~FrameBuffer()
{
	del();
	// TODO Auto-generated destructor stub
}

void FrameBuffer::create(int width, int height)
{
	m_Width = width;
	m_Height = height;

	Texture frameTexture;
	frameTexture.create(width, height);
	m_TextureObject = frameTexture.get();

	// glGenTextures(1, &m_frameBuffer);
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	// Create depth buffer.
	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
	m_RBO = depthrenderbuffer;

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameTexture.get(), 0);
	// glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, frameTexture.get(), 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Debug::log("FRAME_BUFFER_ERROR: Couldn't create FrameBuffer.");
}

void FrameBuffer::resize(int width, int height)
{
	del();
	create(width, height);
}

void FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		// Process/log the error.
		if (err == GL_INVALID_FRAMEBUFFER_OPERATION)
			Debug::log("GL_INVALID_FRAMEBUFFER_OPERATION: BINDING_ERROR");
		if (err == GL_INVALID_VALUE)
			Debug::log("GL_INVALID_VALUE: BINDING_ERROR");
		if (err == GL_OUT_OF_MEMORY)
			Debug::log("GL_OUT_OF_MEMORY: BINDING_ERROR");
		if (err == GL_INVALID_OPERATION)
			Debug::log("GL_INVALID_OPERATION: BINDING_ERROR ", std::to_string(m_FBO));
		if (err == GL_CONTEXT_LOST)
			Debug::log("GL_CONTEXT_LOST: BINDING_ERROR");
	}
}

void FrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::del()
{
	glDeleteFramebuffers(1, &m_FBO);
	glDeleteTextures(1, &m_TextureObject);
}
