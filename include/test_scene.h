#ifndef TEST_SCENE_H
#define TEST_SCENE_H
#include <cgltf.h>
#include <cglm/cglm.h>
#include "mesh.h"
#include "shader.h"
#include "texture.h"
unsigned int vbo, vao;
struct shader_program shd;
struct texture tex;
mat4 view, model, projection;
int ww, wh;
struct mesh avocado;
cgltf_options options = {0};
cgltf_data *data = NULL;
const char *vs =
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec2 aNormal;\n"
	"layout (location = 2) in vec2 aTexCoords;\n"
	"out vec2 TexCoords;\n"
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 projection;\n"
	"void main()\n"
	"{\n"
	"    TexCoords = aTexCoords;\n"
	"    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
	"}\0";
const char *fs =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec2 TexCoords;\n"
	"uniform sampler2D texture1;\n"
	"void main()\n"
	"{\n"
	"	FragColor = texture(texture1, TexCoords);\n"
	"}\0";

static void impl_test_scene(void)
{
	graphics_get_window_size(&ww, &wh);
	printf("%d, %d\n", ww, wh);
	shader_program_init(&shd, "test shader", vs, fs);
	texture_init(&tex, "container", "../assets/Avocado_baseColor.png");
	shader_program_bind(&shd);
	texture_bind(&tex);
	cgltf_parse_file(&options, "../assets/Avocado.gltf", &data);
	cgltf_load_buffers(&options, data, "../assets/Avocado.bin");
	mesh_init(&avocado, &data->meshes[0].primitives[0]);
}

static void run_test_scene(void)
{
	glm_mat4_identity(model);
	glm_mat4_identity(view);
	glm_mat4_identity(projection);
	glm_rotate(model, ((float) SDL_GetTicks() / 1000.0f) * glm_rad(50.0f),
		(vec3){0.0f, 0.5f, 0.0f});
	glm_perspective(glm_rad(60.0f), (float) ww / (float) wh, 0.1f, 100.0f,
		projection);
	glm_translate(view, (vec3){0.0f, -0.02f, -0.2f});
	shader_program_set_uniform_val(&shd, "model", SHADER_UNIFORM_TYPE_MAT4,
		model[0]);
	shader_program_set_uniform_val(&shd, "view", SHADER_UNIFORM_TYPE_MAT4,
		view[0]);
	shader_program_set_uniform_val(&shd, "projection",
		SHADER_UNIFORM_TYPE_MAT4, projection[0]);
	graphics_clear_framebuffer();
	/*glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);*/
	mesh_draw(&avocado);
	graphics_present_framebuffer();
	input_poll();
}

#endif
