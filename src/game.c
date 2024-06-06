#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "common.h"
#include "game.h"
#include "graphics.h"
#include "input.h"
#include "shader.h"

#define SDL_INIT_FLAG (SDL_INIT_VIDEO | SDL_INIT_AUDIO)
#define IMG_INIT_FLAG (IMG_INIT_PNG)

enum game_state {
	GAME_STATE_UNDEFINED = 0,
	GAME_STATE_MAIN_MENU,
	GAME_STATE_GAMEPLAY,
	GAME_STATE_PAUSE_MENU,
	GAME_STATE_GAME_OVER,
	GAME_STATE_OPTIONS,
	GAME_STATE_NUM
};

static struct {
	enum game_state state;
	bool is_running;
	bool is_inited;
} game_ctx;
static struct shader_program shd;
const char *vsrc = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos, 1.0);\n"
	"}\0";
const char *fsrc = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\n\0";

int game_init(void)
{
	const char *err_msg = NULL;

	if (game_ctx.is_inited) {
		err_msg = "Game context already initialized.";
		goto handle_err;
	}
	game_ctx.state = GAME_STATE_GAMEPLAY;
	game_ctx.is_running = true;
	if (SDL_Init(SDL_INIT_FLAG) != 0) {
		err_msg = SDL_GetError();
		goto handle_err;
	}
	if (IMG_Init(IMG_INIT_FLAG) != IMG_INIT_FLAG) {
		err_msg = IMG_GetError();
		goto handle_err;
	}
	if (!graphics_init()) {
		err_msg = "Graphics subsystem failed to initialize.";
		goto handle_err;
	}
	shader_program_init(&shd, "test", vsrc, fsrc);
	game_ctx.is_inited = true;
	return RETURN_CODE_SUCCESS;
handle_err:
	fprintf(stderr, "Game context initialization failed: %s\n", err_msg);
	return RETURN_CODE_FAILURE;
}

void game_finish(void)
{
	graphics_finish();
	IMG_Quit();
	SDL_Quit();
	memset(&game_ctx, 0, sizeof(game_ctx));
}

void game_update(void)
{
	input_poll();
	if (input_is_key_down(SDL_SCANCODE_ESCAPE)) {
		game_ctx.is_running = false;
	}
	graphics_clear_framebuffer();
	graphics_present_framebuffer();
}

bool game_is_running(void)
{
	return game_ctx.is_running;
}
