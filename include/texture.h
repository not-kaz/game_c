#ifndef TEXTURE_H
#define TEXTURE_H
#include <stdbool.h>

#define TEXTURE_NAME_MAXLEN 32

struct texture {
	char name[TEXTURE_NAME_MAXLEN];
	unsigned int gl_id;
	bool is_inited;
};

int texture_init(struct texture *texture, const char *name,
		const char *img_filepath);
void texture_finish(struct texture *texture);
int texture_bind(struct texture *texture);

#endif
