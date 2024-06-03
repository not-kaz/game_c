#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "common.h"
#include "game.h"
#include "graphics.h"
#include "input.h"

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
} game_ctx;

static int init_sdl(void)
{
	if (SDL_Init(SDL_INIT_FLAG) != 0) {
		fprintf(stderr, "Failed to initialize SDL. %s\n",
			SDL_GetError());
		return RETURN_CODE_FAILURE;
	}
	if (IMG_Init(IMG_INIT_FLAG) != IMG_INIT_FLAG) {
		fprintf(stderr, "Failed to initialize SDL image library. %s\n",
			IMG_GetError());
		return RETURN_CODE_FAILURE;
	}
	return RETURN_CODE_SUCCESS;
}

int game_init(void)
{
	game_ctx.state = GAME_STATE_GAMEPLAY;
	game_ctx.is_running = true;
	if (!init_sdl()) {
		return RETURN_CODE_FAILURE;
	}
	if (!graphics_init()) {
		return RETURN_CODE_FAILURE;
	}
	return RETURN_CODE_SUCCESS;
}

void game_finish(void)
{
	graphics_finish();
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
