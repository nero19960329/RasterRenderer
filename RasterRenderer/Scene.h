#pragma once

#include "Mesh.h"
#include "tiny_obj_loader.h"

class Scene {
public:
	std::vector<Mesh> meshes;
	std::vector<tinyobj::material_t> materials;

public:
	Scene() {}
	explicit Scene(std::string filename);
	~Scene() {}
};