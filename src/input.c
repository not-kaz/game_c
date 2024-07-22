#include <SDL.h>
#include <stdbool.h>
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
	const uint8_t *keyboard_state = NULL;

	keyboard_state = SDL_GetKeyboardState(NULL);
	if (scancode > SDL_NUM_SCANCODES || keyboard_state == NULL) {
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

void input_mouse_pos(int32_t *x_out, int32_t *y_out)
{
	SDL_GetMouseState(x_out, y_out);
}
