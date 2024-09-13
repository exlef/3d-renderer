#pragma once

#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>

#include "base_shader.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "texture.hpp"
#include "light.hpp"
#include "helper.hpp"
#include "open_gl_error_checking.hpp"
#include "model.hpp"

namespace ex
{
    class DefaultShader : public BaseShader
    {
    private:
        std::string m_vert_source_path = "src/ex3d/shaders/default.vert";
        std::string m_frag_source_path = "src/ex3d/shaders/default.frag";
        // https://stackoverflow.com/questions/7322147/what-is-the-range-of-opengl-texture-id
        uint32_t m_diffuse_texture_id = 0;
        uint32_t m_spec_texture_id = 0;
        // glm::vec3 m_sky_light;

    public:
        u_int32_t id() const { return m_id; }
        DefaultShader() = delete;
        DefaultShader(uint32_t diffuse_texture_id = 0, uint32_t spec_texture_id = 0)
        {
            create_shader_program(m_vert_source_path, m_frag_source_path);
            if (m_id == 0 || m_id == GL_INVALID_INDEX)
            {
                throw std::runtime_error("Failed to create shader program");
            }
            m_diffuse_texture_id = diffuse_texture_id;
            m_spec_texture_id = spec_texture_id;
            // m_sky_light = sky_light;
            setup();
        }

        void update(ex::Model& model, ex::Camera& cam, ex::Light& light_manager)
        {
            use();

            set_textures();

            set_model_matrix(model.tr.get_model_matrix());

            set_view_matrix(cam.get_view_matrix());
            set_view_pos(cam.tr.pos);

            set_projection_matrix(cam.get_projection_matrix());

            set_sky_light(light_manager.sky_light);

            set_directional_light(light_manager.dir_light);

            setInt("pointLightCount", light_manager.point_lights.size());

            for (size_t i = 0; i < light_manager.point_lights.size(); i++)
            {
                set_point_light(light_manager.point_lights[i], i);
            }
        }

        // void create_shader(const std::string& vert_file, const std::string& frag_file)
        // {
        //     m_id = glCreateProgram();

        //     uint32_t vert_id = create_shader(GL_VERTEX_SHADER, vert_file);
        //     uint32_t frag_id = create_shader(GL_FRAGMENT_SHADER, frag_file);

        //     glAttachShader(m_id, vert_id);
        //     glAttachShader(m_id, frag_id);

        //     glLinkProgram(m_id);

        //     { // error handling code
        //         int success;
        //         char infoLog[512];
        //         glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        //         if (!success)
        //         {
        //             glGetProgramInfoLog(m_id, 512, NULL, infoLog);
        //             std::cerr << "ERROR::SHADER::SHADER_PROGRAM::LINKING_FAILED\n"
        //                       << infoLog << std::endl;
        //             m_id = -1;
        //         }
        //     }

        //     glDeleteShader(vert_id);
        //     glDeleteShader(frag_id);
        // }

        void setup()
        {
            use();

            set_textures();

            setFloat("material.shininess", 32.0f);

            // reset the openGL state
            // glUseProgram(0);
        }

        void set_textures()
        {
            // set a default color for diffuse in case there is no textures provided
            if (m_diffuse_texture_id == 0)
            {
                ex::Texture m_default_texture = ex::Texture("src/ex3d/res/default2.png");
                glc(glActiveTexture(GL_TEXTURE0));
                glc(glBindTexture(GL_TEXTURE_2D, m_default_texture.id()));
                glc(glUniform1i(glGetUniformLocation(m_id, "material.diffuse"), 0));
            }
            else
            {
                glc(glActiveTexture(GL_TEXTURE0));
                glc(glBindTexture(GL_TEXTURE_2D, m_diffuse_texture_id));
                glc(glUniform1i(glGetUniformLocation(m_id, "material.diffuse"), 0));
            }

            if (m_spec_texture_id != 0)
            {
                glc(glActiveTexture(GL_TEXTURE1));
                glc(glBindTexture(GL_TEXTURE_2D, m_spec_texture_id));
                glc(glUniform1i(glGetUniformLocation(m_id, "material.specular"), 1));
            }
        }

