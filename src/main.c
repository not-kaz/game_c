#include <SDL.h>
#include "common.h"
#include "game.h"

int main(const int argc, char *argv[])
{
	UNUSED(argc);
	UNUSED(argv);
	if (game_init()) {
		return 1;
	}
	while (game_is_running()) {
		game_update();
	}
	game_finish();
	return 0;
}