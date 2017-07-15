#pragma once

#include <glm/glm.hpp>

// ±ÕÇø¼ä
template<typename T>
bool in(const T &val, const T &e1, const T &e2) {
	return (e1 <= val && val <= e2) ||
		(e2 <= val && val <= e1);
}

bool ptInTri(glm::dvec2 p, glm::dvec2 p1, glm::dvec2 p2, glm::dvec2 p3) {
	auto sign = [](glm::dvec2 p1, glm::dvec2 p2, glm::dvec2 p3) -> bool {
		return ((p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y)) < 0.0;
	};

	bool b1, b2, b3;
	b1 = sign(p, p1, p2);
	b2 = sign(p, p2, p3);
	b3 = sign(p, p3, p1);

	return (b1 == b2) && (b2 == b3);
}