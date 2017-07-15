#pragma once

#include "Camera.h"
#include "Canvas.h"
#include "Scene.h"
#include "Tri.h"

#include <vector>

class Renderer {
public:
	Canvas<glm::dvec3> *frame;
	Canvas<double> depth;
	Camera camera;

public:
	Renderer() {}
	Renderer(int w, int h) : depth(w, h) { frame = new Canvas<glm::dvec3>(w, h); }
	~Renderer() {}

	void setCamera(const Camera &camera_);

	void render(const Scene &scene);

private:
	glm::dmat4 viewPortMat();
	glm::dvec4 model2ViewPort(glm::dvec4 coor);

	glm::dvec3 pixelShader(const Tri &tri, double u, double v, glm::dvec3 fragPos);

	std::vector<glm::ivec2> flatTriPositions(glm::dvec2 p1, glm::dvec2 p2, glm::dvec2 p3);
	void renderTri(const Tri &tri);
};
