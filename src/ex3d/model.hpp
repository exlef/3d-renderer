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
    typedef std::function<void(glm::mat4)> model_matrix_callback_func;

    class Model
    {
    public:
        std::vector<Mesh> meshes;
        ex::Transform tr;

        Model() = delete;
        Model(const std::string& path);
        glm::mat4 get_model_matrix() const { return m_model; }
        glm::mat4 update_model_matrix();
        void set_model_matrix_changed_callback(model_matrix_callback_func callback);

        // void set_pos(glm::vec3 p);
        // void set_pos(float x, float y, float z);
        // void translate(glm::vec3 t);
        // void translateX(float x);
        // void translateY(float y);
        // void translateZ(float z);
        // void set_rot(glm::vec3 r);
        // void set_rot(float x, float y, float z);
        // void rotate(glm::vec3 v);
        // void rotateX(float r);
        // void rotateY(float y);
        // void rotateZ(float z);
        // void set_scale(glm::vec3 s);
        // void scale(glm::vec3 s);
        // void set_scale(float x, float y, float z);
        // void set_scale(float s);
        // void scale(float s);
        // void scaleX(float x);
        // void scaleY(float y);
        // void scaleZ(float z);

    private:
        void processNode(aiNode* node, const aiScene* scene);
        glm::mat4 m_model = glm::mat4(1.0f);
        model_matrix_callback_func m_model_matrix_changed_callback = nullptr;

        // glm::vec3 m_pos = glm::vec3(0);
        // glm::vec3 m_rot = glm::vec3(0);
        // glm::vec3 m_scale = glm::vec3(1);
        // bool m_is_dirty = true;
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
        if (tr.is_dirty)
        {
            m_model = glm::mat4(1.0f);
            m_model = glm::translate(m_model, tr.pos());
            m_model = glm::rotate(m_model, glm::radians(tr.rot().x), glm::vec3(1.0f, 0.0f, 0.0f));
            m_model = glm::rotate(m_model, glm::radians(tr.rot().y), glm::vec3(0.0f, 1.0f, 0.0f));
            m_model = glm::rotate(m_model, glm::radians(tr.rot().z), glm::vec3(0.0f, 0.0f, 1.0f));
            m_model = glm::scale(m_model, tr.scale());
            tr.is_dirty = false;
        }
        if(m_model_matrix_changed_callback)
        {
            m_model_matrix_changed_callback(m_model);
        }

        return m_model;
    }

    // void Model::set_pos(glm::vec3 p)
    // {
    //     m_pos = p;
    //     m_is_dirty = true;
    // }
    // void Model::set_pos(float x, float y, float z)
    // {
    //     m_pos = glm::vec3(x,y,z);
    //     m_is_dirty = true;
    // }
    // void Model::translate(glm::vec3 t)
    // {
    //     m_pos += t;
    //     m_is_dirty = true;
    // }
    // void Model::translateX(float x)
    // {
    //     m_pos.x += x;
    //     m_is_dirty = true;
    // }
    // void Model::translateY(float y)
    // {
    //     m_pos.y += y;
    //     m_is_dirty = true;
    // }
    // void Model::translateZ(float z)
    // {
    //     m_pos.z += z;
    //     m_is_dirty = true;
    // }
    // void Model::set_rot(glm::vec3 r)
    // {
    //     m_rot = r;
    //     m_is_dirty = true;
    // }
    // void Model::set_rot(float x, float y, float z)
    // {
    //     m_rot = glm::vec3(x,y,z);
    //     m_is_dirty = true;
    // }
    // void Model::rotate(glm::vec3 r)
    // {
    //     m_rot += r;
    //     m_is_dirty = true;
    // }
    // void Model::rotateX(float x)
    // {
    //     m_rot.x += x;
    //     m_is_dirty = true;
    // }
    // void Model::rotateY(float y)
    // {
    //     m_rot.y += y;
    //     m_is_dirty = true;
    // }
    // void Model::rotateZ(float z)
    // {
    //     m_rot.z += z;
    //     m_is_dirty = true;
    // }
    // void Model::set_scale(glm::vec3 s)
    // {
    //     m_scale = s;
    //     m_is_dirty = true;
    // }
    // void Model::set_scale(float x, float y, float z)
    // {
    //     m_scale = glm::vec3(x,y,z);
    //     m_is_dirty = true;
    // }
    // void Model::set_scale(float s)
    // {
    //     m_scale = glm::vec3(s, s, s);
    //     m_is_dirty = true;
    // }
    // void Model::scale(glm::vec3 s)
    // {
    //     m_scale += s;
    //     m_is_dirty = true;
    // }
    // void Model::scale(float s)
    // {
    //     m_scale += glm::vec3(s,s,s);
    //     m_is_dirty = true;
    // }
    // void Model::scaleX(float x)
    // {
    //     m_scale.x += x;
    //     m_is_dirty = true;
    // }
    // void Model::scaleY(float y)
    // {
    //     m_scale.y += y;
    //     m_is_dirty = true;
    // }
    // void Model::scaleZ(float z)
    // {
    //     m_scale.z += z;
    //     m_is_dirty = true;
    // }

} // namespace ex
