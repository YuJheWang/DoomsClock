#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "UIBase.hpp"

class UISystem
{
public:

    UISystem(SDL_Window* window, SDL_GLContext context);

    void bind(UIBase* ui) { currentUI = ui; }

    void loop(SDL_Event& event);

    ~UISystem();

private:

    ImGuiIO* io;

    UIBase* currentUI = nullptr;

};

UISystem::UISystem(SDL_Window* window, SDL_GLContext context)
{
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    //io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init();
}

void UISystem::loop(SDL_Event& event)
{
    ImGui_ImplSDL2_ProcessEvent(&event);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::StyleColorsLight();

    bool isRender = true;
    currentUI->render(&isRender);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

UISystem::~UISystem()
{
    delete currentUI;

    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}
