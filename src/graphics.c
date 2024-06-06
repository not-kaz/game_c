#include <glad/gl.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include "common.h"
#include "config.h"
#include "graphics.h"

#define MAX_NUM_DISPLAY_MODES 512

static struct {
	SDL_DisplayMode modes[MAX_NUM_DISPLAY_MODES];
	int num_modes;
	int curr_mode_idx;
	int idx;
} display;
static struct {
	SDL_Window *window;
	SDL_GLContext *gl_ctx;
	bool is_initialized;
} sdl_ctx;
static struct config graphics_config;
const char *graphics_config_filename = "graphics.cfg";

static void fetch_display_modes(void)
{
	display.num_modes = SDL_GetNumDisplayModes(display.idx);
	if (display.num_modes < 1 ) {
		fprintf(stderr, "Failed to fetch available display modes.\n");
		exit(EXIT_FAILURE);
	}
	if (display.num_modes > MAX_NUM_DISPLAY_MODES) {
		fprintf(stderr, "Number of found display modes found (%d) "
			"exceeds our limit (%d). Not all resolutions will be "
			"available.\n", display.num_modes,
			MAX_NUM_DISPLAY_MODES);
		display.num_modes = MAX_NUM_DISPLAY_MODES;
	}
	for (int i = 0; i < display.num_modes; i++) {
		if (SDL_GetDisplayMode(display.idx, i, &display.modes[i]) < 0) {
			fprintf(stderr, "Failed to retrieve display info.\n");
			exit(EXIT_FAILURE);
		}
	}
}

int graphics_init(void)
{
	int ww = 0;
	int wh = 0;
	unsigned int wf = SDL_WINDOW_OPENGL;

	if (sdl_ctx.is_initialized) {
		fprintf(stderr, "Attempted to initialize SDL context again.\n");
		return RETURN_CODE_FAILURE;
	}
	memset(&display, 0, sizeof(display));
	fetch_display_modes();
	config_add_entry(&graphics_config, "window_width",
		display.modes[display.curr_mode_idx].w);
	config_add_entry(&graphics_config, "window_height",
		display.modes[display.curr_mode_idx].h);
	config_add_entry(&graphics_config, "fullscreen", 1);
	config_add_entry(&graphics_config, "vsync", 1);
	if (!config_read_from_file(&graphics_config, "graphics.cfg")) {
		fprintf(stderr, "Failed to load graphics config file (%s).\n",
			graphics_config_filename);
	}
	ww = config_get_entry_val(&graphics_config, "window_width");
	wh = config_get_entry_val(&graphics_config, "window_height");
	if (config_get_entry_val(&graphics_config, "fullscreen")) {
		wf |= SDL_WINDOW_FULLSCREEN;
	}
	sdl_ctx.window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, ww, wh, wf);
	if (!sdl_ctx.window) {
		fprintf(stderr, "Failed to create window. %s\n",
			SDL_GetError());
		return RETURN_CODE_FAILURE;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
		SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	sdl_ctx.gl_ctx = SDL_GL_CreateContext(sdl_ctx.window);
	if (!sdl_ctx.gl_ctx) {
		fprintf(stderr, "Failed to create OpenGL context. %s\n",
			SDL_GetError());
		return RETURN_CODE_FAILURE;
	}
	if (!gladLoadGL((GLADloadfunc)(SDL_GL_GetProcAddress))) {
		fprintf(stderr, "Failed to load OpenGL functions.\n");
		return RETURN_CODE_FAILURE;
	}
	sdl_ctx.is_initialized = true;
	glEnable(GL_DEPTH_TEST);
	// TODO: We need to call glViewport() every time window resizes.
	glViewport(0, 0, ww, wh);
	//config_store_to_file(&graphics_config, "graphics.cfg");
	return RETURN_CODE_SUCCESS;
}

void graphics_finish(void)
{
	memset(&graphics_config, 0, sizeof(struct config));
	memset(&display, 0, sizeof(display));
	SDL_GL_DeleteContext(sdl_ctx.gl_ctx);
	SDL_DestroyWindow(sdl_ctx.window);
	sdl_ctx.gl_ctx = NULL;
	sdl_ctx.window = NULL;
}

void graphics_clear_framebuffer(void)
{
	glad_glClearColor(0.0f, 0.0f, 0.0f, 255.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void graphics_present_framebuffer(void)
{
	SDL_GL_SwapWindow(sdl_ctx.window);
}
