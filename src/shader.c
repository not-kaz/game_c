#include <glad/gl.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "shader.h"

#define GL_MSG_MAXLEN 1024

static int compile_shader(struct shader *shader)
{
	const char *err_msg = NULL;
	int gl_res = 0;
	char gl_msg[GL_MSG_MAXLEN] = {'\0'};
	const char *tmp[1] = {NULL};

	if (shader->type == SHADER_TYPE_VERTEX) {
		shader->id = glCreateShader(GL_VERTEX_SHADER);
	} else if (shader->type == SHADER_TYPE_FRAGMENT) {
		shader->id = glCreateShader(GL_FRAGMENT_SHADER);
	} else {
		err_msg = "Shader type mismatch. Vertex or fragment supported.";
		goto handle_err;
	}
	if (!shader->id) {
		err_msg = "glCreateShader() has failed.";
		goto handle_err;
	}
	// HACK: Find a way to avoid having to use tmp ptr for glShaderSource().
	tmp[0] = shader->src;
	glShaderSource(shader->id, 1, tmp, NULL);
	glCompileShader(shader->id);
	glGetShaderiv(shader->id, GL_COMPILE_STATUS, &gl_res);
	if (!gl_res) {
		glGetShaderInfoLog(shader->id, GL_MSG_MAXLEN, NULL, gl_msg);
		err_msg = "glCompileShader() has failed.";
		goto handle_err;
	}
	shader->is_compiled = true;
	return RETURN_CODE_SUCCESS;
handle_err:
	if (shader->id) {
		glDeleteShader(shader->id);
	}
	fprintf(stderr, "Shader compilation failed: %s\n", err_msg);
	if (gl_msg[0] != '\0') {
		fprintf(stderr, "%s\n", gl_msg);
	}
	return RETURN_CODE_FAILURE;
}

int shader_program_init(struct shader_program *program, const char *name,
		const char *vertex_shader_src, const char *fragment_shader_src)
{
	size_t len = 0;
	const char *err_msg = NULL;
	int gl_res = 0;
	char gl_msg[GL_MSG_MAXLEN] = {'\0'};

	if (!program || !name || !vertex_shader_src || !fragment_shader_src){
		err_msg = "One or more arguments are not valid.";
		goto handle_err;
	}
	if (program->is_inited) {
		err_msg = "Already initialized.";
		goto handle_err;
	}
	len = strlen(name);
	if (!len || len >= SHADER_PROGRAM_NAME_MAXLEN) {
		err_msg = "Name is not specified or exceeds limit.";
		goto handle_err;
	}
	memset(program, 0, sizeof(struct shader_program));
	program->id = glCreateProgram();
	strncpy(program->name, name, len);
	len = strlen(vertex_shader_src);
	if (!len || len >= SHADER_SRC_MAXLEN) {
		err_msg = "Vertex shader source is empty or exceeds limit.";
		goto handle_err;
	}
	strncpy(program->vertex_shader.src, vertex_shader_src, len);
	program->vertex_shader.type = SHADER_TYPE_VERTEX;
	len = strlen(fragment_shader_src);
	if (!len || len >= SHADER_SRC_MAXLEN) {
		err_msg = "Fragment shader source is empty or exceeds limit.";
		goto handle_err;
	}
	strncpy(program->fragment_shader.src, fragment_shader_src, len);
	program->fragment_shader.type = SHADER_TYPE_FRAGMENT;
	compile_shader(&program->vertex_shader);
	compile_shader(&program->fragment_shader);
	if (!program->vertex_shader.is_compiled ||
			!program->fragment_shader.is_compiled) {
		err_msg = "One or more shader compilations failed.";
		goto handle_err;
	}
	glAttachShader(program->id, program->vertex_shader.id);
	glAttachShader(program->id, program->fragment_shader.id);
	glLinkProgram(program->id);
	glGetProgramiv(program->id, GL_LINK_STATUS, &gl_res);
	if (!gl_res) {
		glGetProgramInfoLog(program->id, GL_MSG_MAXLEN, NULL, gl_msg);
		err_msg = "glLinkProgram() has failed.";
		goto handle_err;
	}
	program->is_inited = true;
	return RETURN_CODE_SUCCESS;
handle_err:
	fprintf(stderr, "Shader program initialization failed: %s\n", err_msg);
	if (gl_msg[0] != '\0') {
		fprintf(stderr, "%s\n", gl_msg);
	}
	memset(program, 0, sizeof(struct shader_program));
	return RETURN_CODE_FAILURE;
}

void shader_program_finish(struct shader_program *program)
{
	glDeleteShader(program->vertex_shader.id);
	glDeleteShader(program->fragment_shader.id);
	glDeleteProgram(program->id);
	memset(program, 0, sizeof(struct shader_program));
}
