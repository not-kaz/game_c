#ifndef SHADER_H
#define SHADER_H

enum shader_program_uniform_type {
	SHADER_PROGRAM_UNIFORM_TYPE_VEC3,
	SHADER_PROGRAM_UNIFORM_TYPE_NUMOF
};

enum shader_type {
	SHADER_TYPE_VERTEX,
	SHADER_TYPE_FRAGMENT,
	SHADER_TYPE_NUMOF
};

struct shader {
	const char *src;
	unsigned int id;
	enum shader_type type;
	bool is_compiled;
};

struct shader_program {
	const char *name;
	unsigned int id;
	struct shader vertex_shader;
	struct shader fragment_shader;
};


int shader_program_init(struct shader_program *program, const char *name,
		const char *vertex_shader_src, const char *fragment_shader_src);
void shader_program_finish(struct shader_program *program);

#endif
