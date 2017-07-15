#include "Tri.h"

Tri::Tri(Vert v1, Vert v2, Vert v3) {
	verts[0] = v1;
	verts[1] = v2;
	verts[2] = v3;
}

Tri::Tri(Vert verts_[3]) {
	verts[0] = verts_[0];
	verts[1] = verts_[1];
	verts[2] = verts_[2];
}
