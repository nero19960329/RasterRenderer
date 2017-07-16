#pragma once

#include "Vert.h"

class Tri {
public:
	Vert verts[3];
	int materialId;

public:
	Tri() {}
	Tri(Vert v1, Vert v2, Vert v3, int materialId_);
	Tri(Vert verts_[3], int materialId_);
	~Tri() {}
};