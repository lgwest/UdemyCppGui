#pragma once

#include <cstdint>
#include <string_view>
#include <numbers>
#include <tuple>

class ClockClass
{
private:
    static constexpr auto PI = std::numbers::pi_v<float>;
    static constexpr auto circleRadius = 250.0F;
    static constexpr auto offset = PI / 2.0F;
    static constexpr auto innerRadius = 5.0F;
    static constexpr auto hrsClockHandLength = 0.95F;
    static constexpr auto minsClockHandLength = 0.85F;
    static constexpr auto secsClockHandLength = 0.75F;
    static constexpr auto hrsStrokesLength = 0.90F;
    static constexpr auto minsStrokesLength = 0.95F;

public:
    ClockClass() : secs(0), mins(0), hrs(0), center({}){};
    void Draw(std::string_view label);

private:
    void DrawCircle(const float radius);
    void DrawClockHand(const float radius, const float theta, const ImColor color);
    void DrawAllHourStrokes();
    void DrawAllMinuteStrokes();
    void DrawDigitalClock();

    void GetTime();
    std::tuple<float, float, float> GetTheta();

public:
    std::int32_t secs;
    std::int32_t mins;
    std::int32_t hrs;

private:
    ImVec2 center;
};

void render(ClockClass &window_obj);
