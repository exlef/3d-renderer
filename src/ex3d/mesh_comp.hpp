#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <iostream>
#include <string>
#include <vector>

#include "helper.hpp"
#include "mesh.hpp"

namespace ex
{
    class MeshComponent
    {
    private:
        
    public:
        std::vector<Mesh> meshes;
        MeshComponent(const std::string& path)
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

        MeshComponent(const MeshComponent&) = delete;
        MeshComponent& operator=(const MeshComponent&) = delete;

        void processNode(aiNode* node, const aiScene* scene)
        {
            // process all the node's meshes (if any)
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(Mesh(mesh));
            }
            // then do the same for each of its children recursively
            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode(node->mChildren[i], scene);
            }
        }
    };

} // namespace ex
