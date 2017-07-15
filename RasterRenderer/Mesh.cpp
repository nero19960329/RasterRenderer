#include "Mesh.h"

#include <iostream>

Mesh::Mesh(const tinyobj::shape_t & shape, const tinyobj::attrib_t & attrib) {
	size_t index_offset = 0;
	for (auto f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
		//std::cout << "Face " << f << " : " << std::endl;

		int fv = shape.mesh.num_face_vertices[f];
		if (fv != 3) {
			std::cout << "It's not triangle mesh!" << std::endl;
			exit(1);
		}

		Vert verts[3];
		for (auto v = 0; v < fv; ++v) {
			tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
			tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
			tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
			tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
			tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
			tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
			tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
			//tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
			//tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];

			//std::cout << "Vert " << v << " : " << std::endl;
			//std::cout << vx << ", " << vy << ", " << vz << std::endl;
			//std::cout << nx << ", " << ny << ", " << nz << std::endl;
			//std::cout << tx << ", " << ty << std::endl;

			verts[v] = Vert(
				{ vx, vy, vz },
				{ nx, ny, nz }/*,
				{ tx, ty }*/
			);
			//verts[v] = Vert({ vx, vy, vz });
		}
		tris.emplace_back(verts);

		index_offset += fv;

		// per-face material
		// shapes[s].mesh.material_ids[f];
	}
}
