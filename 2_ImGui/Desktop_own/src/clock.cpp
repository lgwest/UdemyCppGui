#include <iostream>
#include <chrono>
#include <tuple>
#include <cmath>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "clock.hpp"

void Clock::Draw(std::string_view label)
{
    constexpr static auto window_flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
    constexpr static auto window_pos = ImVec2(0.0F, 0.0F);

    ImGui::SetNextWindowSize(window_size);
    ImGui::SetNextWindowPos(window_pos);

    ImGui::Begin(label.data(), nullptr, window_flags);

    const auto cursor_pos = ImGui::GetCursorScreenPos();
    center = ImVec2(cursor_pos.x + circleRadius, cursor_pos.y + circleRadius);

    DrawCircle(circleRadius);

    GetTime();
    const auto [hour_theta, minute_theta, second_theta] = GetTheta();

    DrawClockHand(circleRadius * hrsClockHandLength, hour_theta, ImColor(1.0F, 0.0F, 0.0F, 1.0F));
    DrawClockHand(circleRadius * minsClockHandLength, minute_theta, ImColor(0.0F, 1.0F, 0.0F, 1.0F));
    DrawClockHand(circleRadius * secsClockHandLength, second_theta, ImColor(0.0F, 0.0F, 1.0F, 1.0F));

    DrawAllHourStrokes();
    DrawAllMinuteStrokes();

    DrawCircle(innerRadius);

    DrawDigitalClock();

    ImGui::End();
}

void Clock::DrawCircle(const float radius)
{
    ImGui::GetWindowDrawList()->AddCircle(center, radius, ImGui::GetColorU32(ImGuiCol_Text), 100, 2.0F);
}

void Clock::DrawClockHand(const float radius,
                                const float theta,
                                const ImColor color)
{
    const auto c = cos(theta); // x
    const auto s = sin(theta); // y
    const auto end_point = ImVec2(center.x - radius * c, center.y - radius * s);

    ImGui::GetWindowDrawList()->AddLine(center, end_point, color, 3.0F );
}

void Clock::DrawAllHourStrokes()
{
    for (uint32_t hr = 0; hr < 12; ++hr)
    {
        const auto theta = (hr *((2.0 * PI) / 12.0F)) + offset;
        const auto c = cos(theta); // x
        const auto s = sin(theta); // y   
        const auto start_point = ImVec2(center.x + (circleRadius * hrsStrokesLength) * c, 
                                        center.y + (circleRadius * hrsStrokesLength) * s);
        const auto end_point = ImVec2(center.x + circleRadius * c, center.y + circleRadius * s);

        ImGui::GetWindowDrawList()->AddLine(start_point, end_point, ImGui::GetColorU32(ImGuiCol_Text), 2.0F);
    }
}

void Clock::DrawAllMinuteStrokes()
{
    for (uint32_t mins = 0; mins < 60; ++mins)
    {
        const auto theta = (mins *((2.0 * PI) / 60.0F)) + offset;
        const auto c = cos(theta); // x
        const auto s = sin(theta); // y   
        const auto start_point = ImVec2(center.x + (circleRadius * minsStrokesLength) * c, 
                                        center.y + (circleRadius * minsStrokesLength) * s);
        const auto end_point = ImVec2(center.x + circleRadius * c, center.y + circleRadius * s);

        ImGui::GetWindowDrawList()->AddLine(start_point, end_point, ImGui::GetColorU32(ImGuiCol_Text), 1.0F);
    }}

void Clock::DrawDigitalClock()
{
    ImGui::Text("%02d:%02d:%02d", hrs, mins, secs);
}

void Clock::GetTime()
{
    const auto timestamp_now = std::chrono::system_clock::now();
    const auto time_now = std::chrono::system_clock::to_time_t(timestamp_now);
    const auto time_struct = std::localtime(&time_now);

    secs = time_struct->tm_sec;
    mins = time_struct->tm_min;
    hrs = time_struct->tm_hour;
}

std::tuple<float, float, float> Clock::GetTheta()
{
    const auto seconds_frac = static_cast<float>(secs);
    const auto minutes_frac = static_cast<float>(mins) + seconds_frac / 60.0F;
    const auto hours_frac = static_cast<float>(hrs) + minutes_frac / 60.0F;

    const auto hour_theta = (hours_frac * ((2.0F * PI) / 12.0F)) + offset;
    const auto minute_theta = (minutes_frac * ((2.0F * PI) / 60.0F)) + offset;
    const auto second_theta = (seconds_frac * ((2.0F * PI) / 60.0F)) + offset;

    return std::make_tuple(hour_theta, minute_theta, second_theta);
}

void render(Clock &window_obj)
{
    window_obj.Draw("WallClock[own]");
}
