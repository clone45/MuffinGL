// texture.cpp
#include "texture.hpp"
#include "graphics.hpp"
#include <SDL2/SDL_image.h>
#include <stdexcept>

Texture::Texture(Graphics& graphics)
    : m_graphics(graphics)
    , m_texture(nullptr)
    , m_width(0)
    , m_height(0)
{
}

Texture::Texture(Texture&& other) noexcept
    : m_graphics(other.m_graphics)
    , m_texture(other.m_texture)
    , m_width(other.m_width)
    , m_height(other.m_height)
{
    other.m_texture = nullptr;  // Prevent double deletion
    other.m_width = 0;
    other.m_height = 0;
}

Texture::~Texture() {
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
    }
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        if (m_texture) {
            SDL_DestroyTexture(m_texture);
        }
        
        m_texture = other.m_texture;
        m_width = other.m_width;
        m_height = other.m_height;
        
        other.m_texture = nullptr;
        other.m_width = 0;
        other.m_height = 0;
    }
    return *this;
}

Texture Texture::create(Graphics& graphics, int width, int height) {
    Texture texture(graphics);
    
    texture.m_texture = SDL_CreateTexture(
        graphics.getRenderer(),
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        width, height
    );

    if (!texture.m_texture) {
        throw std::runtime_error("Failed to create texture: " + std::string(SDL_GetError()));
    }

    // Enable alpha blending
    SDL_SetTextureBlendMode(texture.m_texture, SDL_BLENDMODE_BLEND);
    
    texture.m_width = width;
    texture.m_height = height;
    
    return texture;
}

Texture Texture::create(Graphics& graphics, const std::string& path) {
    Texture texture(graphics);
    
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        throw std::runtime_error("Failed to load image: " + std::string(IMG_GetError()));
    }

    texture.m_texture = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
    texture.m_width = surface->w;
    texture.m_height = surface->h;
    
    SDL_FreeSurface(surface);
    
    if (!texture.m_texture) {
        throw std::runtime_error("Failed to create texture from surface: " + std::string(SDL_GetError()));
    }

    SDL_SetTextureBlendMode(texture.m_texture, SDL_BLENDMODE_BLEND);
    
    return texture;
}

void Texture::draw(int x, int y) {
    SDL_Rect destRect = { x, y, m_width, m_height };
    SDL_RenderCopy(m_graphics.getRenderer(), m_texture, nullptr, &destRect);
}

void Texture::draw(Texture& target, int x, int y) {
    // Store current render target
    SDL_Texture* previousTarget = SDL_GetRenderTarget(m_graphics.getRenderer());
    
    // Set the target texture as render target
    SDL_SetRenderTarget(m_graphics.getRenderer(), target.m_texture);
    
    // Draw to the target
    SDL_Rect destRect = { x, y, m_width, m_height };
    SDL_RenderCopy(m_graphics.getRenderer(), m_texture, nullptr, &destRect);
    
    // Restore previous render target
    SDL_SetRenderTarget(m_graphics.getRenderer(), previousTarget);
}

// In texture.cpp
void Texture::resize(int width, int height, ScaleMode mode) {
    // Create new texture
    SDL_Texture* newTexture = SDL_CreateTexture(
        m_graphics.getRenderer(),
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        width, height
    );

    if (!newTexture) {
        throw std::runtime_error("Failed to create resized texture: " + std::string(SDL_GetError()));
    }

    // Set the scaling quality based on mode
    SDL_ScaleMode sdlMode;
    switch (mode) {
        case ScaleMode::Nearest:
            sdlMode = SDL_ScaleModeNearest;
            break;
        case ScaleMode::Linear:
            sdlMode = SDL_ScaleModeLinear;
            break;
        case ScaleMode::Best:
            sdlMode = SDL_ScaleModeBest;
            break;
    }
    
    SDL_SetTextureScaleMode(newTexture, sdlMode);

    // Set up new texture for blending
    SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);

    // Store current render target
    SDL_Texture* previousTarget = SDL_GetRenderTarget(m_graphics.getRenderer());

    // Set new texture as render target
    SDL_SetRenderTarget(m_graphics.getRenderer(), newTexture);

    // Clear the new texture
    SDL_SetRenderDrawColor(m_graphics.getRenderer(), 0, 0, 0, 0);
    SDL_RenderClear(m_graphics.getRenderer());

    // Draw old texture scaled to new size
    SDL_Rect destRect = { 0, 0, width, height };
    SDL_RenderCopy(m_graphics.getRenderer(), m_texture, nullptr, &destRect);

    // Restore previous render target
    SDL_SetRenderTarget(m_graphics.getRenderer(), previousTarget);

    // Clean up old texture and update members
    SDL_DestroyTexture(m_texture);
    m_texture = newTexture;
    m_width = width;
    m_height = height;
}

// BitBlt entire texture to screen
void Texture::render(int destX, int destY, BlendMode mode) {
    setBlendMode(mode);
    
    SDL_Rect destRect = { destX, destY, m_width, m_height };
    SDL_RenderCopy(m_graphics.getRenderer(), m_texture, nullptr, &destRect);
    
    restoreBlendMode();
}

