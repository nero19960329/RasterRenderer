#include "Scene.h"

#include <iostream>

Scene::Scene(std::string filename) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;

	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str());
	if (!err.empty())
		std::cerr << err << std::endl;
	if (!ret)
		exit(1);

	std::cout << "There are " << shapes.size() << " meshes." << std::endl;
	for (auto s = 0; s < shapes.size(); ++s)
		meshes.emplace_back(shapes[s], materials, attrib);
}