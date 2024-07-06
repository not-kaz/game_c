#ifndef SHADER_H
#define SHADER_H
#include <stdbool.h>

#define SHADER_SRC_MAXLEN 1024
#define SHADER_PROGRAM_NAME_MAXLEN 32

enum shader_uniform_type {
	SHADER_UNIFORM_TYPE_MAT4,
	SHADER_UNIFORM_TYPE_NUM_OF
};

enum shader_type {
	SHADER_TYPE_VERTEX,
	SHADER_TYPE_FRAGMENT,
	SHADER_TYPE_NUM_OF
};

struct shader {
	char src[SHADER_SRC_MAXLEN];
	unsigned int gl_id;
	enum shader_type type;
	bool is_compiled;
};

struct shader_program {
	char name[SHADER_PROGRAM_NAME_MAXLEN];
	unsigned int gl_id;
	struct shader vertex_shader;
	struct shader fragment_shader;
	bool is_inited;
};

int shader_program_init(struct shader_program *program, const char *name,
	const char *vertex_shader_src, const char *fragment_shader_src);
void shader_program_finish(struct shader_program *program);
void shader_program_bind(struct shader_program *program);
int shader_program_set_uniform_val(struct shader_program *program,
	const char *uniform_name, enum shader_uniform_type type,
	const void *val);
#endif
