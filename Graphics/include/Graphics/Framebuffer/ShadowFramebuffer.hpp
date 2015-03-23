#pragma once

#include <GL/glew.h>

class ShadowFramebuffer 
{
public:
    ShadowFramebuffer() : m_fboId(0)
    {
        glGenFramebuffers(1, &m_fboId);
        //glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
                //glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGenTextures(1, &m_textureId);
    }

    ~ShadowFramebuffer()
    {
        glDeleteTextures(1, &m_textureId);
        glDeleteRenderbuffers(1, &m_renderBufferId);
        glDeleteFramebuffers(1, &m_fboId);
    }

    bool initialize(uint32_t width = 1024, uint32_t height = 1024)
    {
        glGenRenderbuffers(1, &m_renderBufferId);
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, width, height);
        // Attach the renderbuffer
        
        glBindTexture(GL_TEXTURE_2D, m_textureId);

        // Create empty texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        // Bilinear filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Color needs to be 0 outside of texture coordinates
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC,  GL_LEQUAL);

        // Attach the shadow texture to the depth attachment
        glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_renderBufferId);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_textureId, 0);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            fprintf(stderr, "Error on building shadow framebuffer\n");
            exit( EXIT_FAILURE );
        }

        // Fall back to default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
    GLuint m_renderBufferId;
    GLuint m_textureId;
};