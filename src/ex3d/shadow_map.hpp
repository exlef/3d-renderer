#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "light.hpp"
#include "shadow_map_shader.hpp"

namespace ex
{
    class ShadowMap
    {
    private:
        const float dir_light_distance_factor = 20.0f; // we need to set a position for directioanl light for getting view matrix from

    public:
        const unsigned int SHADOW_WIDTH = 1024;
        const unsigned int SHADOW_HEIGHT = 1024;
        unsigned int depthMapFBO;

        ShadowMapShader shadow_map_shader;

        void setup_dir_light_shadow(const DirectionalLight* dir_light)
        {
            
            glGenFramebuffers(1, &depthMapFBO);

            unsigned int depthMap;
            glGenTextures(1, &depthMap);
            glBindTexture(GL_TEXTURE_2D, depthMap);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                         SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glm::vec3 light_pos = -dir_light->tr.get_forward() * dir_light_distance_factor;

            // Matrices needed for the light's perspective
            glm::mat4 orthgonalProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, 0.1f, 75.0f);
            glm::mat4 lightView = glm::lookAt(light_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 lightProjection = orthgonalProjection * lightView;

            shadow_map_shader.use();
            shadow_map_shader.setMat4("lightProjection", lightProjection);
        }

        ShadowMap() = default;
        ~ShadowMap() = default;
    };
} // namespace ex
