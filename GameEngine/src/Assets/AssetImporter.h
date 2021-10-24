#pragma once
#include <memory>

namespace GameEngine {

    class Model;
    class Texture;

    namespace assetImporter {

        std::unique_ptr<Model> importModel(const char* filepath);

    }

}