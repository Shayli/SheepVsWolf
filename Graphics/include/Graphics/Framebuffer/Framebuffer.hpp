#pragma once

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

enum FBOChannel {
	FBOChannel_Albedo,
	FBOChannel_Normal,
	FBOChannel_Position,
	FBOChannel_Depth,

	FBOChannel_Max
};

class Framebuffer {
	public:
		Framebuffer() {
			glGenFramebuffers(1, &m_fboId);
			glGenTextures(FBOChannel_Max, m_texturesId);
			m_drawbuffersId[0] = GL_COLOR_ATTACHMENT0;
			m_drawbuffersId[1] = GL_COLOR_ATTACHMENT1;
			m_drawbuffersId[2] = GL_COLOR_ATTACHMENT2;
		}

		~Framebuffer() {
			glDeleteTextures(FBOChannel_Max, m_texturesId);
			glDeleteFramebuffers(1, &m_fboId);
		}

		bool initialize(uint32_t width, uint32_t height)
		{
			glBindTexture(GL_TEXTURE_2D, m_texturesId[0]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glBindTexture(GL_TEXTURE_2D, m_texturesId[1]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glBindTexture(GL_TEXTURE_2D, m_texturesId[2]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			// Create depth texture
			glBindTexture(GL_TEXTURE_2D, m_texturesId[3]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			
			glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
			glDrawBuffers(3, m_drawbuffersId);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D, m_texturesId[0], 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1 , GL_TEXTURE_2D, m_texturesId[1], 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2 , GL_TEXTURE_2D, m_texturesId[2], 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texturesId[3], 0);

			int err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if(err != GL_FRAMEBUFFER_COMPLETE)
			{
			    std::cerr << "Error on building framebuffer: " << err << std::endl;
			    return false;
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			return true;
		}

		void bind(){
			glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
		}
		void unbind(){
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		GLuint getChannel(FBOChannel c)
		{
			return m_texturesId[c];
		}

	protected:
		GLuint m_fboId;
		GLuint m_texturesId[FBOChannel_Max];
		GLuint m_drawbuffersId[3];

	private:
		Framebuffer(const Framebuffer& o);
		Framebuffer& operator=(const Framebuffer& o);
};

#endif //FRAMEBUFFER_H