#pragma once
#include <iostream>
#include <glm.hpp>

std::ostream& operator<<(std::ostream& out, const glm::mat2& m);
std::ostream& operator<<(std::ostream& out, const glm::mat3& m);
std::ostream& operator<<(std::ostream& out, const glm::mat4& m);

namespace GameEngine {

	using Matrix2 = glm::mat2;
	using Matrix3 = glm::mat3;
	using Matrix4 = glm::mat4;

}
