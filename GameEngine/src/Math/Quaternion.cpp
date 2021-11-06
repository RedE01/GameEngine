#include "Quaternion.h"
#include "Vector.h"

std::ostream& operator<<(std::ostream& out, const glm::quat& q) {
    return out <<  glm::eulerAngles(q);
}