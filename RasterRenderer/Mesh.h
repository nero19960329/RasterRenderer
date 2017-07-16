#pragma once

#include "tiny_obj_loader.h"
#include "Tri.h"

#include <vector>

class Mesh {
public:
	std::vector<Tri> tris;
	glm::dmat4 trans;

public:
	Mesh(const tinyobj::shape_t &shape, const std::vector<tinyobj::material_t> &materials, const tinyobj::attrib_t &attrib);
	~Mesh() {}
};