#include "Matrix.h"

template <typename T, unsigned int N>
std::ostream& matrixOStream(std::ostream& out, const T* data) {
	out << "\n{";
	for(unsigned int row = 0; row < N; ++row) {
		out << "{";
		for(unsigned int col = 0; col < N; ++col) {
			out << data[col + N * row];
			if(col < N - 1) out << ", ";
		}
		out << "}";
		if(row < N - 1) out << ",\n ";
	}
	out << "}";
	return out;

}

std::ostream& operator<<(std::ostream& out, const glm::mat2& m) {
	return matrixOStream<glm::mediump_float, 2>(out, (glm::mediump_float*)&m);
}
std::ostream& operator<<(std::ostream& out, const glm::mat3& m) {
	return matrixOStream<glm::mediump_float, 3>(out, (glm::mediump_float*)&m);
}
std::ostream& operator<<(std::ostream& out, const glm::mat4& m) {
	return matrixOStream<glm::mediump_float, 4>(out, (glm::mediump_float*)&m);
}
