#pragma once

#include "Renderer.h"
#include "ShadowShader.h"

class PhongShader : public Renderer<glm::dvec3> {
public:
	struct Light {
		glm::dvec3 pos;
		glm::dvec3 color;
		double ambientStrength;
		double diffuseStrength;
		double specularStrength;
	};

private:
	int shadowMapDim;
	ShadowShader shadowShader;
	Light light;
	std::vector<tinyobj::material_t> materials;

public:
	PhongShader() : Renderer<glm::dvec3>() {}
	PhongShader(int w, int h) : Renderer<glm::dvec3>(w, h) {}
	~PhongShader() {}

	void setLight(const Light &light_);
	void setShadowMapDim(int dim);

	virtual glm::dvec3 fragShader(const Tri &tri, glm::dvec3 fragPos, glm::dvec3 normal) override;

	virtual void render(const Scene &scene) override;
};