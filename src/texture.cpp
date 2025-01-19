// texture.cpp
#include "texture.hpp"
#include "graphics.hpp"
#include <SDL2/SDL_image.h>
#include <stdexcept>
#include <iostream>
#include <vector>

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
    

    // Set SDL_HINT_RENDER_SCALE_QUALITY to best (2)
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

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

    texture.m_surface = IMG_Load(path.c_str());
    if (!texture.m_surface) {
        throw std::runtime_error("Failed to load image: " + std::string(IMG_GetError()));
    }

    texture.m_texture = SDL_CreateTextureFromSurface(graphics.getRenderer(), texture.m_surface);
    if (!texture.m_texture) {
        throw std::runtime_error("Failed to create texture: " + std::string(SDL_GetError()));
    }

    texture.m_width = texture.m_surface->w;
    texture.m_height = texture.m_surface->h;
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

void Texture::resize(int width, int height, ScaleMode mode) {
    // Create a surface from the current texture
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(
        0, m_width, m_height, 32, SDL_PIXELFORMAT_RGBA8888
    );
    if (!surface) {
        throw std::runtime_error("Failed to create surface: " + std::string(SDL_GetError()));
    }

    // Set blending mode on surface to NONE (no blending during pixel operations)
    SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);

    // Create temporary texture and read pixels into surface
    SDL_Texture* target = SDL_CreateTexture(
        m_graphics.getRenderer(),
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        m_width, m_height
    );
    if (!target) {
        SDL_FreeSurface(surface);
        throw std::runtime_error("Failed to create target: " + std::string(SDL_GetError()));
    }

    // Store current render target
    SDL_Texture* previousTarget = SDL_GetRenderTarget(m_graphics.getRenderer());

    // Clear the target texture to fully transparent
    SDL_SetRenderTarget(m_graphics.getRenderer(), target);
    SDL_SetRenderDrawColor(m_graphics.getRenderer(), 0, 0, 0, 0);
    SDL_RenderClear(m_graphics.getRenderer());

    // Disable blending on the source texture
    SDL_BlendMode previousBlendMode;
    SDL_GetTextureBlendMode(m_texture, &previousBlendMode);
    SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_NONE);

    // Copy current texture to target
    SDL_RenderCopy(m_graphics.getRenderer(), m_texture, nullptr, nullptr);

    // Restore the source texture's blend mode
    SDL_SetTextureBlendMode(m_texture, previousBlendMode);

    // Read pixels into surface
    SDL_RenderReadPixels(
        m_graphics.getRenderer(), nullptr,
        SDL_PIXELFORMAT_RGBA8888,
        surface->pixels, surface->pitch
    );

    // Restore render target
    SDL_SetRenderTarget(m_graphics.getRenderer(), previousTarget);
    SDL_DestroyTexture(target);

    // Create scaled surface
    SDL_Surface* scaledSurface = SDL_CreateRGBSurfaceWithFormat(
        0, width, height, 32, SDL_PIXELFORMAT_RGBA8888
    );
    if (!scaledSurface) {
        SDL_FreeSurface(surface);
        throw std::runtime_error("Failed to create scaled surface: " + std::string(SDL_GetError()));
    }

    // Set blending mode on the scaled surface to NONE
    SDL_SetSurfaceBlendMode(scaledSurface, SDL_BLENDMODE_NONE);

    // Scale the surface
    SDL_BlitScaled(surface, nullptr, scaledSurface, nullptr);

    // Create new texture from scaled surface
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(m_graphics.getRenderer(), scaledSurface);
    if (!newTexture) {
        SDL_FreeSurface(surface);
        SDL_FreeSurface(scaledSurface);
        throw std::runtime_error("Failed to create new texture: " + std::string(SDL_GetError()));
    }

    // Set blend mode on new texture
    SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);

    // Clean up
    SDL_FreeSurface(surface);
    SDL_FreeSurface(scaledSurface);
    SDL_DestroyTexture(m_texture);

    // Update member variables
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
    // Set the blend mode for the source texture
    setBlendMode(mode);

    // Store the current render target
    SDL_Texture* previousTarget = SDL_GetRenderTarget(m_graphics.getRenderer());

    // Set the target texture as the render target
    SDL_SetRenderTarget(m_graphics.getRenderer(), target.m_texture);

    // Ensure the target area is cleared (optional, depending on use case)
    if (mode == BlendMode::Alpha) {
        SDL_SetRenderDrawBlendMode(m_graphics.getRenderer(), SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(m_graphics.getRenderer(), 0, 0, 0, 0); // Fully transparent
        SDL_Rect clearRect = { destX, destY, m_width, m_height };
        SDL_RenderFillRect(m_graphics.getRenderer(), &clearRect);
    }

    // Render the source texture onto the target
    SDL_Rect destRect = { destX, destY, m_width, m_height };
    SDL_RenderCopy(m_graphics.getRenderer(), m_texture, nullptr, &destRect);

    // Restore the previous render target
    SDL_SetRenderTarget(m_graphics.getRenderer(), previousTarget);

    // Restore the blend mode
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

Color Texture::getPixel(int x, int y) const {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        throw std::runtime_error("Pixel coordinates out of bounds");
    }
    Uint32* pixels = static_cast<Uint32*>(m_surface->pixels);
    Uint32 pixel = pixels[y * m_width + x];

    Uint8 r, g, b, a;
    SDL_GetRGBA(pixel, m_surface->format, &r, &g, &b, &a);

    return Color{r, g, b, a};
}

