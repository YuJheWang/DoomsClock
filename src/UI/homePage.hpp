#pragma once

#include "UIBase.hpp"

class HomePage : public UIBase
{
public:

    HomePage() = default;

    virtual void render();

private:

    void gamer4room();

};

void HomePage::render()
{
    ImGui::Begin("GameMode");
    if (ImGui::Button("4 Gamer Room")) gamer4room();
    ImGui::End();
}

void HomePage::gamer4room()
{
    
}
