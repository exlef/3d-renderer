#pragma once

#include "mesh.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "GLFW/glfw3.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <string>
#include <vector>

namespace ex
{
    class Model
    {
    public:
        Model() = delete;
        Model(const std::string& path);
        void Draw();

    private:
        // model data
        std::vector<Mesh> meshes;

        void loadModel(const std::string& path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    };

    // imp ----------------------------------------------------------------------------------------
    Model::Model(const std::string& path)
    {
        loadModel(path);
    }

    void Model::Draw()
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw();
    }

    void Model::loadModel(const std::string& path)
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

    // TODO: do it without recursion
    void Model::processNode(aiNode* node, const aiScene* scene)
    {
        // process all the node's meshes (if any)
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(Mesh(mesh /*, scene*/));
        }
        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

} // namespace ex

