#include <glad/gl.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "err_msg.h"
#include "texture.h"

#include "result_code.h"

int texture_init(struct texture *texture, const char *name,
		const char *img_filepath)
{
	size_t len = 0;
	SDL_Surface *surf = NULL;

	if (!texture || !name || !img_filepath) {
		err_msg_set("Invalid parameters.");
		goto handle_err;
	}
	if (texture->name[0] != '\0') {
		err_msg_set("Texture is already initialized.");
		goto handle_err;
	}
	// COPY name
	len = strlen(name);
	if (!len || len >= TEXTURE_NAME_MAXLEN) {
		err_msg_set("Texture name is empty or exceeds limit.");
		goto handle_err;
	}
	memset(texture, 0, sizeof(struct texture));
	strncpy(texture->name, name, len);
	surf = IMG_Load(img_filepath);
	if (!surf) {
		err_msg_set(IMG_GetError());
		goto handle_err;
	}
	// HACK: The following parameters are currently hardcoded but should be
	//	 adjustable later on.
	// TODO: Check if any of the following functions return on failure.
	glGenTextures(1, &texture->gl_id);
	glBindTexture(GL_TEXTURE_2D, texture->gl_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surf->w, surf->h, 0, GL_RGB,
		GL_UNSIGNED_BYTE, surf->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_FreeSurface(surf);
	return RESULT_CODE_SUCCESS;
handle_err:
	fprintf(stderr, "texture_init() has failed. %s\n", err_msg_get());
	return RESULT_CODE_UNSPECIFIED_ERROR;
}

void texture_finish(struct texture *texture)
{
	glDeleteTextures(1, &texture->gl_id);
	memset(texture, 0, sizeof(struct texture));
}

int texture_bind(struct texture *texture)
{
	if (texture == NULL) {
		fprintf(stderr, "texture_bind() has failed.\n");
		return RESULT_CODE_NULL_PTR;
	}
	if (texture->name[0] == '\0') {
		return RESULT_CODE_PARAM_INVALID;
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->gl_id);
	return RESULT_CODE_SUCCESS;
}
