#ifndef GKOM2024L_FRAMEBUFFERF_H
#define GKOM2024L_FRAMEBUFFERF_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>

class Framebuffer
{
public:
	Framebuffer();

	void Init(glm::vec2 Resolution);

	void Activate();

	void Deactivate();

	bool SetAttachmentTexture(unsigned int attachmentID, GLenum format, GLenum type);

	bool SetAttachmentDepth(GLenum format);

	void AddDepthBuffer();

	void Clear();

	bool SetDrawBuffers(std::vector<unsigned int> attachmentsID);

	GLuint GetTexture(unsigned int attachmentID);

	GLuint GetDepthTexture();

	static const unsigned int MAX_ATTACHMENTS = 16;

	const GLenum glColorAttachments[MAX_ATTACHMENTS] =
		{
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2,
			GL_COLOR_ATTACHMENT3,
			GL_COLOR_ATTACHMENT4,
			GL_COLOR_ATTACHMENT5,
			GL_COLOR_ATTACHMENT6,
			GL_COLOR_ATTACHMENT7,
			GL_COLOR_ATTACHMENT8,
			GL_COLOR_ATTACHMENT9,
			GL_COLOR_ATTACHMENT10,
			GL_COLOR_ATTACHMENT11,
			GL_COLOR_ATTACHMENT12,
			GL_COLOR_ATTACHMENT13,
			GL_COLOR_ATTACHMENT14,
			GL_COLOR_ATTACHMENT15};

private:
	GLuint _framebufferID;

	glm::vec2 _resolution;

	GLuint _textures[MAX_ATTACHMENTS];

	GLuint _depthTexture;

	GLuint _depthBuffer;

	bool _hasDepthBuffer;
};

#endif