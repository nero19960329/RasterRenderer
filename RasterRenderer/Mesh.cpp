#include "Mesh.h"

#include <iostream>

Mesh::Mesh(const tinyobj::shape_t & shape, const std::vector<tinyobj::material_t> &materials, const tinyobj::attrib_t & attrib) {
	size_t index_offset = 0;
	for (auto f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
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

			tinyobj::real_t nx = 0.0, ny = 0.0, nz = 0.0;
			if (3 * idx.normal_index + 2 <= attrib.normals.size()) {
				nx = attrib.normals[3 * idx.normal_index + 0];
				ny = attrib.normals[3 * idx.normal_index + 1];
				nz = attrib.normals[3 * idx.normal_index + 2];
			}

			tinyobj::real_t tx = 0.0, ty = 0.0;
			if (3 * idx.texcoord_index + 1 <= attrib.texcoords.size()) {
				tx = attrib.texcoords[2 * idx.texcoord_index + 0];
				ty = attrib.texcoords[2 * idx.texcoord_index + 1];
			}

			verts[v] = Vert(
				{ vx, vy, vz },
				{ nx, ny, nz },
				{ tx, ty }
			);
		}
		tris.emplace_back(verts, shape.mesh.material_ids[f]);

		index_offset += fv;
	}
}
