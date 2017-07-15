#include "Renderer.h"
#include "Utils.h"

#include <glm/ext.hpp>

#include <algorithm>
#include <iostream>

void Renderer::setCamera(const Camera & camera_) {
	camera = camera_;
}

void Renderer::render(const Scene & scene) {
	this->frame->setAll(glm::dvec3(0.0, 0.0, 0.0));
	this->depth.setAll(1.0);

	for (const auto &mesh : scene.meshes) {
		for (const auto &tri : mesh.tris)
			renderTri(tri);
	}
}

glm::dmat4 Renderer::viewPortMat() {
	return glm::transpose(glm::dmat4(
		frame->w * 0.5, 0.0, 0.0, frame->w * 0.5,
		0.0, frame->h * 0.5, 0.0, frame->h * 0.5,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	));
}

glm::dvec4 Renderer::model2ViewPort(glm::dvec4 coor) {
	glm::dvec4 res = camera.projMat() * camera.viewMat() * coor;
	return glm::dvec4(this->viewPortMat() * (res / res[3]));
}

glm::dvec3 Renderer::pixelShader(const Tri &tri, double u, double v, glm::dvec3 fragPos) {
	double ambientStrength = 0.1;
	double diffuseStrength = 0.5;
	double specularStrength = 0.2;
	double shininess = 32.0;

	glm::dvec3 lightPos(1.0, 0.0, 0.0);
	glm::dvec3 viewPos = camera.eye();

	glm::dvec3 color(1.0, 0.0, 0.0);
	glm::dvec3 lightColor(1.0, 1.0, 1.0);
	glm::dvec3 ambient = ambientStrength * lightColor;

	glm::dvec3 normal = glm::normalize(glm::dvec3((1 - u - v) * tri.verts[0].normal + u * tri.verts[1].normal + v * tri.verts[2].normal));
	glm::dvec3 lightDir = glm::normalize(lightPos - fragPos);
	glm::dvec3 diffuse = diffuseStrength * std::max(glm::dot(normal, lightDir), 0.0) * lightColor;

	glm::dvec3 viewDir = glm::normalize(viewPos - fragPos);
	glm::dvec3 reflDir = glm::reflect(-lightDir, normal);
	float spec = std::pow(std::max(glm::dot(viewDir, reflDir), 0.0), shininess);
	glm::dvec3 specular = specularStrength * spec * lightColor;

	return (ambient + diffuse + specular) * color;
}

std::vector<glm::ivec2> Renderer::flatTriPositions(glm::dvec2 p1, glm::dvec2 p2, glm::dvec2 p3) {
	std::vector<glm::ivec2> posVec;

	double y0 = p1.y, y1 = p2.y;
	if (p2.y < p1.y)
		std::swap(y0, y1);

	for (int y = int(floor(y0)); y <= int(ceil(y1)); ++y) {
		double factor = (y - p1.y) * 1.0 / (p2.y - p1.y);
		double leftX = p1.x + (p2.x - p1.x) * factor;
		double rightX = p1.x + (p3.x - p1.x) * factor;

		// 只要保证两端在三角形内部，中间部分就一定在内部
		int x0 = int(floor(leftX)), x1 = int(ceil(rightX));
		while (!ptInTri(glm::dvec2(x0, y), p1, p2, p3) && x0 <= x1) ++x0;
		while (!ptInTri(glm::dvec2(x1, y), p1, p2, p3) && x1 >= x0) --x1;
		for (int x = x0; x <= x1; ++x)
			posVec.emplace_back(x, y);
	}

	return posVec;
}

void Renderer::renderTri(const Tri & tri) {
	glm::dvec2 coors[3];
	glm::dvec4 verts[3];

	int idx = 0;
	for (const auto &vert : tri.verts) {
		verts[idx] = model2ViewPort(vert.coor);
		coors[idx] = glm::dvec2(verts[idx]);
		idx++;
	}

	glm::dvec2 p1 = coors[1], p2 = coors[0], p3 = coors[2];
	if (in(coors[1].y, coors[2].y, coors[0].y)) {
		p1 = coors[2];
		p2 = coors[1];
		p3 = coors[0];
	} else if (in(coors[2].y, coors[0].y, coors[1].y)) {
		p1 = coors[0];
		p2 = coors[2];
		p3 = coors[1];
	}

	std::vector<glm::ivec2> triPos;
	glm::dvec2 p4;
	p4.x = (p2.y * (p3.x - p1.x) - p1.y * p3.x + p3.y * p1.x) / (p3.y - p1.y);
	p4.y = p2.y;
	if (p2.x > p4.x)
		std::swap(p2, p4);
	if (p1.y < p3.y)
		std::swap(p1, p3);

	if (!(p1.y == p2.y)) {
		auto tmpPos = flatTriPositions(p1, p2, p4);
		triPos.insert(triPos.end(), tmpPos.begin(), tmpPos.end());
	}
	if (!(p3.y == p2.y)) {
		auto tmpPos = flatTriPositions(p3, p2, p4);
		triPos.insert(triPos.end(), tmpPos.begin(), tmpPos.end());
	}

	glm::dmat2 tmpMat = glm::inverse(glm::dmat2(
		coors[1].x - coors[0].x, coors[2].x - coors[0].x,
		coors[1].y - coors[0].y, coors[2].y - coors[0].y
	));
	for (const auto &pos : triPos) {
		int x = pos.x, y = pos.y;

		glm::dvec2 uv = glm::dvec2(pos.x - coors[0].x, pos.y - coors[0].y) * tmpMat;
		double u = uv.x, v = uv.y;

		glm::dvec3 fragPos = glm::dvec3((1 - u - v) * tri.verts[0].coor + u * tri.verts[1].coor + v * tri.verts[2].coor);
		glm::dvec4 fragScreenPos = model2ViewPort(glm::dvec4(fragPos, 1.0));

		double z = fragScreenPos[2];
		if (z < depth.at(x, y)) {
			depth.at(x, y) = z;
			frame->at(x, y) = pixelShader(tri, u, v, fragPos);
		}
	}
}
