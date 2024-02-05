#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

#include "homePage.hpp"

class UISystem
{
public:

    UISystem(SDL_Window* window, SDL_Renderer* renderer);

    void loop(SDL_Event& event);

    ~UISystem();

private:

    ImGuiIO* io;

    UIBase* currentUI = nullptr;

};

UISystem::UISystem(SDL_Window* window, SDL_Renderer* renderer)
{
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    //io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    currentUI = new HomePage;

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
}

void UISystem::loop(SDL_Event& event)
{
    ImGui_ImplSDL2_ProcessEvent(&event);

    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::StyleColorsLight();

    currentUI->render();

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}

UISystem::~UISystem()
{
    delete currentUI;

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}
