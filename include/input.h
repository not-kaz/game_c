#ifndef INPUT_H
#define INPUT_H
#include <SDL_scancode.h>

void input_poll(void);
bool input_is_key_down(SDL_Scancode scancode);
bool input_is_button_down(uint32_t button);
void input_mouse_pos(int32_t *x_out, int32_t *y_out);

#endif
