#include "../Renderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "ColorFillTest.h"

namespace test
{
    ColorFillTest::ColorFillTest() : m_ClearColor{0.3f, 0.5f, 0.2f, 1.0f} {}

    ColorFillTest::~ColorFillTest() {}

    void ColorFillTest::OnRender()
    {
        GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }

    void ColorFillTest::OnImGuiRender()
    {
        ImGui::NewFrame();
        {
            ImGui::Begin("Transform");

            ImGui::ColorPicker4("Color", m_ClearColor);

            ImGui::End();
        }
    };
}
