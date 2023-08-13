#include <iostream>

#include <fmt/format.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <implot.h>

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    constexpr static auto window_flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
    constexpr static auto window_pos = ImVec2(0.0F, 0.0F);

    ImGui::SetNextWindowSize(window_size);
    ImGui::SetNextWindowPos(window_pos);

    ImGui::Begin(label.data(), nullptr, window_flags);

    DrawSelection();
    DrawDiffView();
    DrawStats();

    ImGui::End();
}

void WindowClass::DrawSelection()
{
    ImGui::InputText("Left", &filePath1);
    ImGui::SameLine();
    if (ImGui::Button("Save###Left"))
    {
        SaveFileContent(filePath1, fileContent1);
    }

    ImGui::InputText("Right", &filePath2);
    ImGui::SameLine();
    if (ImGui::Button("Save###Right"))
    {
        SaveFileContent(filePath2, fileContent2);
    }
}

void WindowClass::DrawDiffView()
{
}

void WindowClass::DrawStats()
{
}

WindowClass::FileContent WindowClass::LoadFileContent(std::string_view file_path)
{
    return FileContent();
}

void WindowClass::SaveFileContent(std::string_view file_path, FileContent file_content)
{
}

void WindowClass::CreateDiff()
{
}


void render(WindowClass &window_obj)
{
    window_obj.Draw("DiffViewer[own]");
}
