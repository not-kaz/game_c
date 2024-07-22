#ifndef TEXTURE_H
#define TEXTURE_H

#define TEXTURE_NAME_MAXLEN 32

struct texture {
	char name[TEXTURE_NAME_MAXLEN];
	unsigned int gl_id;
};

int texture_init(struct texture *texture, const char *name,
	const char *img_filepath);
void texture_finish(struct texture *texture);
int texture_bind(const struct texture *texture);

#endif
