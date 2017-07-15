#pragma once

#include "Mesh.h"

class Scene {
public:
	std::vector<Mesh> meshes;

public:
	Scene() {}
	explicit Scene(std::string filename);
	~Scene() {}
};