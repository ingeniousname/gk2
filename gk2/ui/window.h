#include "SDL.h"
#include "../triangulation.h"
#include "imgui.h"

class Window {
	int WIDTH;
	int HEIGHT;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture = NULL;
	Triangulation t;

	void renderGUI(int*);
	void updateFrame(int);
public:
	Window(int, int);
	~Window();
	void run();
};