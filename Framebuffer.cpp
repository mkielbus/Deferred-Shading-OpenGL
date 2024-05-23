#include "Framebuffer.h"

Framebuffer::Framebuffer()
{
	for (int i = 0; i < MAX_ATTACHMENTS; i++)
	{
		_textures[i] = 0;
	}

	_framebufferID = 0;
	_depthBuffer = 0;
	_depthTexture = 0;

	_hasDepthBuffer = false;
}

void Framebuffer::Init(glm::vec2 resolution)
{
	this->_resolution = resolution;

	glGenFramebuffers(1, &_framebufferID);
}

void Framebuffer::Activate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _framebufferID);

	glViewport(0, 0, (GLsizei)_resolution.x, (GLsizei)_resolution.y);
}

void Framebuffer::Deactivate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool Framebuffer::SetAttachmentTexture(unsigned int attachmentID, GLenum format, GLenum type)
{
	if (attachmentID >= MAX_ATTACHMENTS)
	{
		return false;
	}

	if (_textures[attachmentID] != 0)
	{
		glDeleteTextures(1, _textures + attachmentID);
	}

	glGenTextures(1, _textures + attachmentID);

	glBindTexture(GL_TEXTURE_2D, _textures[attachmentID]);

	glTexImage2D(GL_TEXTURE_2D, 0, format, (GLsizei)_resolution.x, (GLsizei)_resolution.y, 0, format, type, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, _framebufferID);

	glFramebufferTexture(GL_FRAMEBUFFER, glColorAttachments[attachmentID], _textures[attachmentID], 0);

	return true;
}

bool Framebuffer::SetAttachmentDepth(GLenum format)
{
	glGenTextures(1, &_depthTexture);

	glBindTexture(GL_TEXTURE_2D, _depthTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, format, (GLsizei)_resolution.x, (GLsizei)_resolution.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, _framebufferID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _depthTexture, 0);

	_hasDepthBuffer = true;

	return true;
}

void Framebuffer::AddDepthBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _framebufferID);

	glGenRenderbuffers(1, &_depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (GLsizei)_resolution.x, (GLsizei)_resolution.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);

	_hasDepthBuffer = true;
}

void Framebuffer::Clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _framebufferID);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Framebuffer::SetDrawBuffers(std::vector<unsigned int> attachmentsID)
{
	std::vector<GLenum> drawBuffers;

	for (unsigned int id : attachmentsID)
	{
		if (_textures[id] == 0)
		{
			return false;
		}

		drawBuffers.push_back(glColorAttachments[id]);
	}

	glDrawBuffers(drawBuffers.size(), drawBuffers.data());

	return true;
}

GLuint Framebuffer::GetTexture(unsigned int attachmentID)
{
	if (attachmentID >= MAX_ATTACHMENTS)
	{
		return 0;
	}

	return _textures[attachmentID];
}

GLuint Framebuffer::GetDepthTexture()
{
	return _depthTexture;
}