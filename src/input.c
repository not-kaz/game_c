#include <SDL2/SDL.h>
#include <stdbool.h>
#include "common.h"
#include "input.h"

void input_poll(void)
{
	SDL_Event event;

	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT) {
		exit(EXIT_SUCCESS);
	}
}

bool input_is_key_down(SDL_Scancode scancode)
{
	const uint8_t *keyboard_state;

	keyboard_state = SDL_GetKeyboardState(NULL);
	if (scancode > SDL_NUM_SCANCODES || !keyboard_state) {
		return false;
	}
	return (bool) keyboard_state[scancode];
}

bool input_is_button_down(uint32_t button)
{
	if (button > SDL_BUTTON_X2) {
		return false;
	}
	return (bool) (SDL_GetMouseState(NULL, NULL) & button);
}

struct vec2i input_mouse_position(void)
{
	struct vec2i pos;

	SDL_GetMouseState(&pos.x, &pos.y);
	return pos;
}
