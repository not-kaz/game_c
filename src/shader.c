#include <glad/gl.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "shader.h"

#define GL_MSG_MAXLEN 1024

static int compile_shader(struct shader *shader)
{
	int gl_res;
	char gl_msg[GL_MSG_MAXLEN];
	const char *tmp[1]; // Required to supress glShaderSource() warnings.

	if (shader->type == SHADER_TYPE_VERTEX) {
		shader->id = glCreateShader(GL_VERTEX_SHADER);
	} else if (shader->type == SHADER_TYPE_FRAGMENT) {
		shader->id = glCreateShader(GL_FRAGMENT_SHADER);
	} else {
		fprintf(stderr, "Shader compilation failed. Type mismatch.\n");
		return RETURN_CODE_FAILURE;
	}
	if (!shader->id) {
		fprintf(stderr, "Shader compilation failed. "
				"glCreateShader() was not successful.\n");
		return RETURN_CODE_FAILURE;
	}
	tmp[0] = shader->src;
	glShaderSource(shader->id, 1, tmp, NULL);
	glCompileShader(shader->id);
	glGetShaderiv(shader->id, GL_COMPILE_STATUS, &gl_res);
	if (!gl_res) {
		glDeleteShader(shader->id);
		glGetShaderInfoLog(shader->id, GL_MSG_MAXLEN, NULL, gl_msg);
		fprintf(stderr, "Shader compilation failed. %s\n", gl_msg);
		return RETURN_CODE_FAILURE;
	}
	shader->is_compiled = true;
	return RETURN_CODE_SUCCESS;
}

int shader_program_init(struct shader_program *program, const char *name,
		const char *vertex_shader_src, const char *fragment_shader_src)
{
	int gl_res;
	char gl_msg[GL_MSG_MAXLEN];

	if (!program || !name || !vertex_shader_src || !fragment_shader_src){
		return RETURN_CODE_FAILURE;
	}
	memset(program, 0, sizeof(struct shader_program));
	program->id = glCreateProgram();
	program->name = name;
	// TODO: Check if shader sources are zero length / empty.
	program->vertex_shader.src = vertex_shader_src;
	program->vertex_shader.type = SHADER_TYPE_VERTEX;
	program->fragment_shader.src = fragment_shader_src;
	program->fragment_shader.type = SHADER_TYPE_FRAGMENT;
	compile_shader(&program->vertex_shader);
	compile_shader(&program->fragment_shader);
	if (!program->vertex_shader.is_compiled ||
			!program->fragment_shader.is_compiled) {
		return RETURN_CODE_FAILURE;
	}
	glAttachShader(program->id, program->vertex_shader.id);
	glAttachShader(program->id, program->fragment_shader.id);
	glLinkProgram(program->id);
	glGetProgramiv(program->id, GL_LINK_STATUS, &gl_res);
	if (!gl_res) {
		glGetProgramInfoLog(program->id, GL_MSG_MAXLEN, NULL, gl_msg);
		fprintf(stderr, "Shader program (%s) linking failed. %s\n",
				name, gl_msg);
		return RETURN_CODE_FAILURE;
	}
	return RETURN_CODE_SUCCESS;
}

void shader_program_finish(struct shader_program *program)
{
	glDeleteShader(program->vertex_shader.id);
	glDeleteShader(program->fragment_shader.id);
	glDeleteProgram(program->id);
}
