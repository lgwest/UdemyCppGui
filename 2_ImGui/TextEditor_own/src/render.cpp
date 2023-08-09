#include <iostream>

#include <fmt/format.h>
#include <imgui.h>
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

    DrawMenu();
    DrawContent();
    DrawInfo();

    ImGui::End();
}

void WindowClass::DrawMenu()
{
    const auto ctrl_pressed = ImGui::GetIO().KeyCtrl;
    const auto esc_pressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape));
    const auto s_pressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_S));
    const auto l_pressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_L));
    
    if (ImGui::Button("Save") || (ctrl_pressed && s_pressed))
    {
        ImGui::OpenPopup("Save File");
    }

    ImGui::SameLine();

    if (ImGui::Button("Load")  || (ctrl_pressed && l_pressed))
    {
        ImGui::OpenPopup("Load File");
    }

    ImGui::SameLine();

    if (ImGui::Button("Clear"))
    {
        std::memset(textBuffer, 0, bufferSize);
    }

    DrawSavePopup();
    DrawLoadPopup();
}
void WindowClass::DrawSavePopup()
{
    static char saveFilenameBuffer[256] = "text.txt";
    const auto esc_pressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape));

    if (ImGui::BeginPopupModal("Save File"))
    {
        ImGui::InputText("Filename", saveFilenameBuffer, sizeof(saveFilenameBuffer));
        if (ImGui::Button("Save")) {
            SaveToFile(saveFilenameBuffer);
            currentFilename = saveFilenameBuffer;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();

        if (ImGui::Button("Cancel") || esc_pressed) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void WindowClass::DrawLoadPopup()
{
    static char loadFilenameBuffer[256] = "text.txt";
    const auto esc_pressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape));

    if (ImGui::BeginPopupModal("Load File"))
    {

        ImGui::InputText("Filename", loadFilenameBuffer, sizeof(loadFilenameBuffer));
        if (ImGui::Button("Load")) {
            LoadFromFile(loadFilenameBuffer);
            currentFilename = loadFilenameBuffer;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();

        if (ImGui::Button("Cancel") || esc_pressed) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();    }

}

void WindowClass::DrawContent()
{

}

void WindowClass::DrawInfo()
{

}

void WindowClass::SaveToFile(std::string_view filename)
{

}

void WindowClass::LoadFromFile(std::string_view filename)
{

}

std::string WindowClass::GetFileExtension(std::string_view filename)
{
    return "";
}

void render(WindowClass &window_obj)
{
    window_obj.Draw("Texteditor[own]");
}
