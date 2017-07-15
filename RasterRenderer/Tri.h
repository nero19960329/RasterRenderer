#pragma once

#include "Vert.h"

class Tri {
public:
	Vert verts[3];

public:
	Tri() {}
	Tri(Vert v1, Vert v2, Vert v3);
	Tri(Vert verts_[3]);
	~Tri() {}
};