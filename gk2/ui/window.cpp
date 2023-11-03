#include "window.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "../triangle_filler.h"


void Window::renderGUI(int* value)
{
    ImGuiIO& io{ ImGui::GetIO() };
    static int selected = 0;

    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();


    ImGui::SetNextWindowSize(ImVec2(400, 300));
    ImGui::Begin("Options");
    if (ImGui::SliderInt("Divisions in X axis", &t.divisions_X, 1, 100))
    {
        t.updateTriangulation(WIDTH, HEIGHT);
    }
    if (ImGui::SliderInt("Divisions in y axis", &t.divisions_Y, 1, 100))
    {
        t.updateTriangulation(WIDTH, HEIGHT);
    }
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
    ImGui::Render();

    SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}

void Window::updateFrame(int offset)
{
    int pitch;
    void* frame;
    SDL_LockTexture(texture, NULL, &frame, &pitch);

    //for (int x = 0; x < WIDTH; x++)
    //{
    //    for (int y = 0; y < HEIGHT; y++) 
    //    {
    //        Uint8* base = ((Uint8*)frame) + (4 * (x * HEIGHT + y));
    //        base[0] = (float)x / WIDTH * 255 + offset;
    //        base[1] = (float)x / WIDTH * 255 + offset;
    //        base[2] = (float)x / WIDTH * 255 + offset;
    //        base[3] = 255;
    //    }
    //}
    
    //std::vector<PointData> t;
    //t.push_back(PointData(100, 100, 0.0));
    //t.push_back(PointData(200, 200, 0.0));
    //t.push_back(PointData(100, 300, 0.0));
    //t.push_back(PointData(200, 400, 0.0));
    //t.push_back(PointData(400, 300, 0.0));
    //t.push_back(PointData(300, 100, 0.0));
    //TriangleFiller::fillPolygon((Uint8*)frame, t, WIDTH, HEIGHT);

    TriangleFiller::fillTriangles((Uint8*)frame, t.getData(), WIDTH, HEIGHT);


    SDL_UnlockTexture(texture);
}

Window::Window(int width, int height) : WIDTH(width), HEIGHT(height)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) < 0)
    {
        throw("failed to initialize window");
        return;
    }

#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow("gk2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, window_flags);
    if (window == NULL)
    {
        throw("failed to create window");
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        throw("failed to create renderer");
        return;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io{ ImGui::GetIO() }; (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    t.updateTriangulation(WIDTH, HEIGHT);
}

Window::~Window()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);
    SDL_Quit();
}

void Window::run()
{
    bool quit = false;
    int value = 0;
    int tick = 0;
    SDL_Event e;
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            ImGui_ImplSDL2_ProcessEvent(&e);
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }


        SDL_RenderClear(renderer);
        updateFrame(tick);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        renderGUI(&value);
        SDL_RenderPresent(renderer);
        tick += 5;
    }
}
