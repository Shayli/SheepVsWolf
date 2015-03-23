#pragma once

class LightFramebuffer {
public:
	LightFramebuffer() {
		glGenFramebuffers(1, &m_fboId);
		glGenTextures(1, &m_textureId);
		m_drawbufferId = GL_COLOR_ATTACHMENT0;
	}

	void initialize(uint32_t width, uint32_t height) {
		glBindTexture(GL_TEXTURE_2D, m_textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
		glDrawBuffers(1, &m_drawbufferId);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D, m_textureId, 0);
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
		    std::cerr << "Error on building framebuffer" << std::endl;
		    return;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return;
	}	

	void bind(){
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
	}
	void unbind(){
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	GLuint getChannel()
	{
		return m_textureId;
	}

protected:
	GLuint m_fboId;
	GLuint m_textureId;
	GLuint m_drawbufferId;
};