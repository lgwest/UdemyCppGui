#include <algorithm>
#include <array>
#include <chrono>
#include <compare>
#include <cstdint>
#include <map>
#include <string_view>
#include <iostream>
#include <vector>
#include <fstream>

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

    const auto curr_time = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
    const auto todays_date = std::chrono::year_month_day(curr_time);

    const auto y = selectedYear;
    for (std::int32_t m = 1; m <= 12; ++m)
    {
        ImGui::Text("%s", fmt::format("{:.3}", monthNames[m - 1]).data());
        ImGui::SameLine();

        for (std::int32_t d = 1; d <= 31; ++d)
        {
            const auto iteration_date = 
                std::chrono::year_month_day(std::chrono::year(y),
                                            std::chrono::month(m),
                                            std::chrono::day(d));
            if (!iteration_date.ok())
                break;
            
            ImGui::SameLine();

            if (iteration_date == todays_date)
            {
                ImGui::TextColored(ImVec4(0.0F, 1.0F, 0.0F, 1.0F), "%d", d);
            }
            else if (iteration_date == selectedDate)
            {
                ImGui::TextColored(ImVec4(0.0F, 0.0F, 1.0F, 1.0F), "%d", d);
            }
            else if (meetings.contains(iteration_date))
            {
                ImGui::TextColored(ImVec4(1.0F, 0.0F, 0.0F, 1.0F), "%d", d);
            }
            else
            {
                ImGui::Text("%d", d);
            }
            if (ImGui::IsItemClicked())
            {
                selectedDate = iteration_date;
                UpdateSelectedDateVariables();
            }
        }
    }
    ImGui::SetWindowFontScale(original_font_size);
    
    ImGui::EndChild();
}

void WindowClass::DrawAddMeetingWindow()
{
    static char meeting_name_buffer[128] = "...";

    ImGui::SetNextWindowSize(meetingWindowSize);
    ImGui::SetNextWindowPos(meetingWindowPos);
   
    ImGui::Begin("###addMeeting", &addMeetingWindowOpen, meetingWindowFlags);
    ImGui::Text("Add meeting to %d.%s.%d", selectedDay, monthNames[selectedMonth - 1].data(), selectedYear);
    ImGui::InputText("Meeting Name", meeting_name_buffer, sizeof(meeting_name_buffer));

    if (ImGui::Button("Save"))
    {
        meetings[selectedDate].push_back(Meeting{meeting_name_buffer});
        std::memset(meeting_name_buffer, 0, sizeof(meeting_name_buffer));
        addMeetingWindowOpen = false;
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel"))
    {
        addMeetingWindowOpen = false;
    }

    ImGui::End();
}

void WindowClass::DrawMeetingList()
{
    if (meetings.size() == 0)
    {
        ImGui::Text("No meetings at all.");
        return;
    }

    ImGui::Text("Meetings on %d-%s-%d: ", selectedDay, monthNames[selectedMonth - 1].data(), selectedYear);

    if (meetings[selectedDate].empty())
    {
        ImGui::Text("no meetings this day: %d-%s-%d", selectedDay, monthNames[selectedMonth -1].data(), selectedYear);
        return;
    }

    for (auto const& meeting: meetings[selectedDate])
    {
        ImGui::BulletText("%s", meeting.name.data());

        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
        {
            std::erase(meetings[selectedDate], meeting);
            return;
        }
    }
}

void WindowClass::LoadMeetingsFromFile(std::string_view filename)
{
    auto in = std::ifstream(filename.data(), std::ios::binary);

    if (!in || !in.is_open())
        return;

    auto num_meetings = std::size_t{0};
    in.read(reinterpret_cast<char *>(&num_meetings), sizeof(num_meetings));

    for (std::size_t i = 0; i < num_meetings; ++i)
    {
        auto date = std::chrono::year_month_day{};
        in.read(reinterpret_cast<char *>(&date), sizeof(date));

        auto num_meetings_on_that_date = std::size_t{0};
        in.read(reinterpret_cast<char *>(&num_meetings_on_that_date), sizeof(num_meetings_on_that_date));
        
        for (size_t j = 0; j < num_meetings_on_that_date; ++j)
        {
            auto meeting = Meeting::Deserialize(in);
            meetings[date].push_back(meeting);
        }
    }

    in.close();
}

void WindowClass::SaveMeetingsToFile(std::string_view filename)
{
    auto out = std::ofstream(filename.data(), std::ios::binary);

    if (!out || !out.is_open())
        return;

    const auto num_meetings = meetings.size();
    auto x = reinterpret_cast<const char *>(&num_meetings);
    auto n = sizeof(num_meetings);
    out.write(reinterpret_cast<const char *>(&num_meetings),
              sizeof(num_meetings));

    for (const auto &[date, meeting_vec] : meetings)
    {
        out.write(reinterpret_cast<const char *>(&date), sizeof(date));

        const auto meetings_count_on_that_date = meeting_vec.size();
        out.write(reinterpret_cast<const char *>(&meetings_count_on_that_date), sizeof(meetings_count_on_that_date));

        for (auto const& meeting : meeting_vec)
        {
            meeting.Serialize(out);
        }
    }

    out.close();}

void WindowClass::UpdateSelectedDateVariables()
{
    selectedDay = static_cast<int>(selectedDate.day().operator unsigned int());
    selectedMonth = static_cast<int>(selectedDate.month().operator unsigned int());
    selectedYear = static_cast<int>(selectedDate.year());
}

void WindowClass::Meeting::Serialize(std::ofstream &out) const
{
    const auto name_length = name.size();
    out.write(reinterpret_cast<const char *>(&name_length), sizeof(name_length));
    out.write(name.data(), static_cast<std::streamsize>(name.size()));
};

WindowClass::Meeting WindowClass::Meeting::Deserialize(std::ifstream &in)
{
    auto meeting = Meeting{};
    auto name_length = std::size_t{0};

    in.read(reinterpret_cast<char *>(&name_length), sizeof(name_length));
    meeting.name.resize(name_length);

    in.read(meeting.name.data(), static_cast<std::streamsize>(name_length));

    return meeting;
}

void render(WindowClass &window_obj)
{
    window_obj.Draw("Calendar[own]");
}
