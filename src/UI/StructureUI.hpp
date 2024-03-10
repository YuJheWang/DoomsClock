#pragma once

#include <cstdlib>
#include <ctime>
#include <vector>
#include <exception>

#include "UIBase.hpp"
#include "../GameLogic/Structures.hpp"
#include "../Render/StructureRender.hpp"
#include "../GameLogic/Player.hpp"

#define UI_CONTAIN 4

const char* measureImageFile[] = {
    "../asset/CardImages/DestroyStructure.png",
    "../asset/CardImages/ExtinguishFire.png",
    "../asset/CardImages/CutTree.png",
    "../asset/CardImages/PlantTree.png",
    "../asset/CardImages/addButtonM.png"
};

class StructureUI : public UIBase
{
public:

    StructureUI(SDL_Window* window, SDL_GLContext context);

    ~StructureUI() { glDeleteTextures(All + 1, textureIDs); glDeleteTextures(AllM + 1, measureIDs); }

    void bindPlayer(Player* p) { player = p; }

    virtual void render(bool* flag);

    bool isWindowHovered() { return hovered; }

    Uint8 &getCurrentBindStructure() { return currentBindStructure; }

    Uint8 &getCurrentBindMeasure() { return currentBindMeasure; }

private:

    void loadTexture(Uint8 structure);

    void loadMeasureTexture(Uint8 measure);

    void shiftCurrentStructures(int step) 
    {
        int max = player->getStructureCardTypeCount();
        currentStructureCardCursor = (currentStructureCardCursor + step + max) % max;
    }

    void shiftCurrentMeasures(int step)
    {
        int max = player->getMeasureCardTypeCount();
        currentMeasureCardCursor = (currentMeasureCardCursor + step + max) % max;
    }

    Player* player;

    GLuint textureIDs[All + 1];

    GLuint measureIDs[AllM + 1];

    Uint8 currentStructures[UI_CONTAIN] = { 0, 1, 2, 3 };

    Uint8 currentMeasures[UI_CONTAIN] = { 0, 1, 2, 3 };

    Uint8 currentStructureCardCursor = 0, currentMeasureCardCursor = 0;

    std::vector<Uint8> structureYouHave;

    ImVec2 shiftButtonSize, cardSize = ImVec2(160.0f, 200.0f);

    long long lastNewStructureCardTime = -5000, lastNewMeasureCardTime = -5000;

    bool hovered = false;

    Uint8 currentBindStructure = All, currentBindMeasure = AllM;

};

StructureUI::StructureUI(SDL_Window* window, SDL_GLContext context) : UIBase(window, context)
{
    glGenTextures(All + 1, textureIDs);
    glGenTextures(AllM + 1, measureIDs);
    for (int i = 0; i <= All; i++)
        if (i != Tree)
        loadTexture(i);
    for (int i = 0; i <= AllM; i++)
        loadMeasureTexture(i);
}

