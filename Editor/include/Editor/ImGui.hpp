#pragma once

#include <imgui.h>

namespace vi
{
    class ImGuiImpl
    {
    public:
        static bool Initialize(const void* pWindow);
        static void Shutdown();

        static void NewFrame();
        static void Render();
    };
}