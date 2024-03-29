#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <cstring>

#include <glad/glad.h>
#include <GameLogic/Structures.hpp>
#include <GameLogic/Measure/Measure.hpp>

#include "shader.hpp"
#include "Camera.hpp"

#include <algorithm>

#define IMAGE_FILE_NAME(structure) strcat(strcat("../assets/AIGenerated/bgRemoved/", structure), ".png")

struct Vec3 { float x, y, z; };

const Vec3 VERTICES[] = {
    {1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 0.0f},
    {1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
    {0.0f, 1.0f, 1.0f}
};

float initUVs[] = {
    /*0.5f, 0.5f,*/
    0.5f, -0.25f,
    0.0f, 0.0f,
    0.0f, 0.75f,
    0.5f, 1.0f,
    1.0f, 0.75f,
    1.0f, 0.0f//,
    /*0.5f, 0.0f*/
};

struct ImageData
{
    unsigned char* data;
    int width, height;
    int channel;
};

const char* imageFileName[All] = {
    "../asset/AIGenerated/bgRemoved/factory.png",
    "../asset/AIGenerated/bgRemoved/firePowerPlant.png",//
    "../asset/AIGenerated/bgRemoved/nuclearPlant.png",//
    "../asset/AIGenerated/bgRemoved/fireDepartment.png",//
    "../asset/AIGenerated/bgRemoved/windPowerPlant.png",//
    "../asset/AIGenerated/bgRemoved/residentialArea.png",//
    "../asset/AIGenerated/bgRemoved/park.png",//
    "../asset/AIGenerated/bgRemoved/coal.png",//
    "../asset/AIGenerated/bgRemoved/farm.png",
    "../asset/AIGenerated/bgRemoved/storage.png",
    ""
};

float structureHeight[All] = {
    2.0f,
    3.5f,
    3.0f,
    2.5f,
    2.0f,
    2.0f,
    3.0f,
    2.0f,
    1.0f,
    3.5f
};

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
    ""
};

const char* measureCardImage[AllM] = {
    "../asset/CardImages/DestroyStructure.png",
    "../asset/CardImages/ExtinguishFire.png",
    "../asset/CardImages/CutTree.png",
    "../asset/CardImages/PlantTree.png"
};

class StructureRender
{
public:

    StructureRender();

    ~StructureRender();

    void bind(unsigned int structure);

    void render(const glm::mat4& viewMat, const glm::ivec2& position, const glm::mat4& projMat, float r);

private:

    GLuint vbo, vao;

    GLuint texture;

    Shader shader;

    Structure _structure;

    float height;

    ImageData imageData;

    bool initialized = false;

};

class StructureImageRender
{
public:

    StructureImageRender();

    void render(Uint8 structure, const glm::vec2& mousePosOnScreen, const glm::vec2& screenSize);

private:

    GLuint vbo, vao;

    GLuint textures[All];

    Shader shader;

    ImageData imagesData[All];

    float vertices[16] = {
        0.0f, 0.0f, 0.0f, 0.0f,
        150.0f, 0.0f, 1.0f, 0.0f,
        150.0f, 150.0f, 1.0f, 1.0f,
        0.0f, 150.0f, 0.0f, 1.0f
    };

};

StructureImageRender::StructureImageRender()
{
    shader.loadFromFile("../asset/shaders/image.vs.glsl", "../asset/shaders/image.fs.glsl");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*) (sizeof(float) * 2));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenTextures(All, textures);
    for (int i = 0; i < All; i++)
    {
        if (i == Tree) continue;
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        imagesData[i].data = stbi_load(
            imageFileName[i], 
            &imagesData[i].width, &imagesData[i].height, 
            &imagesData[i].channel, 0
        );
        if (imagesData[i].data == nullptr)
            std::cout << "Fail to load image! ID: " << i << std::endl;
        else 
        {
            glTexImage2D(
                GL_TEXTURE_2D, 
                0, 
                GL_RGBA, 
                imagesData[i].width, imagesData[i].height, 
                0, 
                GL_RGBA, 
                GL_UNSIGNED_BYTE,
                imagesData[i].data
            );
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(imagesData[i].data);
        }

        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void StructureImageRender::render(
    Uint8 structure, 
    const glm::vec2& mousePosOnScreen, 
    const glm::vec2& screenSize
) {
    if (structure == All) return;

    glm::mat4 MVP = glm::ortho(0.0f, screenSize.x, screenSize.y, 0.0f) 
        * glm::translate(glm::vec3(mousePosOnScreen + glm::vec2{-75.0f, -150.0f}, 0.0f));

    shader.setUniform("MVP", MVP);

    shader.use();
    glBindTexture(GL_TEXTURE_2D, textures[structure]);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    shader.unuse();
}

StructureRender::StructureRender()
{
    if (!initialized) {
        initialize_structures();
    }
} 

StructureRender::~StructureRender()
{
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteTextures(1, &texture);
    stbi_image_free(imageData.data);
}

void StructureRender::bind(unsigned int structure)
{
    if (structure == Tree) return;
    shader.loadFromFile("../asset/shaders/structure.vs.glsl", "../asset/shaders/structure.fs.glsl");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES) + sizeof(initUVs), VERTICES, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(VERTICES), sizeof(initUVs), initUVs);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)(sizeof(VERTICES)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    std::cout << "Generate VBO, VAO successfully!" << std::endl;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    std::cout << "Successfully generate tex para!" << std::endl;

    _structure = structures[structure];
    height = structureHeight[structure];
    imageData.data = stbi_load(
        imageFileName[structure], 
        &imageData.width, &imageData.height,
        &imageData.channel, 0
    );
    if (imageData.data == NULL)
        std::cerr << "Fail to load image data!\nFile: " << imageFileName[structure] << "\n";
    else 
    {
        std::cout << "Successfully load image!" << std::endl;
        std::cout << imageData.channel << std::endl;
        glTexImage2D(
            GL_TEXTURE_2D, 
            0,
            GL_RGBA,
            imageData.width, imageData.height, 
            0, 
            GL_RGBA, 
            GL_UNSIGNED_BYTE, 
            imageData.data
        );
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0); 
    }

    std::cout << "Successfully bind " << imageFileName[structure] << std::endl;
}

void StructureRender::render(
    const glm::mat4& viewMat, 
    const glm::ivec2& position, 
    const glm::mat4& projMat, 
    float colorRatio)
{
    shader.setUniform("viewMat", viewMat);
    shader.setUniform("projMat", projMat);
    shader.setUniform("scale", glm::scale(glm::vec3(_structure.size.x, height, _structure.size.y)));
    shader.setUniform("translate", glm::translate(glm::vec3(position.x, 0.0f, position.y)));
    shader.setUniform("colorRatio", colorRatio);

    shader.use();
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);

    shader.unuse();
}