        void set_model_matrix(const glm::mat4& mat)
        {
            setMat4("model", mat);
        }

        void set_view_matrix(const glm::mat4& mat)
        {
            setMat4("view", mat);
        }

        void set_projection_matrix(const glm::mat4& mat)
        {
            setMat4("projection", mat);
        }

        void set_color(const glm::vec3& color)
        {
            setVec3("material.color", color);
        }

        void set_view_pos(glm::vec3 cam_pos)
        {
            setVec3("viewPos", cam_pos);
        }

        void set_sky_light(SkyLight light)
        {
            setFloat("skyLight", light.strength);
        }

        void set_directional_light(const DirectionalLight& dir_light)
        {
            setVec3("dirLight.direction", dir_light.tr.get_forward());
            setVec3("dirLight.color", dir_light.color);
            setFloat("dirLight.strength", dir_light.strength);
        }

        void set_point_light(PointLight& light, int index)
        {
            /*
                vec3 position;
                vec3 color;
                float constant;
                float linear;
                float quadratic;
            */

            std::string index_str = std::to_string(index);
            setVec3("pointLights[" + index_str + "].position", light.tr.pos);
            setVec3("pointLights[" + index_str + "].color", light.color);
            setFloat("pointLights[" + index_str + "].strength", light.strength);
            setFloat("pointLights[" + index_str + "].constant", light.constant);
            setFloat("pointLights[" + index_str + "].linear", light.linear);
            setFloat("pointLights[" + index_str + "].quadratic", light.quadratic);
        }
    // private:
        // std::string load_shader_source(const std::string& relativePath)
        // {
        //     // Get the current working directory
        //     std::filesystem::path currentPath = std::filesystem::current_path();

        //     // Construct the full path
        //     std::filesystem::path fullPath = currentPath / relativePath;

        //     // Ensure the file exists
        //     if (!std::filesystem::exists(fullPath))
        //     {
        //         throw std::runtime_error("ERROR::SHADER::FILE_NOT_FOUND: " + fullPath.string());
        //     }

        //     std::string shaderCode;
        //     std::ifstream shaderFile;

        //     // Ensure ifstream objects can throw exceptions
        //     shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        //     try
        //     {
        //         // Open file
        //         shaderFile.open(fullPath);
        //         std::stringstream shaderStream;

        //         // Read file's buffer contents into stream
        //         shaderStream << shaderFile.rdbuf();

        //         // Close file handler
        //         shaderFile.close();

        //         // Convert stream into string
        //         shaderCode = shaderStream.str();
        //     }
        //     catch (std::ifstream::failure& e)
        //     {
        //         throw std::runtime_error("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " + fullPath.string());
        //     }

        //     return shaderCode;
        // }

        // unsigned int create_shader(GLenum shader_type, std::string relative_path_to_shader_source)
        // {
        //     unsigned int shader_id = glCreateShader(shader_type);
        //     std::string shader_source = load_shader_source(relative_path_to_shader_source);
        //     const char* source_ptr = shader_source.c_str();

        //     glShaderSource(shader_id, 1, &source_ptr, NULL);
        //     glCompileShader(shader_id);

        //     { // error handling code
        //         int success;
        //         char infoLog[512];
        //         glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

        //         if (!success)
        //         {
        //             glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
        //             switch (shader_type)
        //             {
        //             case GL_VERTEX_SHADER:
        //                 std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        //                 break;
        //             case GL_FRAGMENT_SHADER:
        //                 std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        //                 break;
        //             default:
        //                 break;
        //             }
        //         }
        //     }
        //     return shader_id;
        // }
    };
} // namespace ex

