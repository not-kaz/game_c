#include <glad/gl.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "shader.h"

#define GL_MSG_MAXLEN 1024

static int compile_shader(struct shader *shader)
{
	char err_msg[ERR_MSG_MAXLEN] = {'\0'};
	const char *tmp[1] = {NULL};
	int gl_res = 0;
	char gl_msg[GL_MSG_MAXLEN] = {'\0'};

	if (shader->type == SHADER_TYPE_VERTEX) {
		shader->gl_id = glCreateShader(GL_VERTEX_SHADER);
	} else if (shader->type == SHADER_TYPE_FRAGMENT) {
		shader->gl_id = glCreateShader(GL_FRAGMENT_SHADER);
	} else {
		snprintf(err_msg, ERR_MSG_MAXLEN,
			"Shader type mismatch. Vertex or fragment supported.");
		goto handle_err;
	}
	if (!shader->gl_id) {
		snprintf(err_msg, ERR_MSG_MAXLEN,
			"glCreateShader() has failed.");
		goto handle_err;
	}
	// HACK: Find a way to avoid having to use tmp ptr for glShaderSource().
	tmp[0] = shader->src;
	glShaderSource(shader->gl_id, 1, tmp, NULL);
	glCompileShader(shader->gl_id);
	glGetShaderiv(shader->gl_id, GL_COMPILE_STATUS, &gl_res);
	if (!gl_res) {
		glGetShaderInfoLog(shader->gl_id, GL_MSG_MAXLEN, NULL, gl_msg);
		snprintf(err_msg, ERR_MSG_MAXLEN,
			"glCompileShader() has failed.");
		goto handle_err;
	}
	shader->is_compiled = true;
	return RETURN_CODE_SUCCESS;
handle_err:
	if (shader->gl_id) {
		glDeleteShader(shader->gl_id);
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
	char err_msg[ERR_MSG_MAXLEN] = {'\0'};
	size_t len = 0;
	int gl_res = 0;
	char gl_msg[GL_MSG_MAXLEN] = {'\0'};

	if (!program || !name || !vertex_shader_src || !fragment_shader_src) {
		snprintf(err_msg, ERR_MSG_MAXLEN,
			"One or more arguments are not valid.");
		goto handle_err;
	}
	if (program->is_inited) {
		snprintf(err_msg, ERR_MSG_MAXLEN, "Already initialized.");
		goto handle_err;
	}
	len = strlen(name);
	if (!len || len >= SHADER_PROGRAM_NAME_MAXLEN) {
		snprintf(err_msg, ERR_MSG_MAXLEN,
			"Name is not specified or exceeds limit.");
		goto handle_err;
	}
	memset(program, 0, sizeof(struct shader_program));
	program->gl_id = glCreateProgram();
	strncpy(program->name, name, len);
	len = strlen(vertex_shader_src);
	if (!len || len >= SHADER_SRC_MAXLEN) {
		snprintf(err_msg, ERR_MSG_MAXLEN,
			"Vertex shader source is empty or exceeds limit.");
		goto handle_err;
	}
	strncpy(program->vertex_shader.src, vertex_shader_src, len);
	program->vertex_shader.type = SHADER_TYPE_VERTEX;
	len = strlen(fragment_shader_src);
	if (!len || len >= SHADER_SRC_MAXLEN) {
		snprintf(err_msg, ERR_MSG_MAXLEN,
			"Fragment shader source is empty or exceeds limit.");
		goto handle_err;
	}
	strncpy(program->fragment_shader.src, fragment_shader_src, len);
	program->fragment_shader.type = SHADER_TYPE_FRAGMENT;
	compile_shader(&program->vertex_shader);
	compile_shader(&program->fragment_shader);
	if (!program->vertex_shader.is_compiled ||
			!program->fragment_shader.is_compiled) {
		snprintf(err_msg, ERR_MSG_MAXLEN,
			"One or more shader compilations failed.");
		goto handle_err;
	}
	glAttachShader(program->gl_id, program->vertex_shader.gl_id);
	glAttachShader(program->gl_id, program->fragment_shader.gl_id);
	glLinkProgram(program->gl_id);
	glGetProgramiv(program->gl_id, GL_LINK_STATUS, &gl_res);
	if (!gl_res) {
		glGetProgramInfoLog(program->gl_id, GL_MSG_MAXLEN, NULL,
			gl_msg);
		snprintf(err_msg, ERR_MSG_MAXLEN,
			"glLinkProgram() has failed.");
		goto handle_err;
	}
	program->is_inited = true;
	return RETURN_CODE_SUCCESS;
handle_err:
	fprintf(stderr, "shader_program_init() has failed: %s\n", err_msg);
	if (gl_msg[0] != '\0') {
		fprintf(stderr, "%s\n", gl_msg);
	}
	memset(program, 0, sizeof(struct shader_program));
	return RETURN_CODE_FAILURE;
}

void shader_program_finish(struct shader_program *program)
{
	glDeleteShader(program->vertex_shader.gl_id);
	glDeleteShader(program->fragment_shader.gl_id);
	glDeleteProgram(program->gl_id);
	memset(program, 0, sizeof(struct shader_program));
}

void shader_program_bind(struct shader_program *program)
{
	glUseProgram(program->gl_id);
}

int shader_program_set_uniform_val(struct shader_program *program,
		const char *uniform_name, enum shader_uniform_type type,
		const void *val)
{
	char err_msg[ERR_MSG_MAXLEN] = {'\0'};
	int loc = -1;

	if (!program || !uniform_name || !val) {
		snprintf(err_msg, ERR_MSG_MAXLEN,
			"Unable to set shader uniform value. Invalid params.");
		goto handle_err;
	}
	loc = glGetUniformLocation(program->gl_id, uniform_name);
	if (loc < 0) {
		snprintf(err_msg, ERR_MSG_MAXLEN,
			"glGetUniformLocation() failed. Name (%s) not found.",
			uniform_name);
		goto handle_err;
	}
	switch (type) {
	case SHADER_UNIFORM_TYPE_MAT4:
		glUniformMatrix4fv(loc, 1, GL_FALSE, (const float *)(val));
		break;
	default:
		snprintf(err_msg, ERR_MSG_MAXLEN, "Incorrect type provided.");
		goto handle_err;
	}
	return RETURN_CODE_SUCCESS;
handle_err:
	fprintf(stderr, "shader_program_set_uniform_val() has failed: %s\n",
		err_msg);
	return RETURN_CODE_FAILURE;
}

