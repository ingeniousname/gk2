#include "SDL.h"
#include "../triangulation.h"
#include "imgui.h"
#include <chrono>

class Window {
	int WIDTH;
	int HEIGHT;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* screenTexture = NULL;
	Triangulation t;
	std::chrono::high_resolution_clock::time_point now, last;
	float R, T, time;
	bool paused, textureFromFile, fileError;

	void renderGUI(int*);
	void updateFrame(int);
public:
	Window(int, int);
	~Window();
	void run();
};