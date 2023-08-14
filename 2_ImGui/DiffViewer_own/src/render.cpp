#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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

    if (ImGui::Button("Compare")) 
    {
        fileContent1 = LoadFileContent(filePath1);
        fileContent2 = LoadFileContent(filePath2);

        CreateDiff();
    }
}

void WindowClass::DrawDiffView()
{
    for (std::size_t i = 0; i < fileContent1.size(); ++i)
    {
        ImGui::Text("%s", fileContent1[i].data());
    }

    for (std::size_t i = 0; i < fileContent2.size(); ++i)
    {
        ImGui::Text("%s", fileContent2[i].data());
    }
}

void WindowClass::DrawStats()
{
}

WindowClass::FileContent WindowClass::LoadFileContent(std::string_view file_path)
{
    auto content = FileContent{};
    auto in = std::ifstream(file_path.data());

    if (in.is_open())
    {
        auto line = std::string{};
        while (std::getline(in, line));
        {
            content.push_back(line);
        }

        in.close();
    }

    return content;
}

void WindowClass::SaveFileContent(std::string_view file_path, FileContent file_content)
{
    auto out = std::ofstream(file_path.data());

    if (out.is_open())
    {
        auto line = std::string{};
        for (const auto &line : file_content) 
        {
            out << line << '\n';
        }

        out.close();
    }
}

void WindowClass::CreateDiff()
{
    diffResult1.clear();
    diffResult2.clear();

    const auto max_num_lines = std::max(fileContent1.size(), fileContent2.size());

    for (std::size_t i = 0; i < max_num_lines; ++i)
    {
        const auto line1 = i < fileContent1.size() ? fileContent1[1] : "EMPTY";
        const auto line2 = i < fileContent2.size() ? fileContent2[1] : "EMPTY";

        if (line1 != line2)
        {
            diffResult1.push_back(line1);
            diffResult2.push_back(line2);
        }
        else
        {
            diffResult1.push_back("");
            diffResult2.push_back("");
        }
    }
}


void render(WindowClass &window_obj)
{
    window_obj.Draw("DiffViewer[own]");
}
