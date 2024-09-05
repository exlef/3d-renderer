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
        glm::vec3 pos = glm::vec3(0);
        glm::vec3 rot = glm::vec3(0);

        Model() = delete;
        Model(const std::string& path);
        glm::mat4 get_model_matrix() const { return m_model; }
        glm::mat4 update_model_matrix();
        void set_model_matrix_changed_callback(model_matrix_callback_func callback);

    private:
        void processNode(aiNode* node, const aiScene* scene);
        glm::mat4 m_model;
        model_matrix_callback_func m_model_matrix_changed_callback = nullptr;
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

    glm::mat4 Model::update_model_matrix()
    {
        // glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        m_model = glm::mat4(1.0f);
        m_model = glm::translate(m_model, pos);
        m_model = glm::rotate(m_model, glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
        m_model = glm::rotate(m_model, glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
        m_model = glm::rotate(m_model, glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));

        if(m_model_matrix_changed_callback)
        {
            m_model_matrix_changed_callback(m_model);
        }

        return m_model;
    }

    void Model::set_model_matrix_changed_callback(model_matrix_callback_func callback)
    {
        m_model_matrix_changed_callback = callback;
        // callback(m_model);
    }

} // namespace ex
