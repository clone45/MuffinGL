// src\texture.cpp
#include "muffinGL/texture.hpp"
#include "muffinGL/graphics.hpp"
#include "muffinGL/util.hpp"
#include <SDL2/SDL_image.h>
#include <stdexcept>
#include <iostream>

// Constructor
Texture::Texture(Graphics& graphics)
    : Raster(graphics)  // Call base class constructor
    , m_surface(nullptr)
{
}

Texture::~Texture() {
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
    }
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        // Call base class move assignment
        Raster::operator=(std::move(other));
        
        // Handle the surface
        if (m_surface) {
            SDL_FreeSurface(m_surface);
        }
        m_surface = other.m_surface;
        other.m_surface = nullptr;
    }
    return *this;
}

Texture Texture::create(Graphics& graphics, int width, int height) {
    Texture texture(graphics);  // This now calls Raster constructor too
    
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
    
    // Set dimensions in base class
    texture.m_width = width;
    texture.m_height = height;
    
    return texture;
}

Texture Texture::create(Graphics& graphics, const std::string& path, bool makeTarget) {
    Texture texture(graphics);  // This now calls Raster constructor too

    // Step 1: Load from file normally to preserve alpha
    texture.m_surface = IMG_Load(path.c_str());
    if (!texture.m_surface) {
        throw std::runtime_error("Failed to load image: " + std::string(IMG_GetError()));
    }

    if (!makeTarget) {
        // For non-render targets, just create directly from surface
        texture.m_texture = SDL_CreateTextureFromSurface(graphics.getRenderer(), texture.m_surface);
        if (!texture.m_texture) {
            throw std::runtime_error("Failed to create texture: " + std::string(SDL_GetError()));
        }
    } else {
        // For render targets, create temp texture then copy to target
        SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(), texture.m_surface);
        if (!tempTexture) {
            throw std::runtime_error("Failed to create temp texture: " + std::string(SDL_GetError()));
        }

        // Create the target texture
        texture.m_texture = SDL_CreateTexture(
            graphics.getRenderer(),
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            texture.m_surface->w,
            texture.m_surface->h
        );
        if (!texture.m_texture) {
            SDL_DestroyTexture(tempTexture);
            throw std::runtime_error("Failed to create target texture: " + std::string(SDL_GetError()));
        }

        // Copy temp texture to target
        SDL_SetRenderTarget(graphics.getRenderer(), texture.m_texture);
        SDL_RenderCopy(graphics.getRenderer(), tempTexture, nullptr, nullptr);
        SDL_SetRenderTarget(graphics.getRenderer(), nullptr);

        // Clean up temp texture
        SDL_DestroyTexture(tempTexture);
    }

    // Set dimensions in base class
    texture.m_width = texture.m_surface->w;
    texture.m_height = texture.m_surface->h;
    SDL_SetTextureBlendMode(texture.m_texture, SDL_BLENDMODE_BLEND);

    return texture;
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

    // Create new texture
    SDL_Texture* newTexture = SDL_CreateTexture(
        m_graphics.getRenderer(),
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        width, height
    );
    if (!newTexture) {
        SDL_FreeSurface(surface);
        SDL_FreeSurface(scaledSurface);
        throw std::runtime_error("Failed to create new texture: " + std::string(SDL_GetError()));
    }

    // Set it as render target and copy the scaled surface data to it
    SDL_SetRenderTarget(m_graphics.getRenderer(), newTexture);
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(m_graphics.getRenderer(), scaledSurface);
    SDL_RenderCopy(m_graphics.getRenderer(), tempTexture, nullptr, nullptr);
    SDL_DestroyTexture(tempTexture);
    SDL_SetRenderTarget(m_graphics.getRenderer(), nullptr);

    // Set blend mode on new texture
    SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);

    // Clean up
    SDL_FreeSurface(surface);
    SDL_FreeSurface(scaledSurface);
    SDL_DestroyTexture(m_texture);

    // Update member variables (these are now in base class)
    m_texture = newTexture;
    m_width = width;
    m_height = height;
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

void Texture::applyMask(Texture& mask) {
    if (m_width != mask.m_width || m_height != mask.m_height) {
        throw std::runtime_error("Texture and mask must be the same size");
    }

    // Create a new texture to hold the composite result
    // Note: Using Raster constructor through Texture constructor
    Texture composite(m_graphics);
    composite.m_texture = SDL_CreateTexture(
        m_graphics.getRenderer(),
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        m_width, m_height
    );
    if (!composite.m_texture) {
        throw std::runtime_error("Failed to create composite texture: " + std::string(SDL_GetError()));
    }
    SDL_SetTextureBlendMode(composite.m_texture, SDL_BLENDMODE_BLEND);

    // Store the current render target
    SDL_Texture* previousTarget = SDL_GetRenderTarget(m_graphics.getRenderer());
    SDL_SetRenderTarget(m_graphics.getRenderer(), composite.m_texture);

    // Step 1: Stamp the mask onto the composite texture (overwrite alpha)
    SDL_SetTextureBlendMode(mask.m_texture, SDL_BLENDMODE_NONE);
    SDL_Rect fullRect = { 0, 0, m_width, m_height };
    SDL_RenderCopy(m_graphics.getRenderer(), mask.m_texture, nullptr, &fullRect);

    // Step 2: Stamp the grass onto the composite texture (modulate with alpha)
    SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_MOD);
    SDL_RenderCopy(m_graphics.getRenderer(), m_texture, nullptr, &fullRect);

    // Restore the original render target
    SDL_SetRenderTarget(m_graphics.getRenderer(), previousTarget);
    SDL_SetRenderDrawBlendMode(m_graphics.getRenderer(), SDL_BLENDMODE_BLEND);

    // Step 3: Replace this texture with the composite
    SDL_DestroyTexture(m_texture);
    m_texture = composite.m_texture;

    // The composite texture is now owned by this Texture object
    composite.m_texture = nullptr;
}
