#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <string>
#include <vector>

#include "mesh.hpp"

namespace ex
{
    typedef std::function<void(glm::mat4)> model_matrix_callback_func;

    class Model
    {
    public:
        std::vector<Mesh> meshes;

        Model() = delete;
        Model(const std::string& path);
        glm::mat4 get_model_matrix() const { return m_model; }
        glm::mat4 update_model_matrix();
        void set_model_matrix_changed_callback(model_matrix_callback_func callback);

        void translate(glm::vec3 v);
        void translateX(float x);
        void translateY(float y);
        void translateZ(float z);
        void rotate(glm::vec3 v);
        void rotateX(float x);
        void rotateY(float y);
        void rotateZ(float z);

    private:
        void processNode(aiNode* node, const aiScene* scene);
        glm::mat4 m_model = glm::mat4(1.0f);
        model_matrix_callback_func m_model_matrix_changed_callback = nullptr;

        glm::vec3 m_pos = glm::vec3(0);
        glm::vec3 m_rot = glm::vec3(0);
        bool m_is_model_matrix_updated = false;
    };

    // imp ----------------------------------------------------------------------------------------
    Model::Model(const std::string& path)
    {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            return;
        }

        processNode(scene->mRootNode, scene);
    }

    void Model::processNode(aiNode* node, const aiScene* scene)
    {
        // process all the node's meshes (if any)
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(Mesh(mesh));
        }
        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    void Model::set_model_matrix_changed_callback(model_matrix_callback_func callback)
    {
        m_model_matrix_changed_callback = callback;
    }

    glm::mat4 Model::update_model_matrix()
    {
        if(m_is_model_matrix_updated)
        {
            m_model = glm::mat4(1.0f);
            m_model = glm::translate(m_model, m_pos);
            m_model = glm::rotate(m_model, glm::radians(m_rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
            m_model = glm::rotate(m_model, glm::radians(m_rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
            m_model = glm::rotate(m_model, glm::radians(m_rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
        }
        if(m_model_matrix_changed_callback)
        {
            m_model_matrix_changed_callback(m_model);
        }

        return m_model;
    }

    void Model::translate(glm::vec3 v)
    {
        m_pos += v;
        m_is_model_matrix_updated = true;
    }
    void Model::translateX(float x)
    {
        m_pos.x += x;
        m_is_model_matrix_updated = true;
    }
    void Model::translateY(float y)
    {
        m_pos.y += y;
        m_is_model_matrix_updated = true;
    }
    void Model::translateZ(float z)
    {
        m_pos.z += z;
        m_is_model_matrix_updated = true;
    }
    void Model::rotate(glm::vec3 v)
    {
        m_rot += v;
        m_is_model_matrix_updated = true;
    }
    void Model::rotateX(float x)
    {
        m_rot.x += x;
        m_is_model_matrix_updated = true;
    }
    void Model::rotateY(float y)
    {
        m_rot.y += y;
        m_is_model_matrix_updated = true;
    }
    void Model::rotateZ(float z)
    {
        m_rot.z += z;
        m_is_model_matrix_updated = true;
    }

} // namespace ex
