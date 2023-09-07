#include <algorithm>
#include <array>
#include <chrono>
#include <compare>
#include <cstdint>
#include <map>
#include <string_view>
#include <iostream>
#include <vector>

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

    DrawDateCombo();
    ImGui::Separator();
    DrawCalendar();
    ImGui::Separator();
    DrawMeetingList();

    if (addMeetingWindowOpen)
    {
        DrawAddMeetingWindow();
    }

    ImGui::End();
}

void WindowClass::DrawDateCombo()
{
    ImGui::Text("Select a date:");
    ImGui::PushItemWidth(50);
    if (ImGui::BeginCombo("##day", std::to_string(selectedDay).data()))
    {
        for (std::int32_t day_idx = 1; day_idx <= 31; day_idx++)
        {
            const auto curr_date = 
                std::chrono::year_month_day(std::chrono::year(selectedYear),
                                            std::chrono::month(selectedMonth),
                                            std::chrono::day(day_idx));
            if (!curr_date.ok())
            {
                break;
            }

            if (ImGui::Selectable(std::to_string(day_idx).data(), day_idx == selectedDay))
            {
                selectedDay = day_idx;
                selectedDate = curr_date;
            }
        }

        ImGui::EndCombo();
    }

    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::PushItemWidth(100);

    if (ImGui::BeginCombo("##month", monthNames[selectedMonth - 1].data()))
    {
        for (std::int32_t month_idx = 1; month_idx <= 12;  month_idx++)
        {
            const auto curr_date = 
                std::chrono::year_month_day(std::chrono::year(selectedYear),
                                            std::chrono::month(month_idx),
                                            std::chrono::day(selectedDay));
            if (!curr_date.ok())
            {
                break;
            }

            if (ImGui::Selectable(monthNames[month_idx - 1].data(), month_idx == selectedMonth))
            {
                selectedMonth = month_idx;
                selectedDate = curr_date;
            }
        }

        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::PushItemWidth(60);

    if (ImGui::BeginCombo("##year", std::to_string(selectedYear).data()))
    {
        for (std::int32_t year_idx = minYear; year_idx < maxYear; year_idx++)
        {
            const auto curr_date = 
                std::chrono::year_month_day(std::chrono::year(year_idx),
                                            std::chrono::month(selectedMonth),
                                            std::chrono::day(selectedDay));
            if (!curr_date.ok())
            {
                break;
            }

            if (ImGui::Selectable(std::to_string(year_idx).data(), year_idx == selectedDay))
            {
                selectedYear = year_idx;
                selectedDate = curr_date;
            }
        }

        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();

    if (ImGui::Button("Add Meeting"))
    {
        addMeetingWindowOpen = true;
    }
}

void WindowClass::DrawCalendar()
{
    const auto child_size = ImVec2(ImGui::GetContentRegionAvail().x, 400.0);
    ImGui::BeginChild("###calendar", child_size, false);

    const auto original_font_size = ImGui::GetFontSize();
    ImGui::SetWindowFontScale(calendarFontSize);

    const auto y = selectedYear;
    for (std::int32_t m = 1; m <= 12; ++m)
    {
        ImGui::Text("%s", fmt::format("{:.3}", monthNames[m - 1]).data());
        ImGui::SameLine();

        for (std::int32_t d = 1; d <= 31; ++d)
        {
            const auto curr_date = 
                std::chrono::year_month_day(std::chrono::year(y),
                                            std::chrono::month(m),
                                            std::chrono::day(d));
            if (!curr_date.ok())
                break;
            
            ImGui::SameLine();
            ImGui::Text("%d", d);
        }
    }
    ImGui::SetWindowFontScale(original_font_size);
    
    ImGui::EndChild();
}

void WindowClass::DrawAddMeetingWindow()
{
}

void WindowClass::DrawMeetingList()
{
}

void WindowClass::LoadMeetingsFromFile(std::string_view filename)
{
}

void WindowClass::SaveMeetingsToFile(std::string_view filename)
{
}

void WindowClass::UpdateSelectedDateVariables()
{
}

void WindowClass::Meeting::Serialize(std::ofstream &out)
{
}

WindowClass::Meeting WindowClass::Meeting::Deserialize(std::ifstream &in)
{
    WindowClass::Meeting m{};
    return m;
}

void render(WindowClass &window_obj)
{
    window_obj.Draw("Calendar[own]");
}
