#include <glad/gl.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include "config.h"
#include "gfx.h"

#define MAX_NUM_DISPLAY_MODES 512

static struct config gfx_config;
const char *gfx_config_filename = "graphics.cfg";
static struct SDL_Window *window;
static struct SDL_GLContext *gl_ctx;
static struct {
	SDL_DisplayMode modes[MAX_NUM_DISPLAY_MODES];
	int num_modes;
	int curr_mode_idx;
	int idx;
} display;

static void fetch_display_modes(void)
{
	memset(&display, 0, sizeof(display));
	display.num_modes = SDL_GetNumDisplayModes(display.idx);
	if (display.num_modes < 1 ) {
		fprintf(stderr, "Failed to fetch available display modes.\n");
		exit(EXIT_FAILURE);
	}
	fprintf(stderr, "%d\n", display.num_modes);
	if (display.num_modes > MAX_NUM_DISPLAY_MODES) {
		fprintf(stderr, "Number of found display modes found (%d) "
			"exceeds our limit (%d). Not all resolutions will be "
			"available.\n", display.num_modes, MAX_NUM_DISPLAY_MODES);
		display.num_modes = MAX_NUM_DISPLAY_MODES;
	}
	fprintf(stderr, "%d\n", display.num_modes);
	for (int i = 0; i < display.num_modes; i++) {
		if (SDL_GetDisplayMode(display.idx, i, &display.modes[i]) < 0) {
			fprintf(stderr, "Failed to retrieve display info.\n");
			fprintf(stderr, "%d\n", i);
			exit(EXIT_FAILURE);
		}
	}
}

void gfx_setup(void)
{
	int ww;
	int wh;
	unsigned int wf;

	if (window || gl_ctx) {
		return;
	}
	fetch_display_modes();
	config_add_entry(&gfx_config, "window_width",
		display.modes[display.curr_mode_idx].w);
	config_add_entry(&gfx_config, "window_height",
		display.modes[display.curr_mode_idx].h);
	config_add_entry(&gfx_config, "fullscreen", 1);
	config_add_entry(&gfx_config, "vsync", 1);
	if (!config_read_from_file(&gfx_config, "graphics.cfg")) {
		fprintf(stderr, "Failed to load graphics config file (%s).\n",
			gfx_config_filename);
	}
	ww = config_get_entry_val(&gfx_config, "window_width");
	wh = config_get_entry_val(&gfx_config, "window_height");
	wf = SDL_WINDOW_OPENGL;
	if (config_get_entry_val(&gfx_config, "fullscreen")) {
		wf |= SDL_WINDOW_FULLSCREEN;
	}
	window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, ww, wh, wf);
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
	glViewport(0, 0, ww, wh);
	//config_store_to_file(&gfx_config, "graphics.cfg");
}

void gfx_shutdown(void)
{
	// REVIEW: Is using memset() an optimization and how likely will it
	//	   cause bugs if NULL is not 0x0. Profile versus simple loop.
	memset(&display, 0, sizeof(display));
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
