#pragma once

#include <glm/glm.hpp>

// ±ÕÇø¼ä
template<typename T>
bool in(const T &val, const T &e1, const T &e2) {
	return (e1 <= val && val <= e2) ||
		(e2 <= val && val <= e1);
}

extern bool ptInTri(glm::dvec2 p, glm::dvec2 p1, glm::dvec2 p2, glm::dvec2 p3);

template<typename T>
T interpolate(glm::dvec3 coe, const T &e1, const T &e2, const T &e3) {
	return coe.x * e1 + coe.y * e2 + coe.z * e3;
}