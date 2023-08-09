#pragma once

#include <cstdint>
#include <string_view>

class WindowClass
{
public:
    static constexpr auto bufferSize =  std::size_t{1024};

public:
    WindowClass() : currentFilename({})
    {
        std::memset(textBuffer, 0, bufferSize);
    }

public:
    void Draw(std::string_view label);

private:
    void DrawMenu();
    void DrawContent();
    void DrawInfo();
    
    void SaveToFile(std::string_view filename);
    void LoadFromFile(std::string_view filename);
    std::string GetFileExtension(std::string_view filename);
    void DrawSavePopup();
    void DrawLoadPopup();

private:
    char textBuffer[bufferSize];
    std::string currentFilename;
};

void render(WindowClass &window_obj);
