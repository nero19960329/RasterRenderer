#pragma once

#include "Renderer.h"

class ShadowShader : public Renderer<double> {
public:
	ShadowShader() : Renderer<double>() {}
	ShadowShader(int w, int h) : Renderer<double>(w, h) {}
	~ShadowShader() {}

	virtual double fragShader(const Tri &tri, glm::dvec3 fragPos, glm::dvec3 normal) override {
		glm::dvec3 delta = fragPos - camera.eye();
		return glm::dot(delta, delta);
	}

	virtual void render(const Scene &scene) override {
		this->frame->setAll(DBL_MAX);
		Renderer<double>::render(scene);
	}
};