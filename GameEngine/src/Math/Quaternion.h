#pragma once

#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <iostream>

std::ostream& operator<<(std::ostream& out, const glm::quat& v);

using Quaternion = glm::quat;