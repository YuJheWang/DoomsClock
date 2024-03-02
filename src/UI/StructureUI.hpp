#pragma once

#include <cstdlib>
#include <ctime>
#include <vector>

#include "UIBase.hpp"
#include "../GameLogic/Structures.hpp"
#include "../Render/StructureRender.hpp"

const char* structureCardImages[All] = {
    "../asset/CardImages/Factory.png",
    "../asset/CardImages/FirePowerPlant.png",
    "../asset/CardImages/Nuclear.png",
    "../asset/CardImages/fireDepartment.png",
    "../asset/CardImages/windTurbine.png",
    "../asset/CardImages/ResidentialArea.png",
    "../asset/CardImages/park.png",
    "../asset/CardImages/coal.png",
    "../asset/CardImages/farm.png",
    "../asset/CardImages/storage.png",
};

#define UI_CONTAIN 4

class StructureUI : public UIBase
{
public:

    StructureUI(SDL_Window* window, SDL_GLContext context);

    ~StructureUI() { glDeleteTextures(All, textureIDs); }

    virtual void render(bool* flag);

    bool isWindowHovered() { return hovered; }

    Uint8 &getCurrentBindStructure() { return currentBindStructure; }

private:

    void loadTexture(Uint8 structure);

    void shiftCurrentStructures(int step) 
    {
        for (auto& cs : currentStructures)
            cs = (cs + step + All) % All;
    }

    GLuint textureIDs[All + 1];

    Uint8 currentStructures[UI_CONTAIN] = { 0, 1, 2, 3 };

    std::vector<Uint8> structureYouHave;

    bool hovered = false;

    Uint8 currentBindStructure = All;

    void addStructure() 
    {
        srand(time(NULL));
        Uint8 structure = rand() % All;
        structureYouHave.push_back(structure);
        for (auto s : structureYouHave) std::cout << int(s) << " ";
        std::cout << std::endl;
    }

};

StructureUI::StructureUI(SDL_Window* window, SDL_GLContext context) : UIBase(window, context)
{
    glGenTextures(All + 1, textureIDs);
    for (int i = 0; i <= All; i++)
        if (i != Factory || i != Farm || i != Storage) loadTexture(i);
}

void StructureUI::render(bool* flag)
{
    if (*flag)
    {
        ImGui::Begin("Structures");
        hovered = ImGui::IsWindowHovered();
        if (ImGui::ArrowButton("#leftShiftStructure", ImGuiDir_Left))
            shiftCurrentStructures(-1);
        ImGui::SameLine();
        ImGui::AlignTextToFramePadding();

        for (int i = 0; i < UI_CONTAIN; i++)
        {
            if (ImGui::ImageButton(
                (ImTextureID) textureIDs[currentStructures[i]], 
                ImVec2(160.0f, 200.0f)
            )) currentBindStructure = currentStructures[i];
            ImGui::SameLine();
            ImGui::AlignTextToFramePadding();
        }

        if (ImGui::ArrowButton("#rightShiftStructure", ImGuiDir_Right))
            shiftCurrentStructures(1);

        ImGui::SameLine();

        if (ImGui::ImageButton((ImTextureID) textureIDs[All], ImVec2(160.0f, 200.0f))) 
            addStructure();

        ImGui::End();
    }
}

void StructureUI::loadTexture(Uint8 structure)
{
    glBindTexture(GL_TEXTURE_2D, textureIDs[structure]);
    int width, height, channel;
    unsigned char* data;
    if (structure == All) 
        data = stbi_load("../asset/CardImages/addButton.png", &width, &height, &channel, 0);
    else 
        data = stbi_load(structureCardImages[structure], &width, &height, &channel, 0);
    if (data == nullptr) 
        std::cerr << "Fail to load structure image, structure ID: " << structure << std::endl;
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
}
