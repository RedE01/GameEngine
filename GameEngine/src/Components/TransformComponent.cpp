#include "TransformComponent.h"
#include <gtc/matrix_transform.hpp>

namespace GameEngine {

    TransformComponent::TransformComponent() 
        : m_position(0.0, 0.0, 0.0), m_orientation(1.0, 0.0, 0.0, 0.0), m_scale(1.0, 1.0, 1.0) {

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

    Quaternion qmul(Quaternion q1, Quaternion q2) {
        float w1 = q1.w;
        float x1 = q1.x;
        float y1 = q1.y;
        float z1 = q1.z;

        float w2 = q2.w;
        float x2 = q2.x;
        float y2 = q2.y;
        float z2 = q2.z;

        float rw = w1*w2 - x1*x2 - y1*y2 - z1*z2;
        float rx = w1*x2 + x1*w2 + y1*z2 - z1*y2;
        float ry = w1*y2 - x1*z2 + y1*w2 + z1*x2;
        float rz = w1*z2 + x1*y2 - y1*x2 + z1*w2;

        return Quaternion(rw, rx, ry, rz);
    }

    void TransformComponent::move(Vector3 movement, bool localSpace) {
        if(localSpace) m_position += m_orientation * movement;
        else m_position += movement;
    }

    void TransformComponent::moveX(float x, bool localSpace) {
        if(localSpace) m_position += m_orientation * Vector3(x, 0.0, 0.0);
        else m_position.x += x;
    }

    void TransformComponent::moveY(float y, bool localSpace) {
        if(localSpace) m_position += m_orientation * Vector3(0.0, y, 0.0);
        else m_position.y += y;
    }

    void TransformComponent::moveZ(float z, bool localSpace) {
        if(localSpace) m_position += m_orientation * Vector3(0.0, 0.0, z);
        else m_position.z += z;
    }

    void TransformComponent::rotate(Quaternion quaternion) {
        m_orientation *= quaternion;
    }

    void TransformComponent::rotate(Vector3 rotation) {
        rotate(Quaternion(rotation));
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

    void TransformComponent::scaleX(float x) {
        m_scale.x *= x;
    }

    void TransformComponent::scaleY(float y) {
        m_scale.y *= y;
    }

    void TransformComponent::scaleZ(float z) {
        m_scale.z *= z;
    }

    Matrix4 TransformComponent::getMatrix() const {
        Matrix4 matrix = glm::mat4(1.0);

        matrix = glm::translate(matrix, m_position);
        matrix *= glm::toMat4(m_orientation);
        matrix = glm::scale(matrix, m_scale);

        return matrix;
    }
    
    Vector3 TransformComponent::getForwardVector() const {
        return m_orientation * Vector3(0.0, 0.0, -1.0);
    }

}
