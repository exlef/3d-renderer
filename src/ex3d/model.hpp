#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <string>
#include <vector>

#include "mesh.hpp"
#include "transform.hpp"

namespace ex
{
    class Model
    {
    public:
        std::vector<Mesh> meshes;
        ex::Transform tr;

        Model() = delete;
        Model(const std::string& path);
        glm::mat4 get_model_matrix();

    private:
        void processNode(aiNode* node, const aiScene* scene);
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

    glm::mat4 Model::get_model_matrix()
    {
        glm::mat4 m_model = glm::mat4(1.0f);
        m_model = glm::translate(m_model, tr.pos());
        m_model = glm::rotate(m_model, glm::radians(tr.rot().x), glm::vec3(1.0f, 0.0f, 0.0f));
        m_model = glm::rotate(m_model, glm::radians(tr.rot().y), glm::vec3(0.0f, 1.0f, 0.0f));
        m_model = glm::rotate(m_model, glm::radians(tr.rot().z), glm::vec3(0.0f, 0.0f, 1.0f));
        m_model = glm::scale(m_model, tr.scale());
        tr.is_dirty = false;

        return m_model;
    }

} // namespace ex
