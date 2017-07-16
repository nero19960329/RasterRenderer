#include "Renderer.h"
#include "Utils.h"

#include <glm/ext.hpp>

#include <algorithm>
#include <iostream>

void Renderer::setCamera(const Camera &camera_) {
	camera = camera_;
}

void Renderer::render(const Scene &scene) {
	this->frame->setAll(glm::dvec3(0.0, 0.0, 0.0));
	this->depth.setAll(1.0);
	this->materials = scene.materials;

	int idx = 0;
	for (const auto &mesh : scene.meshes) {
		std::cout << "Rendering " << idx << ". . . " << std::endl;
		for (const auto &tri : mesh.tris)
			renderTri(tri);
		std::cout << "Mesh " << idx << " done !" << std::endl;
		++idx;
	}
}

glm::dmat4 Renderer::viewPortMat() {
	glm::dmat4 viewPortMat = glm::transpose(glm::dmat4(
		frame->w * 0.5, 0.0, 0.0, frame->w * 0.5,
		0.0, frame->h * 0.5, 0.0, frame->h * 0.5,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	));
	invViewPortMat = glm::inverse(viewPortMat);
	return viewPortMat;
}

glm::dvec4 Renderer::model2ViewPort(glm::dvec4 coor) {
	glm::dvec4 res = camera.projMat() * camera.viewMat() * coor;
	return this->viewPortMat() * (res / res[3]);
}

bool Renderer::isValidCoor(glm::dvec3 v) {
	return frame->isValid(v.x, v.y) && in(v.z, -1.0, 1.0);
}

glm::dvec3 Renderer::fragShader(const Tri &tri, glm::dvec3 fragPos, glm::dvec3 normal) {
	tinyobj::material_t mtl = materials[tri.materialId];
	glm::dvec3 ambientColor(mtl.ambient[0], mtl.ambient[1], mtl.ambient[2]);
	glm::dvec3 diffuseColor(mtl.diffuse[0], mtl.diffuse[1], mtl.diffuse[2]);
	glm::dvec3 specularColor(mtl.specular[0], mtl.specular[1], mtl.specular[2]);
	double shininess = mtl.shininess;

	glm::dvec3 lightPos(-0.005, 1.9, -0.03);
	double ambientStrength = 0.2;
	double diffuseStrength = 0.8;
	double specularStrength = 0.0;
	glm::dvec3 viewPos = camera.eye();

	glm::dvec3 color(1.0, 1.0, 1.0);
	glm::dvec3 lightColor(1.0, 1.0, 1.0);
	glm::dvec3 ambient = ambientStrength * ambientColor * lightColor;

	glm::dvec3 lightDir = glm::normalize(lightPos - fragPos);
	glm::dvec3 diffuse = diffuseStrength * diffuseColor * std::max(glm::dot(normal, lightDir), 0.0) * lightColor;

	glm::dvec3 viewDir = glm::normalize(viewPos - fragPos);
	glm::dvec3 reflDir = glm::reflect(-lightDir, normal);
	double spec = std::pow(std::max(glm::dot(viewDir, reflDir), 0.0), shininess);
	glm::dvec3 specular = specularStrength * specularColor * spec * lightColor;

	return ambient + diffuse + specular;
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

std::vector<glm::ivec2> Renderer::triPositions(glm::dvec2 coors[3]) {
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
	glm::dvec2 p4(
		(p2.y * (p3.x - p1.x) - p1.y * p3.x + p3.y * p1.x) / (p3.y - p1.y),
		p2.y
	);
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

	return triPos;
}

void Renderer::renderTri(const Tri & tri) {
	glm::dvec4 clipCoors[3];
	glm::dvec2 coors[3];
	for (int i = 0; i < 3; ++i) {
		clipCoors[i] = camera.projMat() * camera.viewMat() * tri.verts[i].coor;
		glm::dvec4 viewPortCoor = this->viewPortMat() * (clipCoors[i] / clipCoors[i].w);
		coors[i] = glm::dvec2(viewPortCoor);
	}

	glm::dvec3 normals[3];
	for (int i = 0; i < 3; ++i) {
		const glm::dvec3 &n = tri.verts[i].normal;
		if (n.x == 0.0 && n.y == 0.0 && n.z == 0.0) {
			normals[i] = glm::normalize(glm::cross(
				glm::dvec3(tri.verts[0].coor - tri.verts[1].coor),
				glm::dvec3(tri.verts[0].coor - tri.verts[2].coor)
			));
		} else
			normals[i] = n;
	}

	auto triPos = triPositions(coors);
	for (const auto &pos : triPos) {
		int x = pos.x, y = pos.y;
		if (!frame->isValid(x, y))
			continue;

		glm::dvec4 ndcCoor = invViewPortMat * glm::dvec4(x, y, 1, 1);
		glm::dvec3 abc =
			glm::transpose(glm::inverse(glm::dmat3(
				clipCoors[0].x, clipCoors[1].x, clipCoors[2].x,
				clipCoors[0].y, clipCoors[1].y, clipCoors[2].y,
				clipCoors[0].w, clipCoors[1].w, clipCoors[2].w
			))) * glm::dvec3(ndcCoor.x, ndcCoor.y, 1);
		abc /= (abc.x + abc.y + abc.z);

		double z = interpolate(abc, clipCoors[0].z, clipCoors[1].z, clipCoors[2].z) /
			interpolate(abc, clipCoors[0].w, clipCoors[1].w, clipCoors[2].w);
		if (in(z, -1.0, 1.0) && z < depth.at(x, y)) {
			depth.at(x, y) = z;

			glm::dvec3 fragPos = glm::dvec3(interpolate(abc, tri.verts[0].coor, tri.verts[1].coor, tri.verts[2].coor));
			glm::dvec3 normal = interpolate(abc, normals[0], normals[1], normals[2]);
			frame->at(x, y) = fragShader(tri, fragPos, normal);
		}
	}
}
