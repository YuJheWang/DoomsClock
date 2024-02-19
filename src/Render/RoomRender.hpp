#pragma once 

#include "../GameLogic/Room.hpp"

#include "StructureRender.hpp"

struct Vertex
{
    float x, y, z;
    float r, g, b, a;
    float u, v;
};

const Vertex planeVertices[] = {
    { -10.0f, 0.0f, -10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { -10.0f, 0.0f, 37.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
    { 37.0f, 0.0f, 37.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f },
    { 37.0f, 0.0f, -10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f },
};

const Vec3 gridVertices[] = {
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f },
    { 1.0f, 0.0f, 1.0f },
    { 1.0f, 0.0f, 0.0f },
    { 0.05f, 0.05f, 0.05f },
    { 0.05f, 0.05f, 0.95f },
    { 0.95f, 0.05f, 0.95f },
    { 0.95f, 0.05f, 0.05f }
};

const float colorRatios[] = {
    1, 1, 1, 1, 0.707f, 0.707f, 0.707f, 0.707f
};

const unsigned int gridIndices[] = {
    4, 5, 6,
    4, 6, 7,
    0, 4, 5,
    0, 1, 5,
    1, 5, 6,
    1, 2, 6,
    2, 6, 7,
    2, 3, 7,
    0, 3, 7,
    0, 4, 7
};

class RoomRender
{
public:

    RoomRender();

    ~RoomRender() { delete _room; }

    void bind(Room* room);

    void render(float aspectRatio);

    glm::vec3 lookPoint = glm::vec3(1, 0, 1);

private:

    StructureRender *sr[All];

    Room* _room = nullptr;

    Camera camera;

    Shader planeShader, gridsShader;

    GLuint planeVBO, planeVAO, gridVBO, gridEBO, gridVAO;

    GLuint texture;

    void backgroundInit();

    void backgroundRender(float ar);

    void gridInit();

    void gridRender(const glm::ivec2& pos);

};

RoomRender::RoomRender() {}

void RoomRender::bind(Room* room)
{
    _room = room;
    for (int i = 0; i < All; i++)
    {
        sr[i] = new StructureRender;
        sr[i]->bind(i);
    }

    //sr[NuclearPowerPlant] = new StructureRender;
    //sr[NuclearPowerPlant]->bind(NuclearPowerPlant);

    std::cout << "Sucessfully bind all structures!" << std::endl;

    //backgroundInit();
    gridInit();
}

void RoomRender::render(float aspectRatio)
{
    camera.moveTo(glm::vec3(-1, sqrt(2), -1) * 5.0f + lookPoint);
    camera.lookAt(lookPoint);

    auto players = _room->getPlayers();
    for (auto player : players)
    {
        auto structureContained = player->getContainedStructure();
        for (auto structure : structureContained)
        {
            sr[structure.second]->render(
                camera.getViewMatrix(), 
                structure.first,
                glm::ortho(-8.0f, 8.0f, -4.5f, 4.5f, 0.01f, 100.0f)
            );
        }
    }
    //backgroundRender(aspectRatio);

    for (int i = 0 ; i < 27; i++)
        for (int j = 0; j < 27; j++)
            gridRender(glm::ivec2(i, j));
}

void RoomRender::backgroundInit()
{
    planeShader.loadFromFile("../asset/shaders/plane.vs.glsl", "../asset/shaders/plane.fs.glsl");

    glGenVertexArrays(1, &planeVAO);
    glBindVertexArray(planeVAO);

    glGenBuffers(1, &planeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 7));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    int width, height, channel;
    unsigned char* imageData = stbi_load("../asset/background.PNG", &width, &height, &channel, 0);
    if (imageData == NULL) std::cerr << "Fail to load background image!" << std::endl;
    else 
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(imageData);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void RoomRender::backgroundRender(float ar)
{
    planeShader.setUniform("projMat", glm::ortho(-8.0f, 8.0f, -4.5f, 4.5f, 0.01f, 100.0f));
    planeShader.setUniform("viewMat", camera.getViewMatrix());
    planeShader.setUniform("texture", 0);

    planeShader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);

    planeShader.unuse();
}

void RoomRender::gridInit()
{
    gridsShader.loadFromFile(
        "../asset/shaders/grid.vs.glsl", 
        "../asset/shaders/grid.fs.glsl", 
        "../asset/shaders/grid.gs.glsl"
    );

    glGenVertexArrays(1, &gridVAO);
    glBindVertexArray(gridVAO);

    glGenBuffers(1, &gridVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gridVertices) + sizeof(colorRatios), gridVertices, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(gridVertices), sizeof(colorRatios), colorRatios);

    glGenBuffers(1, &gridEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gridIndices), gridIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)(sizeof(gridVertices)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void RoomRender::gridRender(const glm::ivec2& pos)
{
    gridsShader.setUniform("viewMat", camera.getViewMatrix());
    gridsShader.setUniform("projMat", glm::ortho(-8.0f, 8.0f, -4.5f, 4.5f, 0.01f, 100.0f));
    gridsShader.setUniform("translate", glm::translate(glm::vec3(pos.x, 0.0f, pos.y)));
    gridsShader.setUniform("position", pos);

    gridsShader.use();

    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridEBO);
    glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    gridsShader.unuse();

}
