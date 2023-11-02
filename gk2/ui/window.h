#include "SDL.h"

class Window {
	const int WIDTH;
	const int HEIGHT;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture = NULL;

	Uint8* Frame;
	void renderGUI(int*);
	void updateFrame(int);
public:
	Window(int, int);
	~Window();
	void run();
};