#include "window.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "../triangle_filler.h"
#include "../reflection_calculator.h"
#include <iostream>

void Window::renderGUI(int* value)
{
    ImGuiIO& io{ ImGui::GetIO() };
    static int selected = 0;

    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Options");
    ImGui::Text("Triangle count");
    if (ImGui::SliderInt("X axis", &t.divisions_X, 1, 100))
    {
        t.updateTriangulation(WIDTH, HEIGHT);
    }
    if (ImGui::SliderInt("Y axis", &t.divisions_Y, 1, 100))
    {
        t.updateTriangulation(WIDTH, HEIGHT);
    }
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("Coefficients");
    if (ImGui::SliderFloat("kd", &ReflectionCalculator::get()->kd, 0, 1))
    {
        ReflectionCalculator::get()->ks = 1 - ReflectionCalculator::get()->kd;
    }
    if (ImGui::SliderFloat("ks", &ReflectionCalculator::get()->ks, 0, 1))
    {
        ReflectionCalculator::get()->kd = 1 - ReflectionCalculator::get()->ks;
    }
    ImGui::SliderInt("m", &ReflectionCalculator::get()->m, 1, 100);
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Checkbox("Load texture from file", &textureFromFile);
    if(textureFromFile)
    {

    }
    else
    {
        ImGui::ColorEdit3("Object color", &ReflectionCalculator::get()->objectColor.x);
    }
    ImGui::ColorEdit3("Light color", &ReflectionCalculator::get()->lightColor.x);
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("Animation");
    ImGui::SliderFloat("Z", &ReflectionCalculator::get()->lightSource.z, 1, 1000);
    ImGui::SliderFloat("R", &R, 0, 1000);
    ImGui::SliderFloat("T", &T, 0.1f, 20.f);
    if (ImGui::Button("pause/unpause", ImVec2(100, 25)))
    {
        paused = !paused;
    }
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
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
    
    now = std::chrono::high_resolution_clock::now();
    if (paused)
    {
        time += (now - last).count() / 1e9;
        ReflectionCalculator::get()->lightSource.x = WIDTH / 2 + R * std::sin(2 * 3.1415f * time / T);
        ReflectionCalculator::get()->lightSource.y = HEIGHT / 2 + R * std::cos(2 * 3.1415f * time / T);
    }
    last = now;



    SDL_LockTexture(screenTexture, NULL, &frame, &pitch);

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


    SDL_UnlockTexture(screenTexture);
}

Window::Window(int width, int height) : WIDTH(width), HEIGHT(height), R(0.f), T(5.f), time(0.f), textureFromFile(false),
    paused(false), now(std::chrono::high_resolution_clock::now()), last(std::chrono::high_resolution_clock::now())
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

    screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    

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
    SDL_DestroyTexture(screenTexture);
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
            else if (e.type == SDL_WINDOWEVENT)
            {
                if (e.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    SDL_DestroyTexture(screenTexture);
                    WIDTH = e.window.data1;
                    HEIGHT = e.window.data2;
                    screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
                }
            }
        }


        SDL_RenderClear(renderer);
        updateFrame(tick);
        SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
        renderGUI(&value);
        SDL_RenderPresent(renderer);
        tick += 5;
    }
}
