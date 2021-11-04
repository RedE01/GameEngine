#include "TransformComponent.h"
#include <gtc/matrix_transform.hpp>

namespace GameEngine {

    TransformComponent::TransformComponent() 
        : position(0.0, 0.0, 0.0), rotation(), scale(1.0, 1.0, 1.0) {

    }

    Matrix4 TransformComponent::getMatrix() {
        Matrix4 matrix = glm::mat4(1.0);

        matrix = glm::translate(matrix, position);
        matrix *= glm::toMat4(rotation);
        matrix = glm::scale(matrix, scale);

        return matrix;
    }

}