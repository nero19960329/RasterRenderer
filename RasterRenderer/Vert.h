#pragma once

#include <glm/glm.hpp>

class Vert {
public:
	glm::dvec4 coor;
	glm::dvec3 normal;
	glm::dvec2 texcoords;

public:
	Vert() {}
	Vert(glm::dvec4 coor_) : coor(coor_) {}
	Vert(glm::dvec3 coor_) { coor = glm::dvec4(coor_, 1.0); }
	Vert(glm::dvec4 coor_, glm::dvec3 normal_) : coor(coor_), normal(normal_) {}
	Vert(glm::dvec3 coor_, glm::dvec3 normal_) : normal(normal_) { coor = glm::dvec4(coor_, 1.0); }
	Vert(glm::dvec4 coor_, glm::dvec3 normal_, glm::dvec2 texcoords_) : coor(coor_), normal(normal_), texcoords(texcoords_) {}
	Vert(glm::dvec3 coor_, glm::dvec3 normal_, glm::dvec2 texcoords_) : normal(normal_), texcoords(texcoords_) { coor = glm::dvec4(coor_, 1.0); }
	~Vert() {}
};