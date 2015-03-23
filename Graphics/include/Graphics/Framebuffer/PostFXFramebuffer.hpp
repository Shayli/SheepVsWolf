#pragma once

const int FX_TEXTURE_COUNT = 4;

class PostFXFramebuffer {
public:
	PostFXFramebuffer()
	{
		// Create Fx Framebuffer Object
		glGenFramebuffers(1, &fxFbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fxFbo);
		fxDrawBuffers[0] = GL_COLOR_ATTACHMENT0;
		glDrawBuffers(1, fxDrawBuffers);

		glGenTextures(FX_TEXTURE_COUNT, fxTextures);
	}

	void initialize(uint32_t w, uint32_t h)
	{
		width = w; height = h;
		// Create Fx textures
		for (int i = 0; i < FX_TEXTURE_COUNT; ++i)
		{
			glBindTexture(GL_TEXTURE_2D, fxTextures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		// Attach first fx texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D, fxTextures[0], 0);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			fprintf(stderr, "Error on building framebuffer\n");
			exit( EXIT_FAILURE );
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	GLuint getChannel(int i)
	{
		return fxTextures[i];
	}

	void attachChannel(int i)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D, fxTextures[i], 0);
	}

	void bind(){
		glBindFramebuffer(GL_FRAMEBUFFER, fxFbo);
	}
	void unbind(){
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void clear()
	{
		 // Full screen viewport
        glViewport(0, 0, width, height);
        // Clear default framebuffer color buffer
        glClear(GL_COLOR_BUFFER_BIT);
	}

protected:
	GLuint fxTextures[FX_TEXTURE_COUNT];
	GLuint fxFbo;
	GLuint fxDrawBuffers[1];
	uint32_t width, height;
};