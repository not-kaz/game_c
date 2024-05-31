#include <glad/gl.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include "gfx.h"

#define MAX_NUM_DISPLAY_MODES 512

static SDL_Window *window;
static SDL_GLContext *gl_ctx;
static SDL_DisplayMode display_modes[MAX_NUM_DISPLAY_MODES];

static void fetch_display_modes(void)
{
	int display_idx;
	int num_modes;

	// NOTE: Assume main screen index is 0.
	display_idx = 0;
	num_modes = SDL_GetNumDisplayModes(display_idx);
	if (num_modes < 1 ) {
		fprintf(stderr, "Failed to fetch available display modes.\n");
		exit(EXIT_FAILURE);
	}
	if (num_modes > MAX_NUM_DISPLAY_MODES) {
		fprintf(stderr, "Number of found display modes found (%d) "
			"exceeds our limit (%d). Not all resolutions will be "
			"available.\n", num_modes, MAX_NUM_DISPLAY_MODES);
		num_modes = MAX_NUM_DISPLAY_MODES;
	}
	for (int i = 0; i < num_modes; i++) {
		// NOTE: All modes are zero initialized at definition.
		//	 Failed modes can be safely ignored later on.
		SDL_GetDisplayMode(display_idx, i, &display_modes[i]);
	}
}

void gfx_setup(void)
{
	int win_width;
	int win_height;
	unsigned int win_flag;

	fetch_display_modes();
	for (int i = 0; i < MAX_NUM_DISPLAY_MODES; i++) {
		if (display_modes[i].w != 0 && display_modes[i].h != 0) {
			win_width = display_modes[i].w;
			win_height = display_modes[i].h;
			break;
		}
	}
	// TODO: Handle loading window properties from a config file.
	win_flag = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN;
	window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, win_width, win_height, win_flag);
	if (!window) {
		fprintf(stderr, "Failed to create window. %s\n",
			SDL_GetError());
		exit(EXIT_FAILURE);
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
		SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	gl_ctx = SDL_GL_CreateContext(window);
	if (!gl_ctx) {
		fprintf(stderr, "Failed to create OpenGL context. %s\n",
			SDL_GetError());
		exit(EXIT_FAILURE);
	}
	if (!gladLoadGL((GLADloadfunc)(SDL_GL_GetProcAddress))) {
		fprintf(stderr, "Failed to load OpenGL functions.\n");
		exit(EXIT_FAILURE);
	}
	glEnable(GL_DEPTH_TEST);
	// TODO: We need to call glViewport() every time window resizes.
	glViewport(0, 0, win_width, win_height);
}

void gfx_shutdown(void)
{
	// REVIEW: Is using memset() an optimization and how likely will it
	//	   cause bugs if NULL is not 0x0. Profile versus simple loop.
	memset(display_modes, 0, sizeof(display_modes));
	SDL_GL_DeleteContext(gl_ctx);
	SDL_DestroyWindow(window);
}

void gfx_clear_framebuffer(void)
{
	glad_glClearColor(0.0f, 0.0f, 0.0f, 255.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gfx_present_framebuffer(void)
{
	SDL_GL_SwapWindow(window);
}
