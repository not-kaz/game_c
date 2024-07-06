#include <cgltf.h>
#include <glad/gl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "mesh.h"
#include "result_code.h"

static bool are_attr_sizes_equal(const cgltf_primitive *prim)
{
	if (prim->attributes_count > 1) {
		return true;
	}
	for (size_t i = 1; i < prim->attributes_count; i++) {
		if (prim->attributes[i].data->count
			!= prim->attributes[i - 1].data->count) {
			return false;
		}
	}
	return true;
}

static int parse_attr_data(struct mesh *mesh, const cgltf_attribute *attr)
{
	const size_t num_comps = cgltf_num_components(attr->data->type);
	size_t offset;

	switch (attr->type) {
	case cgltf_attribute_type_position:
		offset = offsetof(struct mesh_vertex, position);
		break;
	case cgltf_attribute_type_normal:
		offset = offsetof(struct mesh_vertex, normal);
		break;
	case cgltf_attribute_type_texcoord:
		offset = offsetof(struct mesh_vertex, tex_coord);
		break;
	case cgltf_attribute_type_color:
		offset = offsetof(struct mesh_vertex, color);
		break;
	default:
		return RESULT_CODE_SUCCESS;
	}
	for (size_t i = 0; i < attr->data->count; i++) {
		void *ptr = (char *) &mesh->vertices[i] + offset;

		if (!cgltf_accessor_read_float(attr->data, i, ptr, num_comps)) {
			return RESULT_CODE_READ_ERROR;
		}
	}
	return RESULT_CODE_SUCCESS;
}

static void print_mesh_data(const struct mesh *mesh)
{
	for (size_t i = 0; i < mesh->num_vertices; i++) {
		printf("pos: %f, %f, %f, norm: %f, %f, %f, tex: %f, %f\n",
			mesh->vertices[i].position[0],
			mesh->vertices[i].position[1],
			mesh->vertices[i].position[2],
			mesh->vertices[i].normal[0],
			mesh->vertices[i].normal[1],
			mesh->vertices[i].normal[2],
			mesh->vertices[i].tex_coord[0],
			mesh->vertices[i].tex_coord[1]);
	}
}

int mesh_init(struct mesh *mesh, const cgltf_primitive *prim)
{
	if (mesh == NULL || prim == NULL) {
		return RESULT_CODE_NULL_PTR;
	}
	if (!are_attr_sizes_equal(prim)) {
		return RESULT_CODE_UNSPECIFIED_ERROR;
	}
	if (prim->attributes[0].data->count > MESH_MAX_NUM_VERTICES) {
		return RESULT_CODE_LIMIT_EXCEEDED;
	}
	for (size_t i = 0; i < prim->attributes_count; i++) {
		if (parse_attr_data(mesh, &prim->attributes[i])) {
			return RESULT_CODE_READ_ERROR;
		}
	}
	print_mesh_data(mesh);
	mesh->num_vertices = prim->attributes[0].data->count;
	for (size_t i = 0; i < prim->indices->count; i++) {
		cgltf_accessor_read_uint(prim->indices, i,
			&mesh->indices[i], sizeof(unsigned int));
	}
	mesh->num_indices = prim->indices->count;
	glGenVertexArrays(1, &mesh->vao);
	glGenBuffers(1, &mesh->vbo);
	glGenBuffers(1, &mesh->ebo);
	glBindVertexArray(mesh->vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBufferData(GL_ARRAY_BUFFER,
		mesh->num_vertices * sizeof(struct mesh_vertex),
		&mesh->vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		mesh->num_indices * sizeof(unsigned int), &mesh->indices[0],
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct mesh_vertex), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		sizeof(struct mesh_vertex),
		(void *)offsetof(struct mesh_vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
		sizeof(struct mesh_vertex),
		(void *)offsetof(struct mesh_vertex, tex_coord));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,
		sizeof(struct mesh_vertex),
		(void *)offsetof(struct mesh_vertex, color));
	glBindVertexArray(0);
	print_mesh_data(mesh);
	return RESULT_CODE_SUCCESS;
}

void mesh_finish(struct mesh *mesh)
{
	memset(mesh, 0, sizeof(struct mesh));
}

void mesh_draw(const struct mesh *mesh)
{
	glBindVertexArray(mesh->vao);
	glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}