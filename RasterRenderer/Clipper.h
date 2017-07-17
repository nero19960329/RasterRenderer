#pragma once

#include "Camera.h"
#include "Tri.h"

#include <array>
#include <tuple>
#include <vector>

class Clipper {
private:
	const Camera &camera;

public:
	Clipper(const Camera &camera_) : camera(camera_) {}
	~Clipper() {}

	std::tuple<bool, std::vector<Tri>> clipTri(const Tri &tri);

private:
	std::array<bool, 7> mark(glm::dvec4 p);
	Vert intersect(int mode, const Vert &v1, const Vert &v2);
};