void StructureUI::render(bool* flag)
{
    if (*flag)
    {
        if (currentBindStructure != All) ImGui::SetNextWindowCollapsed(true);

        ImGui::SetNextWindowBgAlpha(0.5f);

        ImGui::Begin("Your cards");
        ImGui::Text("Structure Cards");
        hovered = ImGui::IsWindowHovered();

        //Structure UI
        if (ImGui::ArrowButton("#leftShiftStructure", ImGuiDir_Left))
            if (player->getStructureCardTypeCount() > UI_CONTAIN)
                shiftCurrentStructures(-1);
        shiftButtonSize = ImGui::GetItemRectSize();
        ImGui::SameLine();
        ImGui::AlignTextToFramePadding();

        int max = player->getStructureCardTypeCount();
        auto structureIndices = player->getStructureCardIndices();
        for (int i = 0; i < 4; i++)
        {
            if (max == 0) ImGui::Dummy(cardSize);
            else if (max >= UI_CONTAIN)
            {
                int id = (currentStructureCardCursor + i) % max;
                if (id >= player->getStructureCardTypeCount())
                    ImGui::Dummy(cardSize);
                else
                {
                    int id = (currentStructureCardCursor + i) % max;
                    if (ImGui::ImageButton(
                        (ImTextureID) textureIDs[structureIndices[id]],
                        ImVec2(160.0f, 200.0f)
                    )) currentBindStructure = structureIndices[id];
                }
            }
            else if (i >= player->getStructureCardTypeCount()) ImGui::Dummy(cardSize);
            else 
            {
                if (ImGui::ImageButton(
                    (ImTextureID) textureIDs[structureIndices[i]],
                    ImVec2(160.0f, 200.0f)
                )) currentBindStructure = structureIndices[i];
            }
            ImGui::SameLine();
            ImGui::AlignTextToFramePadding();
        }

        if (ImGui::ArrowButton("#rightShiftStructure", ImGuiDir_Right))
        {
            if (player->getStructureCardTypeCount() > UI_CONTAIN)
                shiftCurrentStructures(1);
        }

        ImGui::SameLine();

        if (ImGui::ImageButton((ImTextureID) textureIDs[All], ImVec2(120.0f, 150.0f)))
        {
            auto time = SDL_GetTicks64();
            if (time - lastNewStructureCardTime > 5000)
            {
                player->addStructureCard();
                lastNewStructureCardTime = time;
            }
            player->viewDecks();
        }

        //Measure UI
        if (ImGui::ArrowButton("#leftShiftMeasure", ImGuiDir_Left))
            if (player->getMeasureCardTypeCount() > UI_CONTAIN)
                shiftCurrentMeasures(-1);
        shiftButtonSize = ImGui::GetItemRectSize();
        ImGui::SameLine();
        ImGui::AlignTextToFramePadding();

        max = player->getMeasureCardTypeCount();
        auto measureIndices = player->getMeasureCardIndices();
        for (int i = 0; i < UI_CONTAIN; i++)
        {
            if (max == 0) ImGui::Dummy(cardSize);
            else if (max >= UI_CONTAIN)
            {
                int id = (currentMeasureCardCursor + i) % max;
                if (id >= player->getMeasureCardTypeCount())
                    ImGui::Dummy(cardSize);
                else
                {
                    int id = (currentMeasureCardCursor + i) % max;
                    if (ImGui::ImageButton(
                        (ImTextureID) measureIDs[measureIndices[id]],
                        ImVec2(160.0f, 200.0f)
                    )) currentBindMeasure = measureIndices[id];
                }
            }
            else if (i >= player->getMeasureCardTypeCount()) ImGui::Dummy(cardSize);
            else 
            {
                if (ImGui::ImageButton(
                    (ImTextureID) measureIDs[measureIndices[i]],
                    ImVec2(160.0f, 200.0f)
                )) {
                    currentBindMeasure = measureIndices[i];
                }
            }
            ImGui::SameLine();
            ImGui::AlignTextToFramePadding();
        }

        if (ImGui::ArrowButton("#rightShiftMeasure", ImGuiDir_Right))
            if (player->getMeasureCardTypeCount() > UI_CONTAIN)
                shiftCurrentMeasures(1);

        ImGui::SameLine();

        if (ImGui::ImageButton((ImTextureID) measureIDs[AllM], ImVec2(120.0f, 150.0f)))
        {
            auto time = SDL_GetTicks64();
            if (time - lastNewMeasureCardTime > 5000)
            {
                player->addMeasureCard();
                lastNewMeasureCardTime = time;
            }
        }

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
    else if (structure != Tree)
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

void StructureUI::loadMeasureTexture(Uint8 measure)
{
    glBindTexture(GL_TEXTURE_2D, measureIDs[measure]);
    int width, height, channel;
    unsigned char* data;
    if (measure == AllM) 
        data = stbi_load("../asset/CardImages/addButtonM.png", &width, &height, &channel, 0);
    else 
        data = stbi_load(measureCardImage[measure], &width, &height, &channel, 0);
    if (data == nullptr) 
        std::cerr << "Fail to load measure image, structure ID: " << measure << std::endl;
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
}