// BitBlt entire texture to another texture
void Texture::render(Texture& target, int destX, int destY, BlendMode mode) {
    setBlendMode(mode);

    // Store current render target
    SDL_Texture* previousTarget = SDL_GetRenderTarget(m_graphics.getRenderer());
    
    // Set the target texture as render target
    SDL_SetRenderTarget(m_graphics.getRenderer(), target.m_texture);
    
    // BitBlt the entire texture
    SDL_Rect destRect = { destX, destY, m_width, m_height };
    SDL_RenderCopy(m_graphics.getRenderer(), m_texture, nullptr, &destRect);
    
    // Restore previous render target
    SDL_SetRenderTarget(m_graphics.getRenderer(), previousTarget);

    restoreBlendMode();
}

// BitBlt region to screen
void Texture::render(int sourceX, int sourceY, int sourceWidth, int sourceHeight,
                   int destX, int destY, BlendMode mode) {

    setBlendMode(mode);

    // Wrap source coordinates to valid texture positions
    sourceX = sourceX % m_width;
    sourceY = sourceY % m_height;
    
    // Handle negative coordinates by wrapping to positive
    if (sourceX < 0) sourceX += m_width;
    if (sourceY < 0) sourceY += m_height;
    
    // Calculate how much we need to wrap horizontally
    int firstWidth = std::min(sourceWidth, m_width - sourceX);
    int remainingWidth = sourceWidth - firstWidth;
    
    // Calculate how much we need to wrap vertically
    int firstHeight = std::min(sourceHeight, m_height - sourceY);
    int remainingHeight = sourceHeight - firstHeight;
    
    // First segment (top-left)
    SDL_Rect sourceRect = { sourceX, sourceY, firstWidth, firstHeight };
    SDL_Rect destRect = { destX, destY, firstWidth, firstHeight };
    SDL_RenderCopy(m_graphics.getRenderer(), m_texture, &sourceRect, &destRect);
    
    // If we need to wrap horizontally, render the top-right segment
    if (remainingWidth > 0) {
        sourceRect = { 0, sourceY, remainingWidth, firstHeight };
        destRect = { destX + firstWidth, destY, remainingWidth, firstHeight };
        SDL_RenderCopy(m_graphics.getRenderer(), m_texture, &sourceRect, &destRect);
    }
    
    // If we need to wrap vertically, render the bottom-left segment
    if (remainingHeight > 0) {
        sourceRect = { sourceX, 0, firstWidth, remainingHeight };
        destRect = { destX, destY + firstHeight, firstWidth, remainingHeight };
        SDL_RenderCopy(m_graphics.getRenderer(), m_texture, &sourceRect, &destRect);
    }
    
    // If we need to wrap both horizontally and vertically, render the bottom-right segment
    if (remainingWidth > 0 && remainingHeight > 0) {
        sourceRect = { 0, 0, remainingWidth, remainingHeight };
        destRect = { destX + firstWidth, destY + firstHeight, remainingWidth, remainingHeight };
        SDL_RenderCopy(m_graphics.getRenderer(), m_texture, &sourceRect, &destRect);
    }

    restoreBlendMode();
}

void Texture::render(Texture& target,
                   int sourceX, int sourceY, int sourceWidth, int sourceHeight,
                   int destX, int destY, BlendMode mode) {

    setBlendMode(mode);

    // Store current render target
    SDL_Texture* previousTarget = SDL_GetRenderTarget(m_graphics.getRenderer());
    
    // Set the target texture as render target
    SDL_SetRenderTarget(m_graphics.getRenderer(), target.m_texture);
    
    // Wrap source coordinates to valid texture positions
    sourceX = sourceX % m_width;
    sourceY = sourceY % m_height;
    
    // Handle negative coordinates by wrapping to positive
    if (sourceX < 0) sourceX += m_width;
    if (sourceY < 0) sourceY += m_height;
    
    // Calculate how much we need to wrap horizontally
    int firstWidth = std::min(sourceWidth, m_width - sourceX);
    int remainingWidth = sourceWidth - firstWidth;
    
    // Calculate how much we need to wrap vertically
    int firstHeight = std::min(sourceHeight, m_height - sourceY);
    int remainingHeight = sourceHeight - firstHeight;
    
    // First segment (top-left)
    SDL_Rect sourceRect = { sourceX, sourceY, firstWidth, firstHeight };
    SDL_Rect destRect = { destX, destY, firstWidth, firstHeight };
    SDL_RenderCopy(m_graphics.getRenderer(), m_texture, &sourceRect, &destRect);
    
    // If we need to wrap horizontally, render the top-right segment
    if (remainingWidth > 0) {
        sourceRect = { 0, sourceY, remainingWidth, firstHeight };
        destRect = { destX + firstWidth, destY, remainingWidth, firstHeight };
        SDL_RenderCopy(m_graphics.getRenderer(), m_texture, &sourceRect, &destRect);
    }
    
    // If we need to wrap vertically, render the bottom-left segment
    if (remainingHeight > 0) {
        sourceRect = { sourceX, 0, firstWidth, remainingHeight };
        destRect = { destX, destY + firstHeight, firstWidth, remainingHeight };
        SDL_RenderCopy(m_graphics.getRenderer(), m_texture, &sourceRect, &destRect);
    }
    
    // If we need to wrap both horizontally and vertically, render the bottom-right segment
    if (remainingWidth > 0 && remainingHeight > 0) {
        sourceRect = { 0, 0, remainingWidth, remainingHeight };
        destRect = { destX + firstWidth, destY + firstHeight, remainingWidth, remainingHeight };
        SDL_RenderCopy(m_graphics.getRenderer(), m_texture, &sourceRect, &destRect);
    }
    
    // Restore previous render target
    SDL_SetRenderTarget(m_graphics.getRenderer(), previousTarget);

    restoreBlendMode();
}

