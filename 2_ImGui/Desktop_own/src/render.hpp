#pragma once

#include <cstdint>
#include <string_view>
#include <string>
#include <vector>

#include "clock.hpp"

class WindowClass
{
public:
    constexpr static auto numIcons = uint32_t{10};

    struct Icon
    {
        Icon(std::string_view label_) : 
                    label(label_), 
                    position(ImVec2{}),
                    popupOpen(false),
                    clickCount(0){};
        void Draw();

        std::string label;
        ImVec2 position;
        bool popupOpen;
        uint32_t clickCount;
    };

public:
    void Draw(std::string_view label);

private:
    void DrawDesktop();
    void DrawTaskbar();

    void ShowIconList(bool *open = nullptr);

private:
    Clock clock;
};

void render(WindowClass &window_obj);
