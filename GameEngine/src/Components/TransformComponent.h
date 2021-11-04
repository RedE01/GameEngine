#pragma once
#include "Component.h"
#include "../Math/Vector.h"
#include "../Math/Quaternion.h"
#include "../Math/Matrix.h"

namespace GameEngine {

    class TransformComponent : public Component {
    public:
        TransformComponent();

        Matrix4 getMatrix();

    public:
        Vector3 position;
        Quaternion rotation;
        Vector3 scale;
    };

}