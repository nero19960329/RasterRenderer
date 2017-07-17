#include "Clipper.h"

std::tuple<bool, std::vector<Tri>> Clipper::clipTri(const Tri &tri) {
	std::vector<Vert> verts(3);
	for (int i = 0; i < 3; ++i) {
		verts[i] = tri.verts[i];
		verts[i].coor = camera.projMat() * camera.viewMat() * verts[i].coor;
	}

	bool isChanged = false;
	for (int i = 0; i < 7; ++i) {
		std::vector<Vert> tmpVerts;

		for (auto it = verts.begin(); it != verts.end(); ++it) {
			auto nextIt = (std::next(it) == verts.end()) ? verts.begin() : std::next(it);
			auto m1 = mark(it->coor), m2 = mark(nextIt->coor);
			
			if (m1[i] && !m2[i]) {			// 外-内
				tmpVerts.push_back(intersect(i, *it, *nextIt));
				tmpVerts.push_back(*nextIt);
				isChanged = true;
			} else if (!m1[i] && !m2[i]) {	// 内-内
				tmpVerts.push_back(*nextIt);
			} else if (!m1[i] && m2[i]) {	// 内-外
				tmpVerts.push_back(intersect(i, *it, *nextIt));
				isChanged = true;
			} else {
				isChanged = true;
			}
		}

		verts.clear();
		verts = tmpVerts;
	}

	if (!isChanged)
		return std::make_tuple(false, std::vector<Tri>());
	else if (verts.size() == 0)
		return std::make_tuple(true, std::vector<Tri>());

	glm::dmat4 invViewProjMat = glm::inverse(camera.viewMat()) * glm::inverse(camera.projMat());
	for (auto &vert : verts)
		vert.coor = invViewProjMat * vert.coor;

	std::vector<Tri> tris;
	for (auto i = 1; i < verts.size() - 1; ++i)
		tris.push_back(Tri(verts[0], verts[i], verts[i + 1], tri.materialId));

	return std::make_tuple(true, tris);
}

std::array<bool, 7> Clipper::mark(glm::dvec4 clip) {
	glm::dvec3 p(clip);
	p /= clip.w;

	std::array<bool, 7> res = { false, false, false, false, false, false };
	if (clip.w < camera.zNear()) res[0] = true;
	if (p.x < -1.0) res[1] = true;
	if (p.x > 1.0) res[2] = true;
	if (p.y < -1.0) res[3] = true;
	if (p.y > 1.0) res[4] = true;
	if (p.z < -1.0) res[5] = true;
	if (p.z > 1.0) res[6] = true;
	return res;
}

Vert Clipper::intersect(int mode, const Vert & v1, const Vert & v2) {
	double eps = 1e-6;
	double u;
	if (mode == 0)			// w = zNear;
		u = (camera.zNear() - eps - v1.coor.w) / (v2.coor.w - v1.coor.w);
	else if (mode == 1)		// ndc.x = -1.0
		u = (v1.coor.x + v1.coor.w * (1.0 + eps)) / (v1.coor.x - v2.coor.x + (v1.coor.w - v2.coor.w) * (1.0 + eps));
	else if (mode == 2)		// ndc.x = 1.0
		u = (v1.coor.x - v1.coor.w * (1.0 + eps)) / (v1.coor.x - v2.coor.x - (v1.coor.w - v2.coor.w) * (1.0 + eps));
	else if (mode == 3)		// ndc.y = -1.0
		u = (v1.coor.y + v1.coor.w * (1.0 + eps)) / (v1.coor.y - v2.coor.y + (v1.coor.w - v2.coor.w) * (1.0 + eps));
	else if (mode == 4)		// ndc.y = 1.0
		u = (v1.coor.y - v1.coor.w * (1.0 + eps)) / (v1.coor.y - v2.coor.y - (v1.coor.w - v2.coor.w) * (1.0 + eps));
	else if (mode == 5)		// ndc.z = -1.0
		u = (v1.coor.z + v1.coor.w * (1.0 + eps)) / (v1.coor.z - v2.coor.z + (v1.coor.w - v2.coor.w) * (1.0 + eps));
	else					// ndc.z = 1.0
		u = (v1.coor.z - v1.coor.w * (1.0 + eps)) / (v1.coor.z - v2.coor.z - (v1.coor.w - v2.coor.w) * (1.0 + eps));

	return Vert(
		v1.coor + (v2.coor - v1.coor) * u,
		v1.normal + (v2.normal - v1.normal) * u,
		v1.texcoords + (v2.texcoords - v1.texcoords) * u
	);
}
