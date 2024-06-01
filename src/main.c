#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "config.h"
#include "common.h"
#include "graphics.h"
#include "input.h"

#define SDL_INIT_FLAG (SDL_INIT_VIDEO | SDL_INIT_AUDIO)
#define IMG_INIT_FLAG (IMG_INIT_PNG)

static struct {
	bool is_running;
} game_ctx;

int main(int argc, char *argv[])
{
	struct config config;
	struct config_entry *entry = NULL;

	memset(&config, 0, sizeof(struct config));
	config_add_entry(&config, "test", 123);
	config_add_entry(&config, "test", 444);
	printf("%d\n", config_get_entry_val(&config, "test"));
	UNUSED(argc);
	UNUSED(argv);
	if (SDL_Init(SDL_INIT_FLAG) != 0) {
		SDL_Log("Failed to initialize SDL.\n");
		return 1;
	}
	if (IMG_Init(IMG_INIT_FLAG) != IMG_INIT_FLAG) {
		SDL_Log("Failed to initialize SDL image library.\n");
		return 1;
	}
	graphics_setup();
	game_ctx.is_running = true;
	while (game_ctx.is_running) {
		input_poll();
		if (input_is_key_down(SDL_SCANCODE_ESCAPE)) {
			game_ctx.is_running = false;
		}
		graphics_clear_framebuffer();
		graphics_present_framebuffer();
	}
	graphics_shutdown();
	IMG_Quit();
	SDL_Quit();
	return 0;
}
