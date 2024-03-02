#pragma once

#include <string>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

class UIBase
{
public:

    UIBase(SDL_Window* window, SDL_GLContext context) : window(window), context(context) {}

    void loadImGui();

    void renderToWindow(bool* flag);

    virtual void render(bool* flag) = 0;

private:

    std::string bottomRowTitle, rightColumnTitle;

    SDL_Window* window;

    SDL_GLContext context;

    void popup();

};


void UIBase::loadImGui()
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init("#version 430 core");
}

void UIBase::renderToWindow(bool* flag)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    render(flag);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGui::EndFrame();
}