void Texture::clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    // Store current render target
    SDL_Texture* previousTarget = SDL_GetRenderTarget(m_graphics.getRenderer());
    
    // Set this texture as render target
    SDL_SetRenderTarget(m_graphics.getRenderer(), m_texture);
    
    // Clear with specified color
    SDL_SetRenderDrawColor(m_graphics.getRenderer(), r, g, b, a);
    SDL_RenderClear(m_graphics.getRenderer());
    
    // Restore previous target
    SDL_SetRenderTarget(m_graphics.getRenderer(), previousTarget);
}

bool Texture::save(const std::string& path) const {
    // Create a surface to hold our texture data
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(
        0, m_width, m_height, 32, SDL_PIXELFORMAT_RGBA32
    );
    
    if (!surface) {
        return false;
    }
    
    // Store current render target
    SDL_Texture* previousTarget = SDL_GetRenderTarget(m_graphics.getRenderer());
    
    // Set up a temporary texture for reading
    SDL_Texture* readTexture = SDL_CreateTexture(
        m_graphics.getRenderer(),
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_TARGET,
        m_width, m_height
    );
    
    if (!readTexture) {
        SDL_FreeSurface(surface);
        return false;
    }
    
    // Clear the intermediate texture to fully transparent
    SDL_SetRenderTarget(m_graphics.getRenderer(), readTexture);
    SDL_SetRenderDrawColor(m_graphics.getRenderer(), 0, 0, 0, 0);
    SDL_RenderClear(m_graphics.getRenderer());
    
    // Disable blending on the source texture
    SDL_BlendMode previousBlendMode;
    SDL_GetTextureBlendMode(m_texture, &previousBlendMode);
    SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_NONE);
    
    // Copy the source texture to the intermediate texture
    SDL_RenderCopy(m_graphics.getRenderer(), m_texture, nullptr, nullptr);
    
    // Restore the source texture's blend mode
    SDL_SetTextureBlendMode(m_texture, previousBlendMode);
    
    // Read the pixels into the surface
    SDL_RenderReadPixels(
        m_graphics.getRenderer(),
        nullptr,
        SDL_PIXELFORMAT_RGBA32,
        surface->pixels,
        surface->pitch
    );
    
    // Restore previous render target
    SDL_SetRenderTarget(m_graphics.getRenderer(), previousTarget);
    SDL_DestroyTexture(readTexture);
    
    // Save the surface as a PNG file
    bool success = (IMG_SavePNG(surface, path.c_str()) == 0);
    SDL_FreeSurface(surface);
    
    return success;
}
