#ifndef GAME_H
#define GAME_H
#include <stdbool.h>

int game_init(void);
void game_finish(void);
void game_update(void);
bool game_is_running(void);

#endif
