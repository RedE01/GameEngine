#pragma once
#include "Component.h"
#include "../Math/Vector.h"
#include "../Math/Quaternion.h"
#include "../Math/Matrix.h"

namespace GameEngine {

    class TransformComponent : public Component {
    public:
        TransformComponent();

        Vector3 getPosition() const;
        Quaternion getOrientation() const;
        Vector3 getOrientationEuler() const;
        Vector3 getScale() const;

        void setPosition(Vector3 position);
        void setOrientation(Quaternion orientation);
        void setOrientation(Vector3 eulerAngles);
        void setScale(Vector3 scale);

        void move(Vector3 movement, bool localSpace = false);
        void moveX(float x, bool localSpace = false);
        void moveY(float y, bool localSpace = false);
        void moveZ(float z, bool localSpace = false);

        void rotate(Quaternion quaternion);
        void rotate(Vector3 rotation);
        void rotateX(float x);
        void rotateY(float y);
        void rotateZ(float z);

        void scale(Vector3 scale);
        void scaleX(float x);
        void scaleY(float y);
        void scaleZ(float z);

        Matrix4 getMatrix() const;

    private:
        Vector3 m_position;
        Quaternion m_orientation;
        Vector3 m_scale;
    };

}