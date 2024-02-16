#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <cstring>

#include <glad/glad.h>
#include <GameLogic/Structures.hpp>

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
    1.0f, 0.0f,
    1.0f, 0.75f,
    0.5f, 1.0f,
    0.0f, 0.75f,
    0.0f, 0.0f//,
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
    "../asset/AIGenerated/bgRemoved/firePowerPlant.png",
    "../asset/AIGenerated/bgRemoved/nuclearPlant.png",
    "../asset/AIGenerated/bgRemoved/fireDepartment.png",
    "../asset/AIGenerated/bgRemoved/windPowerPlant.png",
    "../asset/AIGenerated/bgRemoved/residentialArea.png",
    "../asset/AIGenerated/bgRemoved/park.png",
    "../asset/AIGenerated/bgRemoved/coal.png",
    "../asset/AIGenerated/bgRemoved/farm.png",
    "../asset/AIGenerated/bgRemoved/storage.png"
};

float structureHeight[All] = {
    2.0f,
    4.0f,
    2.0f,
    4.0f,
    4.0f,
    4.0f,
    4.0f,
    2.0f,
    1.0f,
    4.0f
};

class StructureRender
{
public:

    StructureRender();

    ~StructureRender();

    void bind(unsigned int structure);

    void render(const glm::mat4& viewMat, const glm::ivec2& position, const glm::mat4& projMat);

private:

    GLuint vbo, vao;

    GLuint texture;

    Shader shader;

    Structure _structure;

    float height;

    ImageData imageData;

    bool initialized = false;

};

StructureRender::StructureRender()
{
    if (!initialized) initialize_structures();
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
    if (
        structure == Factory || structure == WindPowerPlant || structure == Coal ||
        structure == Farm    || structure == Storage
    ) return;

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

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

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
}

void StructureRender::render(const glm::mat4& viewMat, const glm::ivec2& position, const glm::mat4& projMat)
{
    shader.setUniform("viewMat", viewMat);
    shader.setUniform("projMat", projMat);
    shader.setUniform("scale", glm::scale(glm::vec3(_structure.size.x, height, _structure.size.y)));
    shader.setUniform("translate", glm::translate(glm::vec3(position.x, 0.0f, position.y)));

    shader.use();
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);

    shader.unuse();
}
