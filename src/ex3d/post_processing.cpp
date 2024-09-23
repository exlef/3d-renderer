#include "post_processing.hpp"


#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace ex 
{
    PostProcessing::PostProcessing(glm::vec2 framebuffer_dimensions)
    {

        m_shader.create_shader_program(m_vert_source_path, m_frag_source_path);
        m_shader.use();
        m_shader.setTexture("screenTexture", 0);

        float rectangleVertices[24]  = {

            // Coords    // texCoords
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 1.0f,

            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 0.0f, 1.0f
        };

        unsigned int rectVBO;
        glGenVertexArrays(1, &m_rectVAO);
        glGenBuffers(1, &rectVBO);
        glBindVertexArray(m_rectVAO);
        glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        // Create Frame Buffer Object
        glGenFramebuffers(1, &m_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

        // Create Framebuffer Texture
        // unsigned int framebufferTexture;
        glGenTextures(1, &m_framebufferTexture);
        glBindTexture(GL_TEXTURE_2D, m_framebufferTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, framebuffer_dimensions.x, framebuffer_dimensions.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_framebufferTexture, 0);

        // Create Render Buffer Object
        unsigned int RBO;
        glGenRenderbuffers(1, &RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebuffer_dimensions.x, framebuffer_dimensions.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

        // Error checking framebuffer
        auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer error: " << fboStatus << std::endl;


        // resets OpenGL state
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void PostProcessing::prepare()
    {
        // Bind the custom framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
        // Enable depth testing since it's disabled when drawing the framebuffer rectangle
        glEnable(GL_DEPTH_TEST);

    }

    void PostProcessing::draw()
    {
        // Bind the default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // Draw the framebuffer rectangle

        m_shader.use();
        glBindVertexArray(m_rectVAO);
        glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
        // glBindTexture(GL_TEXTURE_2D, m_framebufferTexture);
        glBindTexture(GL_TEXTURE_2D, 7); // debug. delete later
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}