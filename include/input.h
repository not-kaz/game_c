#ifndef INPUT_H
#define INPUT_H
#include <SDL2/SDL_Scancode.h>
#include "common.h"

void input_poll(void);
bool input_is_key_down(SDL_Scancode scancode);
bool input_is_button_down(uint32_t button);
struct vec2i input_mouse_position(void);

#endif
