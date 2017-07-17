#include "PhongShader.h"

#include <algorithm>

void PhongShader::setLight(const Light & light_) {
	light = light_;
}

void PhongShader::setShadowMapDim(int dim) {
	shadowMapDim = dim;
}

glm::dvec3 PhongShader::fragShader(const Tri & tri, glm::dvec3 fragPos, glm::dvec3 normal) {
	tinyobj::material_t mtl;
	if (tri.materialId != -1)
		mtl = materials[tri.materialId];
	else {
		mtl.ambient[0] = mtl.ambient[1] = mtl.ambient[2] = 1.0;
		mtl.diffuse[0] = mtl.diffuse[1] = mtl.diffuse[2] = 1.0;
		mtl.specular[0] = mtl.specular[1] = mtl.specular[2] = 0.0;
		mtl.shininess = 32.0;
	}
	glm::dvec3 ambientColor(mtl.ambient[0], mtl.ambient[1], mtl.ambient[2]);
	glm::dvec3 diffuseColor(mtl.diffuse[0], mtl.diffuse[1], mtl.diffuse[2]);
	glm::dvec3 specularColor(mtl.specular[0], mtl.specular[1], mtl.specular[2]);
	double shininess = mtl.shininess;

	glm::dvec3 ambient = light.ambientStrength * ambientColor * light.color;

	if (shadowMapDim > 0) {
		glm::dvec4 shadowMapCoor = shadowShader.model2ViewPort(glm::dvec4(fragPos, 1.0));
		int projX = int(round(shadowMapCoor.x)), projY = int(round(shadowMapCoor.y));
		if (projX < 0 || projX >= shadowShader.frame->w || projY < 0 || projY >= shadowShader.frame->h)
			;
		else if (shadowShader.frame->at(projX, projY) < glm::dot(fragPos - light.pos, fragPos - light.pos) * 0.9)
			return ambient;
	}

	glm::dvec3 lightDir = glm::normalize(light.pos - fragPos);
	glm::dvec3 diffuse = light.diffuseStrength * diffuseColor * std::max(glm::dot(normal, lightDir), 0.0) * light.color;

	glm::dvec3 viewPos = camera.eye();
	glm::dvec3 viewDir = glm::normalize(viewPos - fragPos);
	glm::dvec3 reflDir = glm::reflect(-lightDir, normal);
	double spec = std::pow(std::max(glm::dot(viewDir, reflDir), 0.0), shininess);
	glm::dvec3 specular = light.specularStrength * specularColor * spec * light.color;

	return ambient + diffuse + specular;
}

void PhongShader::render(const Scene & scene) {
	Camera lightCam;
	lightCam.lookAt(
		light.pos,
		camera.center(),
		{ 0.0, 1.0, 0.0 }
	);
	lightCam.perspective(90.0, 1.0, 0.1, 100.0);

	if (shadowMapDim > 0) {
		shadowShader = ShadowShader(shadowMapDim, shadowMapDim);
		shadowShader.setCamera(lightCam);
		shadowShader.render(scene);
	}

	this->materials = scene.materials;
	Renderer::render(scene);
}
