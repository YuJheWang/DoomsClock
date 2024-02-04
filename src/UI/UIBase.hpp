#pragma once

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

class UIBase
{
public:

    UIBase() = default;

    virtual void render() = 0;

};
