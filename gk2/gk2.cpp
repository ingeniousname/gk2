#include <iostream>
#include <SDL.h>
#undef main

int main()
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "hello world", "udalo sie", NULL);
	return 0;
}

