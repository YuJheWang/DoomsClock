#pragma once

#include <string>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

class UIBase
{
public:

    UIBase() = default;

    virtual void render() = 0;

private:

    std::string bottomRowTitle, rightColumnTitle;

    void popup();

    void renderStructureSelection();

    void renderResourcesViewer();

    void renderHomePage();

    void switchToGamePlaying();

    void switchToHomePage();

};

UIBase::UIBase()
{
    
}

void UIBase::renderHomePage()
{
    bool isOpen;
    ImGui::Begin("Room", nullptr, ImGuiWindowFlags_NoTitleBar);
    ImGui::Button("Create New Room");
    ImGui::Button("Join Room");
    ImGui::End();
}

void UIBase::renderStructureSelection()
{
    ImGui::Begin("Structures");
}
