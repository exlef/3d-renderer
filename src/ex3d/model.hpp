#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <iostream>
#include <string>
#include <vector>

#include "mesh.hpp"
#include "transform.hpp"
#include "helper.hpp"

namespace ex
{
    class Model
    {
    public:
        std::vector<Mesh> meshes;
        ex::Transform tr;

        Model() = delete;
        Model(const std::string& path);
        ~Model();
        Model(const Model& other);

    private:
        void processNode(aiNode* node, const aiScene* scene);
    };

    // imp ----------------------------------------------------------------------------------------
    Model::Model(const std::string& path)
    {
        std::cout << "model constructor called" << std::endl;

        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_FlipWindingOrder);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            return;
        }

        processNode(scene->mRootNode, scene);
    }
    
    // TODO: free the memory?
    Model::~Model()
    {
        std::cout << "model destructor called" << std::endl;
    }

    // Copy constructor
    Model::Model(const Model& other)
    {
        UNUSED(other);
        std::cout << RED << "error" << RESET << std::endl;
        throw std::runtime_error("Copy constructor for Model class called\n");
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

} // namespace ex
