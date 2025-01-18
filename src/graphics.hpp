// graphics.hpp
#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <memory>

class Texture;  // Forward declaration

class Graphics {
public:
    Graphics(int width, int height, const std::string& title);
    ~Graphics();

    // Prevent copying
    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;

    // Event system
    enum class EventType {
        None,
        Quit,
        MouseMove,
        MouseDown,
        MouseUp
    };

    struct MouseData {
        int x, y;
        uint8_t button;
    };

    struct Event {
        EventType type = EventType::None;
        MouseData mouseData = {0, 0, 0};
    };

    bool pollEvent(Event& event);

    // Basic drawing functions
    void clear();
    void drawRectangle(int x, int y, int width, int height);
    void render();

    // For Texture's use
    SDL_Renderer* getRenderer() const { return m_renderer; }

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    friend class Texture;  // Allow Texture to access private members if needed
};