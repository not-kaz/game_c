#ifndef MESH_H
#define MESH_H
#include <stddef.h>

#define MESH_MAX_NUM_VERTICES 16384
#define MESH_MAX_NUM_INDICES 8192

struct cgltf_primitive;

struct mesh_vertex {
	float position[3];
	float normal[3];
	float tex_coord[2];
	float color[3];
};

struct mesh {
	struct mesh_vertex vertices[MESH_MAX_NUM_VERTICES];
	size_t num_vertices;
	unsigned int indices[MESH_MAX_NUM_INDICES];
	size_t num_indices;
	unsigned int vbo;
	unsigned int vao;
	unsigned int ebo;
};

int mesh_init(struct mesh *mesh, const struct cgltf_primitive *prim);
void mesh_finish(struct mesh *mesh);
void mesh_draw(const struct mesh *mesh);

#endif
