#pragma once

#include <assimp/scene.h>

#include <string>
#include <vector>

#include "mesh.hpp"

namespace ex
{
    class MeshComponent
    {
    public:
        std::vector<Mesh> meshes;
        MeshComponent(const std::string& path);

        MeshComponent(const MeshComponent&) = delete;
        MeshComponent& operator=(const MeshComponent&) = delete;

        void processNode(aiNode* node, const aiScene* scene);
    };

} // namespace ex
