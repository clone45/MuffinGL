// graphics.cpp
#include "graphics.hpp"
#include <SDL2/SDL_image.h>
#include <stdexcept>
#include <iostream>

Graphics::Graphics(int width, int height, const std::string& title) 
    : m_window(nullptr)
    , m_renderer(nullptr)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error("SDL initialization failed: " + std::string(SDL_GetError()));
    }

    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        SDL_Quit();
        throw std::runtime_error("SDL_image initialization failed: " + std::string(IMG_GetError()));
    }

    m_window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_SHOWN
    );

    if (!m_window) {
        IMG_Quit();
        SDL_Quit();
        throw std::runtime_error("Window creation failed: " + std::string(SDL_GetError()));
    }

    m_renderer = SDL_CreateRenderer(
        m_window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!m_renderer) {
        SDL_DestroyWindow(m_window);
        IMG_Quit();
        SDL_Quit();
        throw std::runtime_error("Renderer creation failed: " + std::string(SDL_GetError()));
    }
}

Graphics::~Graphics() {
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
    }
    IMG_Quit();
    SDL_Quit();
}

void Graphics::clear() {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
}

void Graphics::render() {
    SDL_RenderPresent(m_renderer);
}

bool Graphics::pollEvent(Event& event) {
    SDL_Event sdlEvent;
    if (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
            case SDL_QUIT:
                event.type = EventType::Quit;
                return true;

            case SDL_MOUSEMOTION:
                event.type = EventType::MouseMove;
                event.mouseData.x = sdlEvent.motion.x;
                event.mouseData.y = sdlEvent.motion.y;
                return true;

            case SDL_MOUSEBUTTONDOWN:
                event.type = EventType::MouseDown;
                event.mouseData.x = sdlEvent.button.x;
                event.mouseData.y = sdlEvent.button.y;
                event.mouseData.button = sdlEvent.button.button;
                return true;

            case SDL_MOUSEBUTTONUP:
                event.type = EventType::MouseUp;
                event.mouseData.x = sdlEvent.button.x;
                event.mouseData.y = sdlEvent.button.y;
                event.mouseData.button = sdlEvent.button.button;
                return true;

            case SDL_MOUSEWHEEL:
                event.type = EventType::MouseWheel;
                event.mouseData.wheelX = sdlEvent.wheel.x;
                event.mouseData.wheelY = sdlEvent.wheel.y;
                SDL_GetMouseState(&event.mouseData.x, &event.mouseData.y);
                return true;

            default:
                event.type = EventType::None;
                return true;
        }
    }
    return false;
}