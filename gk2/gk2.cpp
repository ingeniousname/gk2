#include <iostream>
#include <SDL.h>
#include "ui/window.h"
#undef main

int main()
{
	Window window(1024, 768);
	window.run();
}

