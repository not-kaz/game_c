#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "err_msg.h"
#include "game.h"
#include "graphics.h"
#include "input.h"
#include "result_code.h"
#include "shader.h"
#include "test_scene.h"

#define SDL_INIT_FLAG (SDL_INIT_VIDEO | SDL_INIT_AUDIO)
#define IMG_INIT_FLAG (IMG_INIT_JPG | IMG_INIT_PNG)

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
} game_ctx;

int game_init(void)
{
	if (game_ctx.state != GAME_STATE_UNDEFINED) {
		err_msg_set("Game context already initialized.");
		goto handle_err;
	}
	game_ctx.state = GAME_STATE_GAMEPLAY;
	if (SDL_Init(SDL_INIT_FLAG) != 0) {
		err_msg_set(SDL_GetError());
		goto handle_err;
	}
	if (IMG_Init(IMG_INIT_FLAG) != IMG_INIT_FLAG) {
		err_msg_set(IMG_GetError());
		goto handle_err;
	}
	if (graphics_init()) {
		//err_msg_set("Graphics subsystem failed to initialize.");
		goto handle_err;
	}
	impl_test_scene();
	game_ctx.is_running = true;
	return RESULT_CODE_SUCCESS;
handle_err:
	fprintf(stderr, "game_init() has failed: %s\n", err_msg_get());
	return RESULT_CODE_UNSPECIFIED_ERROR;
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
	run_test_scene();
	/*graphics_clear_framebuffer();
	graphics_present_framebuffer();*/
}

bool game_is_running(void)
{
	return game_ctx.is_running;
}
