#include "TransformComponent.h"
#include <gtc/matrix_transform.hpp>

namespace GameEngine {

    TransformComponent::TransformComponent() 
        : m_position(0.0, 0.0, 0.0), m_orientation(0.0, 0.0, 0.0, 1.0), m_scale(1.0, 1.0, 1.0) {

    }

    Vector3 TransformComponent::getPosition() const {
        return m_position;
    }

    Quaternion TransformComponent::getOrientation() const {
        return m_orientation;
    }

    Vector3 TransformComponent::getOrientationEuler() const {
        return glm::eulerAngles(m_orientation);
    }

    Vector3 TransformComponent::getScale() const {
        return m_scale;
    }

    void TransformComponent::setPosition(Vector3 position) {
        m_position = position;
    }

    void TransformComponent::setOrientation(Quaternion orientation) {
        m_orientation = orientation;
    }

    void TransformComponent::setOrientation(Vector3 eulerAngles) {
        m_orientation = Quaternion(eulerAngles);
    }

    void TransformComponent::setScale(Vector3 scale) {
        m_scale = scale;
    }

    void TransformComponent::move(Vector3 movement) {
        m_position += movement;
    }

    void TransformComponent::moveX(float x) {
        m_position.x += x;
    }

    void TransformComponent::moveY(float y) {
        m_position.y += y;
    }

    void TransformComponent::moveZ(float z) {
        m_position.z += z;
    }

    void TransformComponent::rotate(Quaternion quaternion) {
        m_orientation *= quaternion;
    }

    void TransformComponent::rotate(Vector3 rotation) {
        m_orientation *= Quaternion(rotation);
    }

    void TransformComponent::rotateX(float x) {
        rotate(Vector3(x, 0.0, 0.0));
    }

    void TransformComponent::rotateY(float y) {
        rotate(Vector3(0.0, y, 0.0));
    }

    void TransformComponent::rotateZ(float z) {
        rotate(Vector3(0.0, 0.0, z));
    }

    void TransformComponent::scale(Vector3 scale) {
        m_scale *= scale;
    }

    Matrix4 TransformComponent::getMatrix() const {
        Matrix4 matrix = glm::mat4(1.0);

        matrix = glm::translate(matrix, m_position);
        matrix *= glm::toMat4(m_orientation);
        matrix = glm::scale(matrix, m_scale);

        return matrix;
    }

}