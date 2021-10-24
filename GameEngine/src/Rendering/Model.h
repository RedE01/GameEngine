#pragma once
#include <vector>
#include "Mesh.h"

namespace GameEngine {

    class Model {
    public:
        Model() = default;
        Model(const Model& model) = delete;

    public:
        std::vector<std::unique_ptr<Mesh>> meshes;
    };

}